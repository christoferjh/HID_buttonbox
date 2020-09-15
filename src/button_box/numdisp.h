#ifndef numdisp_h
#define numdisp_h
#include "TimerThree.h"
class NumDisp{
  private:
    int pin_sclk;
    int pin_rclk;
    int pin_dio;
    bool raw = false;
    int NextDigi = 0;
    /**
     *  ___        7
     * |   |     2   6
     *  ---        1
     * |   |     3   5
     *  ---  .     4    0
     * 
     */
    char disp[8], characters[74] = {
    0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09, // 0  -  9
    0x02, 0x9E, 0x24, 0x0C, 0x98, 0x48, 0x40, 0x1E, 0x00, 0x08, // 0. -  9.
    0x11, 0x01, 0x63, 0x03, 0x61, 0x71, 0x43, 0x91, 0xF3, 0x87, 0x91, 0xE3, 0x55, 0x13, 0x03, 0x31, 0x19, 0x73, 0x49, 0x1F, 0x81, 0x81, 0xA9, 0x91, 0x89, 0x25, // A - Z
    0x05, 0xC1, 0xE5, 0x85, 0x21, 0x71, 0x09, 0xD1, 0xF7, 0x8F, 0x51, 0xE3, 0x55, 0xD5, 0xC5, 0x31, 0x19, 0xF5, 0x49, 0xE1, 0xC7, 0xC7, 0xA9, 0x91, 0x89, 0x25, // a-z
    0xFD, 0xFF // - and [space]
  };
  public:
  NumDisp(int sclk, int rclk, int dio){
      this->pin_sclk = sclk;
      this->pin_rclk = rclk;
      this->pin_dio = dio;
    }
  void begin(){
    pinMode(this->pin_sclk, OUTPUT);
    pinMode(this->pin_rclk, OUTPUT);
    pinMode(this->pin_dio, OUTPUT);
  
    digitalWrite(this->pin_rclk, HIGH);

    
  }
  void setIsRaw(bool raw){
    this->raw = raw;
    }
  void send(){
    digitalWrite(this->pin_rclk, LOW);
    shiftOut(this->pin_dio, this->pin_sclk, LSBFIRST, (this->raw?this->disp[this->NextDigi]:this->characters[this->disp[this->NextDigi]]));
    shiftOut(this->pin_dio, this->pin_sclk, LSBFIRST, 0x80>>this->NextDigi);
    digitalWrite(this->pin_rclk, HIGH);
    this->NextDigi = (this->NextDigi >= 3 ? 0 : this->NextDigi+1);
  }
  void showRaw(char digiOne, char digiTwo, char digiTree, char digiFour){
    //disp.show(~(1<<i), 0xFF, 0xFF, 0); 
    this->disp[0] = digiFour;
  this->disp[1] = digiTree;
  this->disp[2] = digiTwo;
  this->disp[3] = digiOne;
  this->setIsRaw(true);
    }
    void showH(char digiOne, char digiTwo, char digiTree, char digiFour){
    //disp.show(~(1<<i), 0xFF, 0xFF, 0); 
      this->disp[0] = digiFour;
      this->disp[1] = digiTree;
      this->disp[2] = digiTwo;
      this->disp[3] = digiOne;
      this->setIsRaw(false);
    }

   void showH3(char digiOne, char digiTwo, char digiTree, char digiFour){
    //disp.show(~(1<<i), 0xFF, 0xFF, 0); 
      this->show(digiOne, digiTwo, digiTree, digiFour);
      this->disp[0] = digiFour;
      this->disp[1] = digiTree;
      //this->disp[2] = digiTwo;
      //this->disp[3] = digiOne;
      this->setIsRaw(false);
    }
  void show(char digiOne, char digiTwo, char digiTree, char digiFour) {
    this->disp[0] = digiFour;
  this->disp[1] = digiTree;
  this->disp[2] = digiTwo;
  this->disp[3] = digiOne;

  for (int i=0;i<8;i++) {
    char val = this->disp[i];
    if((val >= 32)&&(val <= 47)) val = (val == 45) ? 72 : 73;
    else if((val >= '0')&&(val <= '9')) val -= 48;
    else if((val >= 'A')&&(val <= 'Z')) val -= 45;
    else if((val >= 'a')&&(val <= 'z')) val -= 51;
    
    this->disp[i] = val;
  }
  this->setIsRaw(false);
}


void print(int number) {
  show((number/1000)%10, (number/100)%10, (number/10)%10, number%10);
}
  
};




#endif
