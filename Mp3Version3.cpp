//Version 5
//Final Code
//We have achieved a fully functional mp3 player
//1) The delay on the joystick FIXED
//2) The song ending and not going back FIXED
//3) Cleaning the code to be more readable FIXED
//3.1) Cleaning a lot of the not needed variables FIXED
//4) Possibly making some custom FIXED
#include "Timer.h"
#include "Adafruit_LiquidCrystal.h"
// Depending on the LiquidCrystal library you are able to install, it might be:
// #include "LiquidCrystal.h"
#include "pitches.h"

// Variables  
int buzzer = 8; //The sound maker
int sw = 10; //Button
int selectedSong = 0; //Song selection
int userPress=0; //Button pressed
int nextPage = 0; //Are we on the next page?
int menuOption2 = 0; //Switching to next page 
int menuOption3 = 0; //Menu Options for the 2nd page
int pause = 0; //Works for pause and play
int goBack = 0; // In order to reset/ go back to the beginning
// == Song 1 ==
int song1[] = { //39
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0, 0, NOTE_C4, 
  NOTE_C5, NOTE_A3, NOTE_A4, NOTE_AS3, 
  NOTE_AS4, 0, 0, NOTE_F3, NOTE_F4, 
  NOTE_D3, NOTE_D4, NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0, 0, NOTE_DS4, 
  NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_DS4, NOTE_DS4, 0
};
int song1_time[] = {
8,8,8,8,8,8,2,8,8,8,8,8,8,
2,8,8,8,8,8,8,2,8,8,8,8,8,
8,2,6,6,6,6,6,6,6,6,6,6,6
};

// == Song 2 ==

int song2[] = {
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_G4, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_G4,
  NOTE_E4, NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_G4, 
  NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5, NOTE_E5,
  NOTE_C5, NOTE_D5, NOTE_B4
};

int song2_time[] = { //27
  8,8,8,8,8,8,16,16,//
  8,8,4,16,16,8,8,8,8,
  8,8,2,2,8,8,2,2,2,2
};

// == Song 3 == 

int song3[] = {
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5,
  NOTE_G4, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4,
  NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_G4,
  NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5,
  NOTE_G5, NOTE_E5, NOTE_C5, NOTE_D5,
  NOTE_B4, NOTE_C5, NOTE_G4, NOTE_E4,
};

int song3_time[] = {//27
  8, 16, 8, 8, 4, 8,
  8, 4, 8, 8, 5, 8, 8,
  5, 8, 16, 8, 8, 8, 8,
  8, 4, 8, 8, 16, 8, 5

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
enum LO_States {LO_init, LO_MenuOptionA, LO_MenuOptionB, LO_MenuOptionC};
enum JI_States {JI_init, JI_Sample};
enum SO_States {SO_init, SO_SoundOn, SO_SoundOn2, SO_SoundOn3, SO_SoundOff, SO_SoundOff2, SO_SoundOff3,SO_Pause};
enum C_States {C_init, C_T1, C_T2};



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

// Task Function Definitions
int readStick() { //returns 1 if the joystick was up, 2 if it is down, 0 for anything else
  // you may have to read from A0 instead of A1 depending on how you orient the joystick
  if (analogRead(A1) > 800) {
    return 2; // down
  }
  else if (analogRead(A1) < 200) {
    return 1; // up
  }
  else if (analogRead(A0) < 200) {
    return 4; // right
  }
  else if (analogRead(A0) > 800) {
    return 3; // left
  }
  else {
    return 0;
  }

}
int menuOption = 0; // Controlls on where the button is clicked

// Task 1
int TickFct_LCDOutput(int state) { // Used in order to write to the LCD at which option I required
  switch (state) { // State Transitions
    case LO_init:
      state = LO_MenuOptionA;
      LCDWriteLines("Song 1  Song 2", "Song 3  Start");
    break;

    case LO_MenuOptionA:
      if(menuOption == 1 && nextPage == 0) {
        state = LO_MenuOptionA;
        LCDWriteLines("Song 1* Song 2", "Song 3  Start");
      }
      else if(menuOption == 2 && nextPage == 0) {
        state = LO_MenuOptionA;
        LCDWriteLines("Song 1 *Song 2", "Song 3  Start");
      }
      else if(menuOption == 3 && nextPage == 0) {
        state = LO_MenuOptionA;
        LCDWriteLines("Song 1  Song 2", "Song 3* Start");
      }
      else if(nextPage == 1){
        state = LO_MenuOptionC;
      }
      else if(menuOption == 0) {
        state = LO_MenuOptionA;
        LCDWriteLines("Song 1  Song 2", "Song 3  Start");
      }
      else{
        state = LO_MenuOptionA;
      }
    break;

    case LO_MenuOptionC:
      //SONG1
      if(menuOption3 == 0 && selectedSong == 1 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 1", "Pause    Play");
      }
      else if(menuOption3 == 1 && selectedSong == 1 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 1", "Pause  * Play");
      }
      else if(menuOption3 == 2 && selectedSong == 1 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 1", "Pause *  Play");
      }
      //SONG2
      else if(menuOption3 == 0 && selectedSong == 2 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 2", "Pause    Play");
      }
      else if(menuOption3 == 1 && selectedSong == 2 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 2", "Pause  * Play");
      }
      else if(menuOption3 == 2 && selectedSong == 2 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 2", "Pause *  Play");
      }
      //SONG 3
      else if(menuOption3 == 0 && selectedSong == 3 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 3", "Pause    Play");
      }
      else if(menuOption3 == 1 && selectedSong == 3 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 3", "Pause  * Play");
      }
      else if(menuOption3 == 2 && selectedSong == 3 && goBack == 0){
        state = LO_MenuOptionC;
        LCDWriteLines("Playing Song 3", "Pause *  Play");
      }
      else if(goBack != 0){
        state = LO_MenuOptionA;
      }
    break;
  }

  switch (state) { // State Actions
    case LO_MenuOptionA:
    break;
    case LO_MenuOptionC:
    break;
  }
  return state;
}
// Task 2
char posX = 6;
char posY = 0;
int countJ = 1;
int TickFct_JoystickInput(int state) { //WILL GIVE CURSOR POSITION and also will be able to give button input
  switch (state) { // State Transitions
    case JI_init:
        state = JI_Sample;
        lcd.setCursor(posX,posY);
        lcd.blink();
    break;

    case JI_Sample:
        lcd.setCursor(posX,posY);

    break;

    break;
  }

   switch (state) { // State Actions
    case JI_Sample:
        int input = readStick();
        if(digitalRead(sw)== LOW){
            userPress =1;
        } 
        if(countJ < 5 && userPress == 1){
          userPress = 1;
          countJ++;
        }
        else if (countJ >= 5){ 
            userPress = 0;
            countJ=0;
        }
        if(nextPage == 0){
            switch(input){
                case 0:
                break;

                case 1:
                if(posY >=1 ){
                    posY--;
                    lcd.blink();
                }
                else if(posY < 1){
                    posY=0;
                }
                break;

                case 2:
                if(posY <=1 ){
                    posY=1;
                }
                else if(posY > 1){
                    posY++;
                    lcd.blink();
                }
                break;

                case 3: //RIGHT
                if(posX >= 7){
                    posX=7;
                }
                else if(posX < 7){
                    posX++;
                    lcd.blink();
                }
                break;

                case 4:
                if(posX >= 7){
                    posX--;
                    lcd.blink();
                }
                else if(posX < 7){
                    posX=6;
                }
                break;

            }
        }
        else{
            if(readStick() == 1){
                state = JI_init;
                menuOption = 0;
                menuOption2=  0;
                menuOption3 = 0;
                selectedSong = 0;
                nextPage = 0;
                goBack = 1;
                posX=6;
                posY=0;
                lcd.blink();
            }
            switch(input){
                case 3:
                if(posX >= 7){
                    posX=7;
                }
                else if(posX < 7){
                    posX++;
                    lcd.blink();
                }
                break;
                case 4:
                if(posX >= 7){
                    posX--;
                    lcd.blink();
                }
                else if(posX < 7){
                    posX=6;
                }
                break;
            }
        }
   }
  return state;
}
// Sound Output
int counter = 0;
int note = 0;
int soundflag= 1; //Verifies that the song has been selected to be outputted
int TickFct_SoundOutput(int state) { //Produces an insane melody on the buzzer
  switch (state) { // State Transitions
    case SO_init:
      goBack = 0;
      note=0;
      if(selectedSong == 0){
        state=SO_init;
      }
      else if(selectedSong == 1 && nextPage == 1){
        soundflag =1;
        state=SO_SoundOn;
      }
      else if(selectedSong == 2 && nextPage == 1){
        soundflag = 2;
        state=SO_SoundOn2;
      }
      else if(selectedSong == 3 && nextPage == 1){
        soundflag=3;
        state=SO_SoundOn3;
      }
    break;
    case SO_SoundOn:
      if(counter >= song1_time[note] && pause == 0 && goBack == 0) {
         state = SO_SoundOff;
         note++;
         counter = 0;
         if(note >= 39){
           state = SO_init;
                menuOption = 0;
                menuOption2=  0;
                menuOption3 = 0;
                selectedSong = 0;
                nextPage = 0;
                goBack = 1;
                posX=6;
                posY=0;
                note=0;
                lcd.blink();
         }
      }
      else if(goBack != 0){
        state = SO_init;
      }
      else{
        state = SO_Pause;
      }
    break;
    case SO_SoundOn2:
      if(counter >= song2_time[note] && pause == 0 && goBack == 0) {
        state = SO_SoundOff2;
        note++;
        counter = 0;
        if(note >= 27){
          state = SO_init;
          menuOption = 0;
          menuOption2=  0;
          menuOption3 = 0;
          selectedSong = 0;
          nextPage = 0;
          goBack = 1;
          posX=6;
          posY=0;
          note=0;
          lcd.blink();
         }
      }
      else if(goBack != 0){
        state = SO_init;
      }
      else{
        state = SO_Pause;
      }
    break;
    case SO_SoundOn3:
      if(counter >= song3_time[note] && pause == 0 && goBack == 0) {
         state = SO_SoundOff3;
         note++;
         counter = 0;
         if(note >= 27){
           state = SO_init;
                menuOption = 0;
                menuOption2=  0;
                menuOption3 = 0;
                selectedSong = 0;
                nextPage = 0;
                goBack = 1;
                posX=6;
                posY=0;
                note=0;
                lcd.blink();
         }
      }
      else if(goBack != 0){
        state = SO_init;
      }
      else{
        state = SO_Pause;
      }
    break;
    case SO_SoundOff:
      state = SO_SoundOn;
    break;
    case SO_SoundOff2:
      state = SO_SoundOn2;
    break;
    case SO_SoundOff3:
      state = SO_SoundOn3;
    break;
    case SO_Pause:
      if(pause == 1){
        state = SO_Pause;
      }
      else if(pause == 0){
        if(soundflag == 1){
          state = SO_SoundOn;
        }
        else if(soundflag == 2){
          state = SO_SoundOn2;
        }
        else if(soundflag == 3){
          state = SO_SoundOn3;
        }
      }
    break;
  }
   switch (state) { // State Actions
    case SO_SoundOn:
      tone(buzzer, song1[note], periodSoundOutput * song1_time[note]);
      counter++;
    break;
    case SO_SoundOn2:
      tone(buzzer, song2[note], periodSoundOutput * song2_time[note]);
      counter++;
    break;
    case SO_SoundOn3:
      tone(buzzer, song3[note], periodSoundOutput * song3_time[note]);
      counter++;
    break;
    case SO_SoundOff:
      noTone(buzzer);
    break;

  }
  return state;
}

// Task 4 (Unused)

int TickFct_Controller(int state) { //All the relayed information, from button presses and menuOptions will be accessed here and then
                                    // relayed to several other functions. Controlling and managing the menu, play pause, and even the selected buttons
                                    // from the menu that is selected
  switch (state) { // State Transitions
    case C_init:
      state = C_T1;
    break;
    case C_T1:
      if(menuOption2 == 0){
        state = C_T1;
      }
      else{
        state = C_T2;
      }
    break;
    case C_T2:
      if(menuOption2 != 0){
        state = C_T2;
      }
      else{
        state = C_T1;
      }
    break;
  }

   switch (state) { // State Actions
    case C_init:
    break;
    case C_T1:
      if(posX == 6 && posY == 0 && userPress == 1){ //TL
        selectedSong=1;
        menuOption = 1;
      }
      else if(posX == 7 && posY == 0 && userPress == 1){ //TR
        selectedSong=2;
        menuOption = 2; 
      }
      else if(posX == 6 && posY == 1 && userPress == 1){ //BL
        selectedSong=3;
        menuOption = 3; 
      }
      else if(posX == 7 && posY == 1 && userPress == 1){ //BR
        if(selectedSong == 1){
          menuOption2 =1;
          nextPage = 1;
        }
        else if(selectedSong == 2){
          menuOption2 = 2;
          nextPage = 1;
        }
        else if(selectedSong == 3){
          menuOption2 = 3;
          nextPage = 1;
        }
        else if(selectedSong == 0){
          menuOption2 = 0;
        }
      }
    break;
    case C_T2:
      if(posX == 7 && posY == 1 && userPress == 1){ //TR
        lcd.noBlink();
        menuOption3 = 1; 
        pause = 0;
      }
      else if(posX == 6 && posY == 1 && userPress == 1){
        lcd.noBlink();
        menuOption3 = 2;
        pause = 1;
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
  tasks[i].state = C_init;
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
  pinMode(sw, INPUT_PULLUP);
  // Initialize Outputs
  lcd.begin(16, 2);
  // Initialize Inputs

}

void loop() {
  // put your main code here, to run repeatedly:
  // Task Scheduler with Timer.h
  /*Serial.print(" ");
  Serial.println(selectedSong);
  Serial.print("Song ");
  Serial.println(selectedSong);
  */
  //Serial.println(goBack);
}
