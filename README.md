# HID_buttonbox
Button box primarily for flight simulation 

## Before you look at the code..
  The one named "button_box_hardware_test": THIS IS THE AD-HOC HARDWARE-TEST VERSION 0.0.1!
  This is problably one of the ugliest pice of code you have ever seen (at least from me).
  Don't judge me...

# The finished button box
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/img_box.PNG)
## How the display and leds work
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/box.gif)
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/buttons.gif)


## And the hidden mess under the lid...
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/img_box_inside.PNG)
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/img_box_messy.PNG)

# Code
The code is just the hardware test verison.
Using the following libs:
[ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary)
[DigitalTube](https://github.com/ioxhop/DigitalTube)


# Really messy schematic, probably correctly wired
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/schem.png)

## BOM
This is almost correct:

| Label | Part Type                | Properties                                                                 |
| ----- | ------------------------ | -------------------------------------------------------------------------- |
| LED1  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| LED2  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| LED3  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| LED4  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| LED5  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| LED6  | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| Part1 | Arduino Leonardo (Rev3b) | type Arduino Leonardo (Rev3)                                               |
| R1    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R3    | 1kΩ Resistor             | pin spacing 400 mil; bands 4; resistance 1kΩ; tolerance ±5%; package THT   |
| R4    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R5    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R6    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R7    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R8    | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| R9    | 1kΩ Resistor             | pin spacing 400 mil; bands 4; resistance 1kΩ; tolerance ±5%; package THT   |
| R10   | 2.2kΩ Resistor           | pin spacing 400 mil; bands 4; resistance 2.2kΩ; tolerance ±5%; package THT |
| R12   | 1kΩ Resistor             | pin spacing 400 mil; bands 4; resistance 1kΩ; tolerance ±5%; package THT   |
| R13   | 1kΩ Resistor             | pin spacing 400 mil; bands 4; resistance 1kΩ; tolerance ±5%; package THT   |
| R14   | 2.2kΩ Resistor           | pin spacing 400 mil; bands 4; resistance 2.2kΩ; tolerance ±5%; package THT |
| R15   | 4.3kΩ Resistor           | pin spacing 400 mil; bands 4; resistance 4.3kΩ; tolerance ±5%; package THT |
| R16   | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| RE1   | Rotary Encoder           | type ALPS STEC12E07; package THT                                           |
| S1    | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| S2    | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| S3    | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| S4    | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| S5    | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| U1    | SWITCH\_SPDT             | package kps-1290                                                           |
| U2    | 74HC595                  | type 74HC595; package DIP16 \[THT\]                                        |
| U3    | POT                      | variant mam; package pot\_mammoth                                          |
| U4    | POT                      | variant mam; package pot\_mammoth                                          |
| U5    | POT                      | variant mam; package pot\_mammoth                                          |
| U6    | SWITCH\_SPDT             | package kps-1290                                                           |
| U7    | SWITCH\_SPDT             | package kps-1290                                                           |
| U8    | SWITCH\_SPDT             | package kps-1290                                                           |
| U9    | SWITCH\_SPDT             | package kps-1290                                                           |
| U10   | SWITCH\_SPDT             | package kps-1290                                                           |
| U11   | 7-SEGMENT-4DIGIT         | segments 7; digits 4; variant variant 1; package 7-segment-4digit-youngsun |

## Shopping list

| Amount | Part Type                | Properties                                                                 |
| ------ | ------------------------ | -------------------------------------------------------------------------- |
| 6      | Red (633nm) LED          | leg yes; color Red (633nm); package 3 mm \[THT\]                           |
| 1      | Arduino Leonardo (Rev3b) | type Arduino Leonardo (Rev3)                                               |
| 7      | 220Ω Resistor            | pin spacing 400 mil; bands 4; resistance 220Ω; tolerance ±5%; package THT  |
| 4      | 1kΩ Resistor             | pin spacing 400 mil; bands 4; resistance 1kΩ; tolerance ±5%; package THT   |
| 2      | 2.2kΩ Resistor           | pin spacing 400 mil; bands 4; resistance 2.2kΩ; tolerance ±5%; package THT |
| 1      | 4.3kΩ Resistor           | pin spacing 400 mil; bands 4; resistance 4.3kΩ; tolerance ±5%; package THT |
| 1      | Rotary Encoder           | type ALPS STEC12E07; package THT                                           |
| 5      | Square Pushbutton        | default state Normally Open; switching circuit SPST                        |
| 6      | SWITCH\_SPDT             | package kps-1290                                                           |
| 1      | 74HC595                  | type 74HC595; package DIP16 \[THT\]                                        |
| 3      | POT                      | variant mam; package pot\_mammoth                                          |
| 1      | 7-SEGMENT-4DIGIT         | segments 7; digits 4; variant variant 1; package 7-segment-4digit-youngsun |

[BOM](https://github.com/christoferjh/HID_buttonbox/blob/master/bom.html)


# Bonus content, easy DIY flight yoke
![schematic](https://github.com/christoferjh/HID_buttonbox/blob/master/images/yoke.PNG)
[On hackaday](https://hackaday.com/2020/10/17/turning-a-desk-drawer-into-a-flight-yoke/)
