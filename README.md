# wearemin
Wearemin - A Bluetooth Wearable Theremin-like-instrument
First time creating a public github repository...

Printables model https://www.printables.com/model/493077-wearemin-a-bluetooth-wearable-theremin-like-instru

Ohshape printed parts https://cad.onshape.com/documents/0d0a24b4e2c86f02219e36fc/w/3666db507762cade373f59c3/e/f5e392f91edd37cb51f9ae51

youtube video https://youtu.be/JclrQr_ucIw


Below text copied from Printables as a starting point.

Description
My entry for the 2023 Musical Instruments contest.

What is Wearemin?
Using an ESP32, a couple of cheap laser range finders and a few inexpensive components you can make a this portable Theremin-like instrument that connects to a Bluetooth speaker or headphones.  Wearemin (Wearable Theremin). 


How to play.
Connect to a nearby Bluetooth Speaker. Just put a nearby speaker in paring mode. I've found some devices reluctant to pair, but most work fine. 

Raising and lowering one handheld controller causes the pitch of the note to change over several octaves. If you press the thumb control, there is a step change between notes - like running your finger across piano keys. 

The other handheld controller adjusts the volume of the note - or press the thumb control button to instantly mute the sound.

The laser is eye-safe as it is low power and unfocused. However, you should check you are comfortable by doing your own research.

Huge apologies to anyone who decides to watch my video - the production and sound quality is truly awful! 🤣


 

Want to play standing up, against a wall or at a table?
The central control box has a latching button. Press this to enter programming mode. Whilst in programming mode hold down the thumb control and wave your hand between the near and far points of the range you want to play. When you let go of the thumb button that range is stored for that controller - repeat for the other handheld controller and then press the latching button again to exit programming mode and resume play mode.

The Project.
I had the idea for this instrument right at the start of the contest. It has been a very steep learning curve for me in terms of music theory, programming and electronics. I hope that either myself or others will improve my code and electronics in time. I was careful not to research the theremin or other devices too deeply so I can't say if this device is unique, but I believe it is - this is all my own work. Its taken me the full two months to get to this point, not helped with a period of ill health and delays in getting parts and supplies shipped out to me. What a great challenge it has been! Most of the code is built from example code that is supplied to use with the VL53L0X time of flight module. Also the amazing (and to me almost incomprehensible) work of Phil Schatzmann who seems to have provided code and examples of almost every sort of ESP32 to Bluetooth audio connection.

Below you will find the code and instructions to build this fun instrument. Maybe you can even make it play a tune! There is a lot of potential.

The Hardware.
I have built the device to be very easy to assemble, basic soldering is required, but I have tried to use breakout boards and DuPont connectors where possible.

Printed parts.
Quickly making an ergonomic handheld device would be a difficult job by almost any other process, but 3D printing makes this whole project possible. This is a fairly simple print, You need to print 2 each of the handheld controllers (top, base and cable clip), and 1 each of the central control box base and lid. My Ender 3 struggled with some of the finer details, wish I had a Pruser 😅. 20% infill and a 0.2 layer height is fine. 

Bill of Materials.
Please see pictures to confirm you are using similar parts.

For the central controller:
an ESP32 development board;
an ESP32 breakout board;
2 x RJ45 Ethernet network LAN 5 wall end plug module adapter jack;
USB C breakout board;
a couple of meters of Ethernet patch lead with connectors, ideally look for a stranded one for a little more flexibility. Solid core is fine too though;
a 10mm latching button.
For the handheld controllers:
2 x 16mm round momentary push buttons;
2 x VL53L0X Time of flight laser range finders (these sound expensive, but are very cheap). Be careful to get the right format. You can also try the VL53L1X for a longer range wearemin. They will fit with the lens in the controller, but I didn't have time to fully make these longer range modules work reliably.
Other hardware:
14 x M2 screws (approx. 5 mm) to hold the VL53L0X and USB-C & ESP32 breakout boards in place. Also these will hold the central controller case closed;
8 x M3 screws (approx. 10 mm) to hold the cable clips and hand controllers together;
Some spare bits of wire for connecting everything up - I just used some of the Ethernet cable cores;
Tools for soldering and assembly. DuPont connectors and crimps are nice to keep things simple and neat, as are some short lengths of shrink tube;
USB power source, battery pack for true wireless; 
Bluetooth speaker or headphones.
Assembly Instructions.
The handheld controllers.
You need an Ethernet/patch lead with only one plug head on, so either cut a longer one in half or cut the ends off two shorter cables. Strip back about 10 cm of the plastic outer sheathing.


I used an angled header on the VL53L0X to save space. I connected via DuPont, but 

soldering connections would be good too. Connections as follows:

Orange to  VL53L0X VIN pin;
Brown to VL53L0X GND pin;
Blue to VL53L0X SCL pin;
Blue/White to VL53L0X SDA pin;
Orange/White to VL53L0X XSHUT pin;
Green to the thumb button (either terminal);
Green/White to the thumb button (either terminal);
Brown/White is unused. It could enable another button or connect the interrupt pin on the VL53L0X.
As long as the right pins connect to the right pins on the controller you should be able to use whatever colour scheme or cable you like. 

Install the 16 mm button and screw the VL53L0X  (ensure the laser is pointing out of the hole) and secure the cable with the cable clip (invert the clip if you have a lower diameter cable) there should be plenty of space to took any extra cores from cable in without trapping. 


The central controller.
Push the ESP32 into the breakout board, mine was too narrow by a mm or so, but it went in despite that. Solder wires on to the latching button and push into place and install the locking nut if needed. The RJ45 sockets are a loose fit and are only secured when the lid is attached. If you followed my colour instructions above and you followed the type B wiring guide on the socket (orange/white first on the cable) then you can use the following to wire up the control box. Solder leads to the USB C Breakout board - I used Orange for VCC and Brown for GND.

Your ESP32 may have pins in different positions so check before powering up:

3 x Orange from  both VL53L0X VIN pins and USB-C VCC to 5V on ESP32 Breakout;
3 x Brown from  both VL53L0X GND pins and USB-C GND to a GND on ESP32 Breakout;
2 x Blue VL53L0X SCL lines should connect to SCL pin 22 on ESP32 Breakout;
2 x Blue/White VL53L0X SDA lines should connect to SDA pin 21 on ESP32 Breakout;
2 x Orange/White VL53L0X SHUT lines should connect to pins 16 and 17 on ESP32 Breakout;
2 x Green from the thumb buttons on the handheld controllers can connect to any GND on ESP32 Breakout;
2 x Green/White from the thumb buttons on the handheld controllers can connect to pins 19 and 23 on ESP32 Breakout;
Make sure you connect the right button to the right  pin, so connections to pins 23 and 17 should come from the same handheld controller. If you get this wrong you'll find the thumb button is on the wrong handheld controller;
The momentary switch on the central controller should connect to GND and P18 on ESP32 Breakout.
Don't put the lid on as you have to program the board first.


The code.
I used Arduino IDE to program my board, instructions for that process are widely available. If this is your first time using and ESP32, I recommend you start with a simpler/better documented project! I hope to improve this code as my knowledge and ability improves!

The program does a few key things. It creates the Bluetooth connection and generates a sine wave. Measurements are taken from each handheld controller to adjust the frequency and amplitude of the wave. I run this process as a separate task on the ESP32's other core to try and speed up processing - this also mitigates my basic programming abilities to some degree.

The program also monitors the status of the mute button and the ‘notes mode’ button as well as checking if the momentary switch has been activated for programming mode.

One of my proudest moments in this project:

frequencyTarget = round(frequencyMin * pow(2,((controller1Range - controller1Min) / (controller1Max-controller1Min) * log2(frequencyMax/frequencyMin))));
That was when I realised that musical notes are not linear (I have no musical theory knowledge - Ha! I do now)! I was getting all my tones stacked up towards the top end of my movement. It took a long time and a lot of paper to come up with the formula above to change this logarithmic scale into a linear one.

The full code is at the very end of this article.

Limitations and Improvements.
Well the sound production is a little laggy or maybe noisy, is this because of Bluetooth or my poor code? I don't know but I have gone to my limit without investing more time in educating myself better. The sound in the video is much worse than in reality as I had no reliable way to record the output.
I wanted to use the longer range VL53L1X modules - good for up to 4 m. However I could not get these to work reliably and in the end decided to abandon the idea and aim to complete the project. The VL53L0X is good to 2 m although I think about 90 cm is more realistic for playing the instrument. That is good for playing against the floor or a desk whilst standing or sitting.
I don't think it sounds much like a theremin, which is a shame as they are great. Still it is fun to play around with and I think with the right level of thought and attention this idea could be a fun instrument. It's more of a weird sound generator at the moment. Mind you if I played violin of any other musical instrument it would probably sound like a weird sound generator too!!
Some status indicator LED's would be great to show when you are in programming mode or when the device is connecting to Bluetooth.
Time, I need more time to make this really great, but I'm really happy I got to some sort of end to the project in the 60 days allowed.
The VL53L0X TOF sensor is a great little device and I'm thinking of loads of applications already.
I'm making everything fully open source so that anyone can take my idea forwards - keep in touch and share your tunes!
If you decide to print and build this, I hope you have fun and maybe improve on my design. 
