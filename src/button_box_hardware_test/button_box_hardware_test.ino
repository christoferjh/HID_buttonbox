/**
  THIS IS THE AD-HOC HARDWARE-TEST VERSION 0.0.1!
  This is problably one of the ugliest pice of code you have ever seen (at least from me).
  Don't judge me...
*/
#include <Joystick.h>
#include <DigitalTube.h>

//Arduino Leonardo
// Create the Joystick
Joystick_ Joystick;

// Constants that maps the physical pin to the joystick button.
const int pinToButtonMap = 2;
const int pinToButtonMap2 = 3;
const int pinToButtonMap3 = 4;
const int pinToButtonMap4 = 5;
const int pinToButtonMap5 = 6;
const int pinToButtonMap6 = A2;
const int potPin = A5;
const int potPin2 = A3;
const int potPin3 = A4;
const int potPin4 = A1;
const int potPin5 = A0;

#define encoder0PinA  0
#define encoder0PinB  1

volatile unsigned int encoder0Pos = 0;
int unsigned lastEncoder0Pos = 0;


//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 10; //LILA
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12; //GRÅ
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11; //BLÅ

//GUL,ORANGE,GRÅ
DigitalTube dis(13, 7, 8);


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

void setup() {
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


  // Initialize Button Pins
  pinMode(pinToButtonMap, INPUT_PULLUP);
  pinMode(pinToButtonMap2, INPUT_PULLUP);
  pinMode(pinToButtonMap3, INPUT_PULLUP);
  pinMode(pinToButtonMap4, INPUT_PULLUP);
  pinMode(pinToButtonMap5, INPUT_PULLUP);
  pinMode(pinToButtonMap6, INPUT);
  pinMode(potPin, INPUT); //Optional

  // Initialize Joystick Library
  Joystick.begin();

  Serial.begin(115200);

  Serial.println();
  Serial.println("hello");

  dis.showRaw(60, 60, 60, 60);
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

/**
   I will change this, will be a array instead.
*/
// Last state of the button/switches/pots
int lastButtonState = 0;
int lastButtonState2 = 0;
int lastButtonState3 = 0;
int lastButtonState4 = 0;
int lastButtonState5 = 0;
int lastButtonState6 = 0;
int lastPotPin = 0;
int lastPotPin2 = 0;
int lastPotPin2Disp = 0;
int lastPotPin3 = 0;
int lastPotPin3Disp = 0;
int lastSelState = -1;
int selState = 0;

int numberToDisplay = 0;
int ledsNumber = -1;
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
int lastDispPos = 0;
int dispPos = 0;
void loop() {

  /**
     Here comes the long list of if-checks...
     Will instead loop the configuration array and save the button states.
     But hey, this also works...
  */
  // Read pin values
  int currentButtonState = !digitalRead(pinToButtonMap);
  if (currentButtonState != lastButtonState)
  {
    Joystick.setButton(0, currentButtonState);
    lastButtonState = currentButtonState;
  }

  // Read pin values
  currentButtonState = !digitalRead(pinToButtonMap2);
  if (currentButtonState != lastButtonState2)
  {
    //if (currentButtonState2){
    //Joystick.pressButton(1);
    Joystick.setButton(1, currentButtonState);
    //}
    lastButtonState2 = currentButtonState;
  }

  // Read pin values
  currentButtonState = !digitalRead(pinToButtonMap3);
  if (currentButtonState != lastButtonState3)
  {
    //if (currentButtonState2){
    //Joystick.pressButton(1);
    Joystick.setButton(2, currentButtonState);
    //}
    lastButtonState3 = currentButtonState;
  }

  // Read pin values
  currentButtonState = !digitalRead(pinToButtonMap4);
  if (currentButtonState != lastButtonState4)
  {
    //if (currentButtonState2){
    //Joystick.pressButton(1);
    Joystick.setButton(3, currentButtonState);
    //}
    lastButtonState4 = currentButtonState;
  }

  // Read pin values
  currentButtonState = !digitalRead(pinToButtonMap5);
  if (currentButtonState != lastButtonState5)
  {
    //if (currentButtonState2){
    //Joystick.pressButton(1);
    Joystick.setButton(4, currentButtonState);
    //}
    lastButtonState5 = currentButtonState;
  }

  // Read pin values
  currentButtonState = digitalRead(pinToButtonMap6);
  if (currentButtonState != lastButtonState6)
  {
    //if (currentButtonState2){
    //Joystick.pressButton(1);
    Joystick.setButton(13, currentButtonState);
    //}
    lastButtonState6 = currentButtonState;
  }


  int currentPotPin = analogRead(potPin);
  Serial.print ("s:");
  Serial.print (currentPotPin);
  Serial.print (' ');
  if (currentPotPin != lastPotPin)
  {
    //Joystick.setButton(2, currentPotPin);
    //Joystick.setThrottle(currentPotPin);
    lastPotPin = currentPotPin;
  }
  //1
  //256
  //510
  //768
  //1023
  if (currentPotPin < 120) {
    Joystick.setButton(16, 1);
    Joystick.setButton(17, 0);
    Joystick.setButton(18, 0);
    Joystick.setButton(19, 0);
    Joystick.setButton(20, 0);
    selState = 1;
    //dis.show('S', '-', '0', '1');
  } else if (currentPotPin < 400) {
    Joystick.setButton(16, 0);
    Joystick.setButton(17, 1);
    Joystick.setButton(18, 0);
    Joystick.setButton(19, 0);
    Joystick.setButton(20, 0);
    selState = 2;
    //dis.show('S', '-', '0', '2');
  } else if (currentPotPin < 650) {
    Joystick.setButton(16, 0);
    Joystick.setButton(17, 0);
    Joystick.setButton(18, 1);
    Joystick.setButton(19, 0);
    Joystick.setButton(20, 0);
    selState = 3;
    //dis.show('S', '-', '0', '3');
  } else if (currentPotPin < 900) {
    Joystick.setButton(16, 0);
    Joystick.setButton(17, 0);
    Joystick.setButton(18, 0);
    Joystick.setButton(19, 1);
    Joystick.setButton(20, 0);
    selState = 4;
    //dis.show('S', '-', '0', '4');
  } else {
    Joystick.setButton(16, 0);
    Joystick.setButton(17, 0);
    Joystick.setButton(18, 0);
    Joystick.setButton(19, 0);
    Joystick.setButton(20, 1);
    selState = 5;
    //dis.show('S', '-', '0', '5');
  }
  if (lastSelState != selState) {
    dis.show(3 + 10, ' ', ' ', selState);
    lastSelState = selState;
  }


  currentPotPin = analogRead(potPin2);
  Serial.print ("x:");
  Serial.print (currentPotPin);
  Serial.print (' ');
  //Serial.print ("x:");
  //Serial.print (currentPotPin/10);
  if (currentPotPin != lastPotPin2)
  {
    Joystick.setXAxis(currentPotPin);
    //dis.show('L', '-', '0', selState);

    if (currentPotPin / 10 != lastPotPin2Disp) {
      lastPotPin2Disp = currentPotPin / 10;

      //dis.print(lastPotPin2Disp);
      dis.show(1 + 10, (lastPotPin2Disp / 100) % 10, (lastPotPin2Disp / 10) % 10, lastPotPin2Disp % 10);
    }


    lastPotPin2 = currentPotPin;
  }

  currentPotPin = 1023 - analogRead(potPin3);
  Serial.print ("y:");
  Serial.print (currentPotPin);
  Serial.print (' ');
  if (currentPotPin != lastPotPin3)
  {
    Joystick.setYAxis(currentPotPin);

    if (currentPotPin / 10 != lastPotPin3Disp) {
      lastPotPin3Disp = currentPotPin / 10;

      //dis.print(lastPotPin3Disp);
      dis.show(2 + 10, (lastPotPin3Disp / 100) % 10, (lastPotPin3Disp / 10) % 10, lastPotPin3Disp % 10);
    }
    lastPotPin3 = currentPotPin;
  }

  currentPotPin = analogRead(potPin4);
  Serial.print (currentPotPin);
  Serial.print (' ');
  //This is the binary table for the switches postion and the read analog value.
  //0 0 - 0
  //1 0 - 319
  //0 1 - 512
  //1 1 - 607
  if (currentPotPin < 300) {
    Joystick.setButton(8, 0);
    Joystick.setButton(9, 0);
  } else if (currentPotPin < 450) {
    Joystick.setButton(8, 1);
    Joystick.setButton(9, 0);
  } else if (currentPotPin < 560) {
    Joystick.setButton(8, 0);
    Joystick.setButton(9, 1);
  } else {
    Joystick.setButton(8, 1);
    Joystick.setButton(9, 1);
  }

  currentPotPin = analogRead(potPin5);
  Serial.print (currentPotPin);
  Serial.print (' ');

  /**
    This is the binary table for the switches postion and the read analog value.
    0   0   0 - 0
    0   1   0 - 180
    1   0   0 - 318
    1   1   0 - 409
    0   0   1 - 511
    0   1   1 - 561
    1   0   1 - 606
    1   1   1 - 639

  */
  if (currentPotPin < 150) {
    Joystick.setButton(10, 0);
    Joystick.setButton(11, 0);
    Joystick.setButton(12, 0);
  } else if (currentPotPin < 300) {
    Joystick.setButton(10, 0);
    Joystick.setButton(11, 1);
    Joystick.setButton(12, 0);
  } else if (currentPotPin < 380) {
    Joystick.setButton(10, 1);
    Joystick.setButton(11, 0);
    Joystick.setButton(12, 0);
  } else if (currentPotPin < 460) {
    Joystick.setButton(10, 1);
    Joystick.setButton(11, 1);
    Joystick.setButton(12, 0);
  } else if (currentPotPin < 535) {
    Joystick.setButton(10, 0);
    Joystick.setButton(11, 0);
    Joystick.setButton(12, 1);
  } else if (currentPotPin < 580) {
    Joystick.setButton(10, 0);
    Joystick.setButton(11, 1);
    Joystick.setButton(12, 1);
  } else if (currentPotPin < 620) {
    Joystick.setButton(10, 1);
    Joystick.setButton(11, 0);
    Joystick.setButton(12, 1);
  } else {
    Joystick.setButton(10, 1);
    Joystick.setButton(11, 1);
    Joystick.setButton(12, 1);
  }


  if (lastEncoder0Pos == encoder0Pos) {
    Joystick.setButton(6, 0);
    Joystick.setButton(7, 0);
  } else if (lastEncoder0Pos < encoder0Pos) {
    lastEncoder0Pos = encoder0Pos;
    Joystick.setButton(6, 0);
    Joystick.setButton(7, 1);
    // o = 60
    // . = 126
    // space =
    //dis.showRaw(((lastEncoder0Pos/10+3)%4)==0?60:73, ((lastEncoder0Pos/10+2)%4)==0?60:73, ((lastEncoder0Pos/10+1)%4)==0?60:73, (lastEncoder0Pos/10%4)==0?60:73);
  } else if (lastEncoder0Pos > encoder0Pos) {
    lastEncoder0Pos = encoder0Pos;
    Joystick.setButton(6, 1);
    Joystick.setButton(7, 0);
    //dis.showRaw(((lastEncoder0Pos/10+3)%4)==0?60:73, ((lastEncoder0Pos/10+2)%4)==0?60:73, ((lastEncoder0Pos/10+1)%4)==0?60:73, (lastEncoder0Pos/10%4)==0?60:73);
  }

  dispPos = lastEncoder0Pos / 10;
  if (dispPos != lastDispPos) {
    dis.showRaw(((dispPos + 3) % 4) == 0 ? 60 : 73, ((dispPos + 2) % 4) == 0 ? 60 : 73, ((dispPos + 1) % 4) == 0 ? 60 : 73, (dispPos % 4) == 0 ? 60 : 73);
    lastDispPos = dispPos;
  }

  Serial.println (encoder0Pos, DEC);
  int switches = (Joystick.getButton(8) << 5) | (Joystick.getButton(9) << 4) | (Joystick.getButton(10) << 3) | (Joystick.getButton(11) << 2) | (Joystick.getButton(12) << 1);
  int buttons = (Joystick.getButton(0) << 5) | (Joystick.getButton(1) << 4) | (Joystick.getButton(2) << 3) | (Joystick.getButton(3) << 2) | (Joystick.getButton(4) << 1);
  setLedsIfChanged((switches & ~buttons) | (~switches & buttons) | (1 << 6));
  delay(50);
  //Auto release the encoders buttons.
  Joystick.setButton(6, 0);
  Joystick.setButton(7, 0);
}
