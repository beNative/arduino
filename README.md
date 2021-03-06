# Arduino
Sources of my shared arduino projects originally maintained on [codebender](http://codebender.cc/search/find/?query=tim.sinaeve), where you could easily upload sketches directly to your Arduino board without the need to struggle through library dependencies. Unfortunately codebender is closing down and as I result I will move my projects to here.

##RC PWM To LEGO PowerFunctions
The [RC-PWM-To-LEGO-PowerFunctions.ino](http://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/RC-PWM-To-LEGO-PowerFunctions.ino) sketch converts a PWM signal from a typical RC receiver to the IR protocol used by LEGO Power Functions (which is open source BTW). It supports **fully proportional control on all channels**.  
See the pictures below with the system installed on the Lego Crawler 9398 (2 channels) and the Lego 42030 - Volvo L350F Front Loader (4 channels).

**Requirements:**  
* 3 IR leds (I connect them in series and connect them to a digital output on the Arduino).
* Arduino compatible board (I used the Pro Mini but any Arduino compatible board can be used).
* RC receiver (The most types -if not any- can be used here. I used a couple of Spektrum compatible 2.4GHz receivers).
* Battery (I use a simple rechargeable 9V battery for this connected to the RAW input on the Pro Mini). 

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Power%20functions%20Receiver.JPG)

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Battery%20Box%20-%20Inside.JPG)

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Battery%20Box.JPG)

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Lego%20Crawler%209398%20IR-installed.png)

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Lego%20Crawler%209398.png)

![PWM to LPF](https://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/Lego%2042030.png)

##JoyStickShield To LEGO PowerFunctions
[JoyStickShield-To-LEGO-PowerFunctions.ino](http://github.com/beNative/arduino/blob/master/JoyStickShield-To-LEGO-PowerFunctions/JoyStickShield-To-LEGO-PowerFunctions.ino) uses the popular Joystick Arduino Shield in combination with some inexpensive NRF40 transceivers to make a better wireless control than the default IR way. It supports fully proportional control on two channels. 

**Requirements:**
* 1 Arduino UNO compatible board
* 1 Arduino Pro Mini compatible board 
* 2 NRF40 tranceivers
* 3 IR leds

##AD9850 Frequency Generator
The [AD9850-Frequency-Generator.ino](http://github.com/beNative/arduino/blob/master/AD9850-Frequency-Generator/AD9850-Frequency-Generator.ino) sketch interfaces with the AD9850 DDS 125MHz synthesizer board to make a simple yet powerful home made function generator.  
**Requirements:**
* AD9850 DDS 125MHz synthesizer board.
* 16x2 LCD display with I2C driver.
* Arduino Pro Mini compatible board.
* Rotary encoder.
* A fancy enclosure.
* Battery.

![AD9850 Frequency Generator](https://github.com/beNative/arduino/blob/master/AD9850-Frequency-Generator/AD9850-Frequency-Generator.png)
