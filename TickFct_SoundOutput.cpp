// Sound Output
int counter = 0;
int note = 0;
int soundflag= 1;
int TickFct_SoundOutput(int state) {
  switch (state) { // State Transitions
    case SO_init:
      goBack = 0;
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
         note = note % 20;
      }
      else if(counter < song1_time[note]){
        state = S0_init;
        menuOption = 0;
        menuOption2=  0;
        menuOption3 = 0;
        selectedSong = 0;
        nextPage = 0;
        goBack = 1;
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
         note = note % 20;
      }
      else if(counter < song2_time[note]){
        state = S0_init;
        menuOption = 0;
        menuOption2=  0;
        menuOption3 = 0;
        selectedSong = 0;
        nextPage = 0;
        goBack = 1;
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
         note = note % 20;
      }
      else if(counter < song3_time[note]){
        state = S0_init;
        menuOption = 0;
        menuOption2=  0;
        menuOption3 = 0;
        selectedSong = 0;
        nextPage = 0;
        goBack = 1;
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