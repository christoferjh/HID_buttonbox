/**
   Using Arduino Leonardo
*/
/**
  PINS
  0: Encoder Blå
  1: Encoder Blå
  2: Knapp Grön
  3: Knapp Grön
  4: Knapp Grön
  5: Knapp Grön
  6: Knapp Grön
  7: Disp Orange
  8: Disp Grå
  9: TOM
  10: Leds Lila
  11: Leds Blå
  12: Leds Grå
  13: Disp Gul

  A5: POT Gul
  A4: POT Gul
  A3: POT Gul
  A2: Missil Grön
  A1: Spak Brun
  A0: Spak Vit

  GND: Spak Svart
  5V: Spak Röd

*/
//Used libs
#include <Joystick.h>
#include <DigitalTube.h>
#include "buttonengine.h"

//Define constants
//Rotary Encoder
#define encoder0PinA  0
#define encoder0PinB  1

volatile unsigned int encoder0Pos = 0;
int unsigned lastEncoder0Pos = 0;

//Leds
//Pin connected to latch pin (ST_CP) of 74HC595, LILA
#define latchPin  10
//Pin connected to clock pin (SH_CP) of 74HC595, GRÅ
#define clockPin  12
//Pin connected to Data in (DS) of 74HC595, BLÅ
#define dataPin  11

//active leds
int ledsNumber = -1;

//Display
#define dispsclk  13
#define disprclk  7
#define dispdio  8

//State for display
int lastDispPos = 0;
int dispPos = 0;

//Define the button engine
ButtonEngine buttonEngine;



//Create the Joystick
Joystick_ Joystick;

#define metabuttonid 0

ButtonEngineSettings settings(&Joystick);

//GUL,ORANGE,GRÅ,int sclk, int rclk, int dio
DigitalTube dis(dispsclk, disprclk, dispdio);

int axisXButtonId;
int axisYButtonId;

void setup() {
  // Initialize Joystick Library
  Joystick.begin();

  Serial.begin(115200);
  Serial.println("hello");

  dis.begin();

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  /**
    This method for checking the encoder is not the best.
    I will change this and use a timer interrupt where i poll the pins instead.
  */
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, CHANGE);  

  buttonEngine.addButton(new Button(2, 0));
  buttonEngine.addButton(new Button(3, 1));
  buttonEngine.addButton(new Button(4, 2));
  buttonEngine.addButton(new Button(5, 3));
  buttonEngine.addButton(new Button(6, 4));
  buttonEngine.addButton((new Button(A2, 5, 6))->nopullup());

//  buttonEngine.addButton((
//                           new PotButton(A1, 6))->addLimit(342)->addLimit(617));
//  buttonEngine.addButton((
//                           new PotButton(A1, 7))->addLimit(527)->addLimit(617));
//
//  buttonEngine.addButton((
//                           new PotButton(A0, 8))->addLimit(341)->addLimit(427)->addLimit(617)->addLimit(649));
//  buttonEngine.addButton((
//                           new PotButton(A0, 9))->addLimit(211)->addLimit(427)->addLimit(573)->addLimit(649));
//  buttonEngine.addButton((
//                           new PotButton(A0, 10))->addLimit(526)->addLimit(573)->addLimit(617)->addLimit(649));
//
//  buttonEngine.addButton((
//                           new PotButton(A5, 11))->addLimit(21));
//  buttonEngine.addButton((
//                           new PotButton(A5, 12))->addLimit(398));
//  buttonEngine.addButton((
//                           new PotButton(A5, 13))->addLimit(627));
//  buttonEngine.addButton((
//                           new PotButton(A5, 14))->addLimit(814));
//  buttonEngine.addButton((
//                           new PotButton(A5, 15))->addLimit(1020));

  buttonEngine.addButton((
                           new PotButton(A1, 7,8))->addLimit(318)->addLimit(607));
  buttonEngine.addButton((
                           new PotButton(A1, 9,10))->addLimit(512)->addLimit(607));

  buttonEngine.addButton((
                           new PotButton(A0, 11,12))->addLimit(318)->addLimit(408)->addLimit(606)->addLimit(639));
  buttonEngine.addButton((
                           new PotButton(A0, 13,14))->addLimit(179)->addLimit(408)->addLimit(560)->addLimit(639));
  buttonEngine.addButton((
                           new PotButton(A0, 15,16))->addLimit(511)->addLimit(560)->addLimit(606)->addLimit(639));

  buttonEngine.addButton((
                           new PotButton(A5, 17))->addLimit(10));
  buttonEngine.addButton((
                           new PotButton(A5, 18))->addLimit(255));
  buttonEngine.addButton((
                           new PotButton(A5, 19))->addLimit(510));
  buttonEngine.addButton((
                           new PotButton(A5, 20))->addLimit(765));
  buttonEngine.addButton((
                           new PotButton(A5, 21))->addLimit(1020));

  axisXButtonId=buttonEngine.addButton((
                           new PotAxisButton(A3,'x')));
   axisYButtonId=buttonEngine.addButton((
                           new PotAxisButton(A4,'y'))->setAsReverse());
  
  buttonEngine.update();

  dis.showRaw(60, 60, 60, 60);

  //dis.showRaw((1<<3), ' ', ' ', '1');
  setLedsIfChanged(255);
}



void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  //Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
}



void setLedsIfChanged(int number) {
  if (number == ledsNumber) {
    return;
  }
  setLeds(number);
  ledsNumber = number;
}

void setLeds(int number) {
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, number);

  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
}

void displayStatusOnLDC(){
    //Show rotation on display
  dispPos = encoder0Pos / 10;
  if (dispPos != lastDispPos) {
    dis.showRaw(((dispPos + 3) % 4) == 0 ? 60 : 73, ((dispPos + 2) % 4) == 0 ? 60 : 73, ((dispPos + 1) % 4) == 0 ? 60 : 73, (dispPos % 4) == 0 ? 60 : 73);
    lastDispPos = dispPos;
  }else if (buttonEngine.getButton(axisXButtonId)->isChanged()){
    int value = buttonEngine.getButton(axisXButtonId)->getState() / 10;
    dis.show(1 + 10, (value / 100) % 10, (value / 10) % 10, value % 10);
  }else if (buttonEngine.getButton(axisYButtonId)->isChanged()){
    int value = buttonEngine.getButton(axisYButtonId)->getState() / 10;
    dis.show(2 + 10, (value / 100) % 10, (value / 10) % 10, value % 10);
  }
  
}

void displayStatus(){
  

  
  //Show on leds
  int switches = (buttonEngine.isButtonOn(6) << 5) | (buttonEngine.isButtonOn(7) << 4) | (buttonEngine.isButtonOn(8) << 3) | (buttonEngine.isButtonOn(9) << 2) | (buttonEngine.isButtonOn(10) << 1);
  int buttons = (buttonEngine.isButtonOn(0) << 5) | (buttonEngine.isButtonOn(1) << 4) | (buttonEngine.isButtonOn(2) << 3) | (buttonEngine.isButtonOn(3) << 2) | (buttonEngine.isButtonOn(4) << 1);
  setLedsIfChanged((switches & ~buttons) | (~switches & buttons) | (1 << 6));
}
void sendEncoderStateClear(){
    Joystick.setButton(27, 0);
    Joystick.setButton(28, 0);  
}
void sendEncoderState(){
    //encoder
  
  if (lastEncoder0Pos == encoder0Pos) {
    Joystick.setButton(27, 0);
    Joystick.setButton(28, 0);
  }else if (lastEncoder0Pos < encoder0Pos) {
    Joystick.setButton(27, 0);
    Joystick.setButton(28, 1);
    }else if (lastEncoder0Pos > encoder0Pos) {
    Joystick.setButton(27, 1);
    Joystick.setButton(28, 0);
    }
  lastEncoder0Pos = encoder0Pos;
}

void loop() {
  //Update buttonstates
  buttonEngine.update();

  if (buttonEngine.isButtonChangedAndOn(metabuttonid) ){
    int mode = settings.setNextMode();
    dis.show('S', ' ', ' ', mode);
    
    Serial.print("mode ");
    Serial.print(mode);
  }
  buttonEngine.setJoystickState(&settings);

  
  
  //Debug print
  buttonEngine.print(false); 
  Serial.print (encoder0Pos, DEC);
  sendEncoderState();

  //Show state on display and leds
  displayStatus();

  Serial.println();
  delay(10);
  sendEncoderStateClear();
}
