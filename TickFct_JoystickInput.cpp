char posX = 7;
char posY = 1;
int TickFct_JoystickInput(int state) { //WILL GIVE CURSOR POSITION
  switch (state) { // State Transitions
    case JI_init:
        state = JI_Sample;
        lcd.setCursor(posX,posY);
        lcd.blink();
    break;

    case JI_Sample:
        lcd.setCursor(posX,posY);
    break;
  }

   switch (state) { // State Actions
    case JI_Sample:
        int input = readStick();
        if(digitalRead(sw)== LOW){
            userPress =1;
            lcd.noblink()
        } 
        else{ 
            userPress = 0;
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
                state = C_T1;
                menuOption = 0;
                menuOption2=  0;
                menuOption3 = 0;
                selectedSong = 0;
                nextPage = 0;
                goBack = 1;
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