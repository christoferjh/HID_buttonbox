#ifndef ButtonEngine_h
#define ButtonEngine_h

#define maxnrofbuttons 25
#define maxnroflimits 4

#define timeformomentpress 5

class ButtonEngineSettings {
  public:
     bool switchIsMoment=true;
     bool switchIsDouble=true;
     int mode = 3;
    Joystick_ *Joystick;
    ButtonEngineSettings(Joystick_ *Joystick){
      this->Joystick=Joystick;
      mode = 0;
    }
    
    virtual int setNextMode(){
        mode++;
        if (mode>((1<<1)|(1<<0))){
          mode = 0;
        }
        switchIsDouble = ((1<<0) & mode)>0;
        switchIsMoment = ((1<<1) & mode)>0;
        return mode;
    }
    virtual bool getSwitchIsDouble(){
     return switchIsDouble; 
    }
    virtual bool getSwitchIsMoment(){
     return switchIsMoment; 
    }
    virtual Joystick_* getJoystick(){
      return Joystick;
    }
};

class Button {
  protected:
    byte pin;
    byte joybutton;
    byte joybutton2 = 0;
    int state;
    bool changed=true;
    int sinceChange=0;
    bool reverse=false;
    void (*callback)(char) = 0;
    char selectorChar;
  public:
    Button(byte pin, byte joybutton) {
      this->pin = pin;
      this->joybutton = joybutton;
      init();
    }
    Button(byte pin, byte joybutton, byte joybutton2) {
      this->pin = pin;
      this->joybutton = joybutton;
      this->joybutton2 = joybutton2;
      init();
    }
    Button(byte pin) {
      this->pin = pin;
      init();
    }
    Button() {
     
    }
    virtual void applyMeta(ButtonEngineSettings *settings,char key) {
      
    }
    virtual void restart() {
      state=-1;
    }
    virtual void init() {
      pinMode(pin, INPUT_PULLUP);
      update();
    }
    bool isChanged(){
      return changed;
      }
    Button* nopullup(){
      pinMode(pin, INPUT);
      reverse=true;
      update();
      return this;
    }
    bool isTraceOn(){
      return false;
      //return this->isMomentButton();
    }
    /**
    template <typename F>
    Button* setCallback(F&& lambda){
      
      return this;
    }
    **/
    Button* setSelectorCharCallback(char c, void (*foo)(char)){
      this->callback = foo;
      this->selectorChar = c;
      return this;
      }
    virtual void cleanJoystickState(ButtonEngineSettings *settings){
    }
    virtual void setJoystickState(ButtonEngineSettings *settings){
      Joystick_ *Joystick = settings->getJoystick();
      
      if (isTraceOn()){Serial.print(sinceChange);Serial.print(" setJoystickState");}
      
      if (settings->getSwitchIsMoment() && this->isMomentButton() && !this->isChanged()){

        if (sinceChange<timeformomentpress){
          sinceChange++;
        }else {
          Joystick->setButton(this->getJoybutton(), 0);
          if (this->isDoubleSwitch()){
            Joystick->setButton(this->getJoybutton2(), 0);
          }
        }
        return;
      }


      if (settings->getSwitchIsDouble() && this->isDoubleSwitch()){
       // if (this->isChanged()){ //only send changes
            //if (this->isPressed()){
             // Joystick->setButton(this->getJoybutton(), this->isPressed());
              //Joystick->setButton(this->getJoybutton2(), 0); //reset elsewhere
            //}else{
              //Joystick->setButton(this->getJoybutton(), 0);
             // Joystick->setButton(this->getJoybutton2(), this->isPressed());  
            //}
       // }
         if (this->isPressed()){
            Joystick->setButton(this->getJoybutton(), 1);
            Joystick->setButton(this->getJoybutton2(), 0);
         }else{
            Joystick->setButton(this->getJoybutton(), 0);
            Joystick->setButton(this->getJoybutton2(), 1);
         }
      }else{
        
        Joystick->setButton(this->getJoybutton(), this->isPressed());
      }
    }
    virtual void print(bool verbose){
      Serial.print(this->isPressed()); 
    }
    virtual bool isStateSame(int a,int b){
        return a == b;
      }
    void setNewState(int newstate){
      if (isStateSame(newstate,state)){
        changed=false;
      }else{
        changed=true;
        sinceChange = 0;
       }
      state = newstate;
    }
    virtual void update() {
      // You can handle the debounce of the button directly
      // in the class, so you don't have to think about it
      // elsewhere in your code
      byte newReading = digitalRead(pin);
    
      int newstate = (reverse?!newReading:newReading);
      setNewState(newstate);

      runCallback();
    }
    virtual void runCallback(){
    //run callback if changed and pressed
      if (callback != 0 && isChanged() && isPressed()){
        callback( selectorChar );
      }  
    }
    virtual bool isMomentButton(){
      //return false;
      return this->isDoubleSwitch();  
    }
    virtual bool isDoubleSwitch(){
      return joybutton2>0;  
    }
    byte getJoybutton() {
      
      return joybutton;
    }
    byte getJoybutton2() {
      
      return joybutton2;
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
    virtual bool isStateSame(int a,int b){
        return abs(a - b)<5;
      }
    void setJoystickState(ButtonEngineSettings *settings){
      Joystick_ *Joystick = settings->getJoystick();
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
      //this->state = 
      setNewState(reverse?1023-newReading:newReading);
      runCallback();
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
      pinMode(pin, INPUT);
    }
    PotButton(byte pin, byte joybutton, byte joybutton2) : Button(pin, joybutton,joybutton2) {
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
      setNewState(newstate);
      runCallback();
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

#define ROT_NONE 0
#define ROT_CW 1
#define ROT_CCW -1

class RotEncButton: public Button {
  private:
    int unsigned lastEncoderPos = 0;
    volatile unsigned int *encoderPos;
    char encStatesCCW[maxnrofbuttons];
    char encStatesCW[maxnrofbuttons];
    char encStateKey[maxnrofbuttons];
    int nrencStates = 0;
  public:
  RotEncButton(volatile unsigned int *encoderPos) : Button() {
      //this->joybutton = joybutton;
      //this->joybutton2 = joybutton2;
      this->encoderPos = encoderPos;
  }

    RotEncButton * addEncState(char c,char joybuttonCCW,char joybuttonCW){
      this->encStateKey[nrencStates] = c;
      this->encStatesCCW[nrencStates] = joybuttonCCW;
      this->encStatesCW[nrencStates] = joybuttonCW;
      this->nrencStates++;
      return this;
    }
   void cleanJoystickState(ButtonEngineSettings *settings){
    Joystick_ *Joystick = settings->getJoystick();
    Joystick->setButton(this->getJoybutton(), 0);
    Joystick->setButton(this->getJoybutton2(), 0);
    }
  void setJoystickState(ButtonEngineSettings *settings){
      Joystick_ *Joystick = settings->getJoystick();

      if (this->state == ROT_NONE){
            Joystick->setButton(this->getJoybutton(), 0);
            Joystick->setButton(this->getJoybutton2(), 0);
        }else if (this->state == ROT_CCW){
          Joystick->setButton(this->getJoybutton(), 0);
           Joystick->setButton(this->getJoybutton2(), 1);
        }else if (this->state == ROT_CW){
          Joystick->setButton(this->getJoybutton(), 1);
           Joystick->setButton(this->getJoybutton2(), 0);
        }

      
    }
    virtual void applyMeta(ButtonEngineSettings *settings,char key) {
      
      for (int i = 0 ; i < nrencStates; i++) {
        if (encStateKey[i]==key){
          Joystick_ *Joystick = settings->getJoystick();
          Joystick->setButton(this->getJoybutton(), 0);
          Joystick->setButton(this->getJoybutton2(), 0);
          this->joybutton = encStatesCCW[i];
          this->joybutton2 = encStatesCW[i];
          
          break;
        }
       }
    }
    void update() {
      //clear 
      
      setRotation(lastEncoderPos - *encoderPos);
      
      lastEncoderPos = *encoderPos;
      //setNewState(newstate);
      runCallback();
    }
  void setRotation(char direction){
    if (direction == 0) {
      setNewState(ROT_NONE);
    }else if (direction < 0) {
      setNewState(ROT_CCW);
    }else if (direction > 0) {
      setNewState(ROT_CW);
    }
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
    int addButton(Button *b) {
      if (nr_buttons >= maxnrofbuttons) {
        return -1;
      }
      this->buttons[nr_buttons] = b;
      this->nr_buttons++;
      return nr_buttons-1;
    }
    void update() {
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        b->update();
      }
    }
  void cleanJoystickState(ButtonEngineSettings *settings ){
      //Joystick_ *Joystick = settings->getJoystick();
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        b->cleanJoystickState(settings);
        //Joystick->setButton(b->getJoybutton(), b->isPressed());
      }
    }
    
    void setJoystickState(ButtonEngineSettings *settings ){
      //Joystick_ *Joystick = settings->getJoystick();
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
        b->setJoystickState(settings);
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

    void restart() {
      for (int i = 0 ; i < nr_buttons; i++) {
        Button *b = buttons[i];
         
        b->restart();
      }
    }
    bool isButtonChangedAndOn(int nr){
      if (nr > nr_buttons) {
        //return null;
      }
      return buttons[nr]->isChanged() && buttons[nr]->isPressed();
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
