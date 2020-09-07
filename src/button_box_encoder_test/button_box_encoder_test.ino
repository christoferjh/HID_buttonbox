#include "EncoderStepCounter.h"
#include "TimerThree.h"

#define ENCODER_PIN1 0
#define ENCODER_INT1 digitalPinToInterrupt(ENCODER_PIN1)
#define ENCODER_PIN2 1
#define ENCODER_INT2 digitalPinToInterrupt(ENCODER_PIN2)

// Create instance for one full step encoder
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2);
// Use the following for half step encoders
//EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2, HALF_STEP);

void setup() {
  Serial.begin(9600);
  // Initialize encoder
  encoder.begin();
  // Initialize interrupts
  //attachInterrupt(ENCODER_INT1, interrupt, CHANGE);
  //attachInterrupt(ENCODER_INT2, interrupt, CHANGE);
  Timer3.initialize(1);
  Timer3.attachInterrupt(interrupt); // blinkLED to run every 0.15 seconds
}

// Call tick on every change interrupt
void interrupt() {
  encoder.tick();
}

void interrupt2() {
  encoder.tick();
}

// This is an example on how to change a "long" variable
// with the library. With every loop the value is added
// and then cleared in the encoder library
signed long position = 0;
void loop() {
  signed char pos = encoder.getPosition();
  if (pos != 0) {
    position += pos;
    encoder.reset();
    Serial.println(position);
  }
  //delay(10);
  encoder.tick();
}
