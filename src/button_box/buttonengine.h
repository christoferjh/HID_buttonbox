#ifndef ButtonEngine_h
#define ButtonEngine_h

#define maxnrofbuttons 25
#define maxnroflimits 4

class Button {
  protected:
    byte pin;
    byte joybutton;
    int state;
    bool reverse=false;
  public:
    Button(byte pin, byte joybutton) {
      this->pin = pin;
      this->joybutton = joybutton;
      init();
    }
    Button(byte pin) {
      this->pin = pin;
      init();
    }
    virtual void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
    Button* nopullup(){
      pinMode(pin, INPUT);
      reverse=true;
      update();
      return this;
    }
    virtual void setJoystickState(Joystick_ *Joystick){
     
        
        Joystick->setButton(this->getJoybutton(), this->isPressed());
      
    }
    virtual void print(bool verbose){
      Serial.print(this->isPressed()); 
    }
    virtual void update() {
      // You can handle the debounce of the button directly
      // in the class, so you don't have to think about it
      // elsewhere in your code
      byte newReading = digitalRead(pin);
    
      state = (reverse?!newReading:newReading);


    }
    byte getJoybutton() {
      
      return joybutton;
    }
    int getState() {
      //update();
      return state;
    }
    bool isPressed() {
      return (getState() == LOW);
    }
};
class PotAxisButton: public Button {
  private:
    char axis;
    //int axisstate;
  public:
    PotAxisButton(byte pin, char axis) : Button(pin) {
     this->axis = axis;
     pinMode(pin, INPUT);
      //this->nrlimits = nrlimits;
    }
    virtual void init() {
      pinMode(pin, INPUT);
      update();
    }
    void setJoystickState(Joystick_ *Joystick){
     
        if (this->axis=='x'){
          Joystick->setXAxis(this->state);
        }else if (this->axis=='y'){
          Joystick->setYAxis(this->state);
        }
    }
    
    PotAxisButton * setAsReverse(){
      this->reverse = true;
      return this;  
    }
    virtual void print(bool verbose){
      
      Serial.print(this->state); 
    }
    void update() {
      int newReading = analogRead(pin);
      
      this->state = (reverse?1023-newReading:newReading);
    }
};

class PotButton: public Button {
  private:
    int limits[maxnroflimits];
    int nrlimits = 0;
    int fuzzy = 10;
    int lastRead;
  public:
    PotButton(byte pin, byte joybutton) : Button(pin, joybutton) {
      //this->limits = limits;
      //this->nrlimits = nrlimits;
      pinMode(pin, INPUT);
    }
     virtual void init() {
      pinMode(pin, INPUT);
      update();
    }
    void update() {
      //this->state = 1;
      int newReading = analogRead(pin);
      lastRead = newReading;
      //Serial.print(" pin ");Serial.print(newReading);
      int newstate = HIGH;
      for (int i = 0 ; i < nrlimits; i++) {
        int limit = limits[i];
        if ((newReading <= limit + fuzzy) && (newReading >= limit - fuzzy)) {
          newstate=LOW;
        }
      }
      state = newstate;
    }
    virtual void print(bool verbose){
      if (verbose){
        Serial.print(lastRead); Serial.print(" "); 
      }
      Serial.print(this->isPressed()); 
    }
    PotButton* addLimit(int limit) {

      this->limits[nrlimits] = limit;
      this->nrlimits++;
      return this;
    }
};

/**
   Max 10 buttons
*/
class ButtonEngine {
  private:
    Button *buttons[maxnrofbuttons];
    char nr_buttons = 0;
  public:

    ButtonEngine() {

    }
    bool addButton(Button *b) {
      if (nr_buttons >= maxnrofbuttons) {
        return false;
      }
      this->buttons[nr_buttons] = b;
      this->nr_buttons++;
      return true;
    }
    void update() {
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        b->update();
      }
    }
    void setJoystickState(Joystick_ *Joystick){
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        b->setJoystickState(Joystick);
        //Joystick->setButton(b->getJoybutton(), b->isPressed());
      }
    }
    void print(bool verbose) {
      Serial.print("Buttons ");
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        if (verbose){
          Serial.print(i);Serial.print(" ");
        }
        b->print(verbose); Serial.print(", ");
      }
    }
    bool isButtonOn(int nr){
      if (nr > nr_buttons) {
        //return null;
      }
      return buttons[nr]->isPressed();
      }
    Button* getButton(int nr) {
      if (nr > nr_buttons) {
        //return null;
      }
      return buttons[nr];
    }
};
#endif
