# Arduino
Sources of my shared arduino projects currently maintained on codebender

##RC PWM To LEGO PowerFunctions
The `RC-PWM-To-LEGO-PowerFunctions.ino` sketch converts a PWM signal from a typical RC receiver to the IR protocol used by LEGO Power Functions (which is open source BTW). It supports fully proportional control on two channels.
**Requirements:**
* 3 IR leds (I connect them in series and connect them to a digital output on the Arduino).
* Arduino compatible board (I used the Pro Mini but any Arduino compatible board can be used.
* RC receiver (The most types can be used here. I use a Spektrum receiver).
* Battery (I use a simple rechargeable 9V battery for this connected to the RAW input on the Pro Mini) 

##JoyStickShield To LEGO PowerFunctions
`JoyStickShield-To-LEGO-PowerFunctions.ino` uses the popular Joystick Arduino Shield in combination with some inexpensive NRF40 transceivers to make a better wireless control than the default IR way. It supports fully proportional control on two channels.
