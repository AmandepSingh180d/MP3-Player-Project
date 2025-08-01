#include "Timer.h"
#include "Adafruit_LiquidCrystal.h"
// Depending on the LiquidCrystal library you are able to install, it might be:
// #include "LiquidCrystal.h"
#include "pitches.h"

// Sound Variables  
int buzzer = 8;
int sw = 10;

// == Song 1 ==
int song1[] = {
NOTE_E4, NOTE_C5, NOTE_B1, NOTE_F3, NOTE_C4, 
NOTE_A4, NOTE_A4, NOTE_GS5, NOTE_C5, NOTE_CS4, 
NOTE_AS4, NOTE_C5, NOTE_DS4, NOTE_CS5, NOTE_GS4, 
NOTE_C3, NOTE_E3, NOTE_DS5, NOTE_D4, NOTE_D3
};
int song1_time[] = {
2, 1, 2, 1, 1, 4, 8, 16, 8, 4, 4, 1, 8, 4, 2, 4, 4, 16, 4, 2
};

// == Song 2 ==

int song2[] = {
  NOTE_FS5, NOTE_D2, NOTE_DS5, NOTE_G2, NOTE_B3, 
  NOTE_C2, NOTE_G5, NOTE_D6, NOTE_CS5, NOTE_AS4, 
  NOTE_DS6, NOTE_D3, NOTE_CS4, NOTE_E5, NOTE_DS6,
   NOTE_E4, NOTE_B4, NOTE_F4, NOTE_E6, NOTE_DS4
};

int song2_time[] = {
  2, 2, 4, 8, 1, 8, 4, 4, 16, 8, 2, 4, 16, 8, 2, 4, 16, 4, 8, 1
};

// == Song 3 == 

int song3[] = {
  NOTE_A5, NOTE_D4, NOTE_D6, NOTE_DS3, NOTE_G4, 
  NOTE_B2, NOTE_F2, NOTE_A3, NOTE_AS2, NOTE_B5, 
  NOTE_C6, NOTE_C3, NOTE_GS3, NOTE_G2, NOTE_FS5, 
  NOTE_AS4, NOTE_GS2, NOTE_CS3, NOTE_C3, NOTE_AS2
};

int song3_time[] = {
  1, 2, 16, 4, 16, 2, 16, 1, 1, 2, 1, 8, 2, 16, 8, 1, 16, 4, 1, 2
};


// LCD variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
Adafruit_LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Depending on the LiquidCrystal library you are able to install, it might be:
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// Task Structure Definition
typedef struct task {
   int state;                  // Tasks current state
   unsigned long period;       // Task period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   int (*TickFct)(int);        // Task tick function
} task;


const unsigned char tasksNum = 4;
task tasks[tasksNum]; // We have 4 tasks

// Task Periods

const unsigned long periodLCDOutput = 100;
const unsigned long periodJoystickInput = 100;
const unsigned long periodSoundOutput = 100;
const unsigned long periodController = 500;


// GCD 
const unsigned long tasksPeriodGCD = 100;

// Task Function Definitions
int TickFct_LCDOutput(int state);
int TickFct_JoystickInput(int state);
int TickFct_SoundOutput(int state);
int TickFct_Controller(int state);

// Task Enumeration Definitions
enum LO_States {LO_init, LO_MenuOptionA, LO_MenuOptionB};
enum JI_States {JI_init, JI_Sample};
enum SO_States {SO_init, SO_SoundOn, SO_SoundOff};
enum C_States {C_Init, C_Start, C_1, C_2, C_3, C_4, C_5};



void TimerISR() { // TimerISR actually defined here
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}


void LCDWriteLines(String line1, String line2) {
  lcd.clear();          
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

int readStick() { //returns 1 if the joystick was up, 2 if it is down, 0 for anything else
  // you may have to read from A0 instead of A1 depending on how you orient the joystick
  if(digitalRead(sw) == HIGH){
    return 5;
  }
  else if (analogRead(A1) > 800) {
    return 2; // down
  }
  else if (analogRead(A1) < 200) {
    return 1; // up
  }
  else if (analogRead(A0) < 200) {
    return 3; // right
  }
  else if (analogRead(A0) > 800) {
    return 4; // left
  }
  else {
    return 0;
  }

}
// Task Function Definitions

int menuOption = 0;
int menuOption2 = 0;

// Task 1
int TickFct_LCDOutput(int state) {
  switch (state) { // State Transitions
    case LO_init:
      state = LO_MenuOptionA;
      LCDWriteLines("Song 1  Song 2", "Song 3  Start");
    break;
    case LO_MenuOptionA:
      if(menuOption2 == 0) { //BR
        state = LO_MenuOptionA;
        //lcd.setCursor(6,1);
      }
      else if(menuOption2 == 1) { // SONG 1 selected
        state = LO_MenuOptionA;
        LCDWriteLines("Playing  Song 1", "Pause    Play");
        //lcd.setCursor(5,1);
      }
      else if(menuOption2 == 2) { // SONG 2 selected
        state = LO_MenuOptionA;
        LCDWriteLines("Playing  Song 2", "Pause    Play");
       // lcd.setCursor(6,1);
      }
      else if(menuOption2 == 3) { // SOng 3 selected
        state = LO_MenuOptionA;
        LCDWriteLines("Playing  Song 3", "Pause    Play");
        //lcd.setCursor(6,1);
      }
    break;
    case LO_MenuOptionB:
      if(menuOption == 0) {
        state = LO_MenuOptionA;
      LCDWriteLines("Song 1  Song 2", "Song 3  Start");
      }
    break;
  }

  switch (state) { // State Actions
    case LO_MenuOptionA:
    
    break;
    case LO_MenuOptionB:
   
    break;
  }
  return state;
}
// Task 2
int TickFct_JoystickInput(int state) {
  switch (state) { // State Transitions
    case JI_init:
    state = JI_Sample;
    break;
    case JI_Sample:
      if(readStick() == 0){
        state = JI_Sample;
        menuOption = 0;
      }
      else if(readStick() == 1){ //UP
        state=JI_Sample;
        menuOption = 2;
      }
      else if(readStick() == 2){ //DOwn
          state = JI_Sample;
          menuOption =1;
      }
      else if(readStick() == 3){ //Right
          state = JI_Sample;
          menuOption=3;
      }
      else if(readStick() == 4){ //Left
          state = JI_Sample;
          menuOption=4;
      }
      else if(readStick() == 5){ //Left
          state = JI_Sample;
          menuOption=5;
      }
    break;
  }

   switch (state) { // State Actions
    case JI_Sample:
      
    break;
  }
  return state;
}
// Sound Output
int counter = 0;
int note = 0;
int TickFct_SoundOutput(int state) {
  switch (state) { // State Transitions
    case SO_init:
      //state = SO_SoundOn;
    break;
    case SO_SoundOn:
      if(counter >= song1_time[note]) {
         state = SO_SoundOff;
         note++;
         counter = 0;
         note = note % 20;
      }
    break;
    case SO_SoundOff:
      state = SO_SoundOn;
    break;
    
  }
   switch (state) { // State Actions
    case SO_SoundOn:
      tone(buzzer, song1[note], periodSoundOutput * song1_time[note]);
      counter++;
    break;
    case SO_SoundOff:
      noTone(buzzer);
    break;

  }
  return state;
}

// Task 4 (Unused)
int posX = 7;
int posY = 1;
int TickFct_Controller(int state) {
  switch (state) { // State Transitions
    case C_Init:
      state = C_Start;
    break;

    case C_Start:
      lcd.blink();
      lcd.setCursor(posX, posY);
      if(menuOption == 0){
        state=C_Init;
      }
      else if(menuOption == 1){
        state=C_1;
      }
      else if(menuOption == 2){
        state=C_2;
      }
      else if(menuOption == 3){
        state = C_3;
      }
      else if(menuOption == 4){
        state = C_4;
      }
      else if(menuOption == 5){
        lcd.noBlink();
        lcd.print("*");
        state = C_5;
      }
    break;
    case C_1:
      state=C_Start;
    break;
    case C_2:
      state=C_Start;
    break;
    case C_3:
      state=C_Start;
    break;
    case C_4:
      state=C_Start;
    break;
    case C_5:
      state=C_Start;
    break;
    
  }

   switch (state) { // State Actions
    case C_Init:
    break;
    case C_Start:
    break;
    case C_1: //UP
      if(posY <=0 ){
        posY=0;
      }
      else if(posY > 0){
        posY--;
      }
    break;
    case C_2: //DOWN
      if(posY >= 1){
        posY=1;
      }
      else if(posY < 1){
        posY++;
      }
    break;
    case C_3: //Right
      if(posX >= 7){
        posX=7;
      }
      else if(posX < 7){
        posX++;
      }
    break;
    case C_4: //Right
      if(posX >= 7){
        posX--;
      }
      else if(posX < 7){
        posX=6;
      }
    break;
  }
  return state;
}



void InitializeTasks() {
  unsigned char i=0;
  tasks[i].state = LO_init;
  tasks[i].period = periodLCDOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_LCDOutput;
  ++i;
  tasks[i].state = JI_init;
  tasks[i].period = periodJoystickInput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_JoystickInput;
  ++i;
  tasks[i].state = SO_init;
  tasks[i].period = periodSoundOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_SoundOutput;
  ++i;
  tasks[i].state = C_Init;
  tasks[i].period = periodController;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_Controller;

}

void setup() {
  // put your setup code here, to run once:
  InitializeTasks();

  TimerSet(tasksPeriodGCD);
  TimerOn();
  Serial.begin(9600);
  // Initialize Outputs
  lcd.begin(16, 2);
  // Initialize Inputs

}

void loop() {
  // put your main code here, to run repeatedly:
  // Task Scheduler with Timer.h
  Serial.println(menuOption);
  
}
