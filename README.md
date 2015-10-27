# Arduino
Sources of my shared arduino projects currently maintained on [codebender](http://codebender.cc/search/find/?query=tim.sinaeve), where you can easily upload sketches directly to your Arduino board without the need to struggle through library dependencies.

##RC PWM To LEGO PowerFunctions
The [RC-PWM-To-LEGO-PowerFunctions.ino](http://github.com/beNative/arduino/blob/master/RC-PWM-To-LEGO-PowerFunctions/RC-PWM-To-LEGO-PowerFunctions.ino) sketch converts a PWM signal from a typical RC receiver to the IR protocol used by LEGO Power Functions (which is open source BTW). It supports fully proportional control on two channels.  
**Requirements:**  
* 3 IR leds (I connect them in series and connect them to a digital output on the Arduino).
* Arduino compatible board (I used the Pro Mini but any Arduino compatible board can be used.
* RC receiver (The most types -if not any- can be used here. I use a Spektrum receiver).
* Battery (I use a simple rechargeable 9V battery for this connected to the RAW input on the Pro Mini). 

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
- AD9850 DDS 125MHz synthesizer board.
- 16x2 LCD display with I2C driver.
- Arduino Pro Mini compatible board.
- Rotary encoder.
- A fancy enclosure.
- Battery.
- 
![AD9850 Frequency Generator](http://github.com/beNative/g)
