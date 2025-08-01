int TickFct_Controller(int state) { //CHoose a song and when start
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
        menuOption3 = 1; 
        pause = 0;
      }
      else if(posX == 6 && posY == 1 && userPress == 1){
        menuOption3 = 2;
        pause = 1;
      }
    break;
  }
  return state;
}