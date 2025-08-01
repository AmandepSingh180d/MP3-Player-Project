int TickFct_LCDOutput(int state) {
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