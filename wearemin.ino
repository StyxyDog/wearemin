#include "Adafruit_VL53L0X.h"
#include <RunningMedian.h>
#include <math.h>

////////////////
#include "AudioTools.h"
#include "AudioLibs/AudioA2DP.h"
///////////////

// address we will assign if dual sensor is present
#define CONTROLLER1_ADDRESS 0x30
#define CONTROLLER2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_CONTROLLER1 17
#define SHT_CONTROLLER2 16
#define CONFIG_SWITCH 18
#define CONTROLLER1_SWITCH 23
#define CONTROLLER2_SWITCH 19
#define numberOfnotes 37
//( 48  =49 notes )double notes[numberOfnotes] = {130.81,138.59,146.83,155.56,164.81,174.61,185,196,207.65,220,233.08,246.94,261.63,277.18,293.66,311.13,329.63,349.23,369.99,392,415.3,440,466.16,493.88,523.25,554.37,587.33,622.25,659.26,698.46,739.99,783.99,830.61,880,932.33,987.77,1046.5,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760,1864.66,1975.53,10000};
double notes[numberOfnotes] = { 130.81, 138.59, 146.83, 155.56, 164.81,
                                174.61, 185,    196,    207.65, 220,
                                233.08, 246.94, 261.63, 277.18, 293.66,
                                311.13, 329.63, 349.23, 369.99, 392,
                                415.3,  440,    466.16, 493.88, 523.25,
                                554.37, 587.33, 622.25, 659.26, 698.46,
                                739.99, 783.99, 830.61, 880,    932.33,
                                987.77, 10000};
TaskHandle_t Task1;
RunningMedian controller1Samples = RunningMedian(3);
RunningMedian controller2Samples = RunningMedian(3);

/////////////////////

const char* name = "";//"Uproar Wireless";
SineFromTable<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
BluetoothA2DPSource a2dp_source;


int32_t get_sound_data(uint8_t * data, int32_t len) {
  return sound.readBytes((uint8_t*)data, len);
}


double frequencyTarget, frequency = 500;
int amplitudeTarget, amplitude = 1000;
double frequencyMin = 130.81;
double frequencyMax = 987.77; //1975.53;
int volume = 90;
//////////////////////////




double controller1Range, controller2Range;
int configSwitchState, controller1SwitchState, controller2SwitchState;
double controller1Min = 50, controller1Max = 800; //default range settings
double controller2Min = 50, controller2Max = 800; 

// objects for the vl53l0x
Adafruit_VL53L0X controller1 = Adafruit_VL53L0X();
Adafruit_VL53L0X controller2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

void setup_LOX() {
  //setup the controllers
  pinMode(SHT_CONTROLLER1, OUTPUT);
  pinMode(SHT_CONTROLLER2, OUTPUT);
  pinMode(CONFIG_SWITCH, INPUT_PULLUP);
  pinMode(CONTROLLER1_SWITCH, INPUT_PULLUP); 
  pinMode(CONTROLLER2_SWITCH, INPUT_PULLUP);
  
  delay(500); 
  Serial.println("Starting mesurment control task");
  Serial.println(F("Shutdown pins inited..."));
  digitalWrite(SHT_CONTROLLER1, LOW);
  digitalWrite(SHT_CONTROLLER2, LOW);
  Serial.println(F("Both LOX in reset mode...(pins are low)"));
  Serial.println(F("Starting LOX..."));
    digitalWrite(SHT_CONTROLLER1, LOW);    
  digitalWrite(SHT_CONTROLLER2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_CONTROLLER1, HIGH);
  digitalWrite(SHT_CONTROLLER2, HIGH);
  delay(10);

  // activating CONTROLLER1 and resetting CONTROLLER2
  digitalWrite(SHT_CONTROLLER1, HIGH);
  digitalWrite(SHT_CONTROLLER2, LOW);

  // initing CONTROLLER1
  if(!controller1.begin(CONTROLLER1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating CONTROLLER2
  digitalWrite(SHT_CONTROLLER2, HIGH);
  delay(10);

  //initing CONTROLLER2
  if(!controller2.begin(CONTROLLER2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
  Serial.println("TOF addresses set.");  
}

void setup_BT_wave() {
  auto cfg = sound.defaultConfig();
  cfg.bits_per_sample = 16; cfg.channels = 2; cfg.sample_rate = 44100;
  sound.begin(cfg);
  
  sineWave.begin(cfg, frequency);
  
  // start the bluetooth
  Serial.println("starting A2DP...");
  a2dp_source.set_auto_reconnect(true);
  a2dp_source.start_raw(name, get_sound_data);
  a2dp_source.set_volume(30);



  Serial.println("A2DP is connected now...");
}

void setup() {
  Serial.begin(115200);
  setup_LOX();
  setup_BT_wave();
  
  Serial.println("Calling mesurment control task");
  xTaskCreatePinnedToCore(measureControl, "Task1", 10000, NULL, 1, &Task1, 0); // create a task to run on core 0
}

void measureControl( void * pvParameters ){

  for(;;){
    controller1.rangingTest(&measure1, false);
    controller2.rangingTest(&measure2, false);
    controller1Samples.add(measure1.RangeMilliMeter);
    controller2Samples.add(measure2.RangeMilliMeter);
    controller1Range = controller1Samples.getMedian();
    controller2Range = controller2Samples.getMedian();


    //programming loop
    bool controller1ProgramingModeEntry = true, controller2ProgramingModeEntry = true;
    configSwitchState = digitalRead(CONFIG_SWITCH);
    while (configSwitchState == LOW) { //enter range setup mode
      Serial.print(".");
      if (controller1ProgramingModeEntry == true) {
        controller1Min=8192;
        controller1Max=0;
        controller1ProgramingModeEntry = false;
        Serial.print("Set Default Values");
      }
      if (controller2ProgramingModeEntry == true) {
        controller2Min=8192;
        controller2Max=0;
        controller2ProgramingModeEntry = false;
        Serial.print("Set Default Values");
      }

      controller1SwitchState = digitalRead(CONTROLLER1_SWITCH);
      if (controller1SwitchState == LOW) { //program controller 1
        controller1.rangingTest(&measure1, false);
        controller1Samples.add(measure1.RangeMilliMeter);
        controller1Range = controller1Samples.getMedian();
        Serial.print("Controller 1 : Programming Mode Range : "); Serial.print(controller1Range); Serial.print(" -:- Min : "); Serial.print(controller1Min); Serial.print(" -:- Max :"); Serial.println(controller1Max);
        if (controller1Range > controller1Max && controller1Range < 2047) { controller1Max = controller1Range; Serial.print("SetMax"); }
        if (controller1Range < controller1Min && controller1Range > 0   ) { controller1Min = controller1Range; Serial.print("SetMin"); }   
      }

      controller2SwitchState = digitalRead(CONTROLLER2_SWITCH);
      if (controller2SwitchState == LOW) { //program controller 2
        controller2.rangingTest(&measure2, false); 
        controller2Samples.add(measure2.RangeMilliMeter);
        controller2Range = controller2Samples.getMedian();
        Serial.print("Controller 2 : Programming Mode Range : "); Serial.print(controller2Range); Serial.print(" -:- Min : "); Serial.print(controller2Min); Serial.print(" -:- Max :"); Serial.println(controller2Max);
        if (controller2Range > controller2Max && controller2Range < 2047) { controller2Max = controller2Range; Serial.print("SetMax"); }
        if (controller2Range < controller2Min && controller2Range > 0   ) { controller2Min = controller2Range; Serial.print("SetMin"); }   
      }
      
      delay(10);
      configSwitchState = digitalRead(CONFIG_SWITCH);
    }
    
    controller2SwitchState = digitalRead(CONTROLLER2_SWITCH);
    if (controller2SwitchState == LOW) { 
      volume = 0;
    } else {
      volume = 100;
      controller1Range = constrain(controller1Range, controller1Min, controller1Max);  
      controller2Range = constrain(controller2Range, controller2Min, controller2Max);  
      amplitudeTarget = map(controller2Range,controller2Min,controller2Max, 0,32000);
      amplitude = amplitudeTarget; // no smoothing needed but it would go here
    }
    frequencyTarget = round(frequencyMin * pow(2,((controller1Range - controller1Min) / (controller1Max-controller1Min) * log2(frequencyMax/frequencyMin))));

    controller1SwitchState = digitalRead(CONTROLLER1_SWITCH);
    if (controller1SwitchState == LOW) {
      for (int i = 0; i<numberOfnotes-1 ; i ++) {
        if (frequencyTarget > notes[i] && frequencyTarget < notes[i+1]) { frequency = notes[i]; break; }
      }
    } else { frequency = frequency+((frequencyTarget-frequency)/3); //smooth the progression }
  } 
}

void loop() {

  /* uncomment for troubleshooting!
  Serial.print("frequency%:");
  Serial.println(float(frequency)/(frequencyMax-frequencyMin)*100);
  Serial.print("amplitude%:");
  Serial.println((float(amplitude)/32000)*100);//*/
  a2dp_source.set_volume(volume);
  sineWave.setFrequency(frequency);
  sineWave.setAmplitude(amplitude);
}
