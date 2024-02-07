This Arduino nano device device is designed to control the RF Remote Control Relay Switch using the DMX protocol and radio signals 433mhz (315mhz). It uses buttons to select a DMX channel and saves its value to the EEPROM memory for later use. It also displays the selected channel on a 4-digit display and sends radio signals depending on the DMX value.

Used libraries:

ShiftDisplay v.3.6.1 (from master) https://github.com/pmiguelpinto90/ShiftDisplay Library for driving 7-segment displays using shift registers (74HC595).

GyverButton v.3.8.0 https://github.com/GyverLibs/GyverButton Library for multi-functional button pressing.

EEPROM

DMXSerial v.1.5.3 https://github.com/mathertel/DMXSerial/ This is a library for sending and receiving DMX codes using the Arduino platform and MAX485 module.

RCSwitch v.2.6.4 https://github.com/sui77/rc-switch Arduino lib to operate 433/315Mhz devices i use 433Mhz RF Transmitter with Receiver Kit. 

I used the receiver from the RF kit and the ReceiverDemo example from the RCSwitch library to capture the signal from the remote control.

The code sequence that is transmitted is written in the firmware. Manual addition of the number of channels and code sequence is planned in future versions.

I coded for arduino for the first time, so there is an interrupt conflict, which is expressed in the display blinking when receiving the correct DMX signal.
