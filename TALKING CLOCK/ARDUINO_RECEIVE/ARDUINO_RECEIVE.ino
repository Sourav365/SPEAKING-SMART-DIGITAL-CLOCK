/////////////////////// Sound is output on digital pin 3(PWM)///////////////////

#include "Talkie.h"
#include "Vocab_US_Large.h"
#include "Vocab_US_Acorn.h"
#include "Vocab_Special.h"
#include "Vocab_US_Clock.h"

Talkie voice;

void sayNumber(int n) {
  if (n==0) {
    voice.say(sp2_ZERO);
  }
  else {
    if (n>19) {
      int tens = n / 10;
      switch (tens) {
        case 2: voice.say(sp2_TWENTY); break;
        case 3: voice.say(sp2_THIR_); voice.say(sp2_T); break;
        case 4: voice.say(sp2_FOUR); voice.say(sp2_T);  break;
        case 5: voice.say(sp2_FIF_);  voice.say(sp2_T); break;
        case 6: voice.say(sp2_SIX);  voice.say(sp2_T); break;
        case 7: voice.say(sp2_SEVEN);  voice.say(sp2_T); break;
        case 8: voice.say(sp2_EIGHT);  voice.say(sp2_T); break;
        case 9: voice.say(sp2_NINE);  voice.say(sp2_T); break;
      }
      n = n%10;
    }
    switch(n) {
      case 1: voice.say(sp2_ONE); break;
      case 2: voice.say(sp2_TWO); break;
      case 3: voice.say(sp2_THREE); break;
      case 4: voice.say(sp2_FOUR); break;
      case 5: voice.say(sp2_FIVE); break;
      case 6: voice.say(sp2_SIX); break;
      case 7: voice.say(sp2_SEVEN); break;
      case 8: voice.say(sp2_EIGHT); break;
      case 9: voice.say(sp2_NINE); break;
      case 10: voice.say(sp2_TEN); break;
      case 11: voice.say(sp2_ELEVEN); break;
      case 12: voice.say(sp2_TWELVE); break;
      case 13: voice.say(sp2_THIR_); voice.say(sp2__TEEN); break;
      case 14: voice.say(sp2_FOUR); voice.say(sp2__TEEN);break;
      case 15: voice.say(sp2_FIF_); voice.say(sp2__TEEN); break;
      case 16: voice.say(sp2_SIX); voice.say(sp2__TEEN); break;
      case 17: voice.say(sp2_SEVEN); voice.say(sp2__TEEN); break;
      case 18: voice.say(sp2_EIGHT); voice.say(sp2__TEEN); break;
      case 19: voice.say(sp2_NINE); voice.say(sp2__TEEN); break;
    }
  }
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()>0){
     unsigned long epoch= Serial.parseInt();
     Serial.println (epoch);

     int hr;// = (epoch  % 86400L) / 3600 ;
     int mint;// = (epoch  % 3600) / 60;

     voice.say(spa_NOW);
     voice.say(sp4_THE);
     voice.say(sp3_TIME);
     voice.say(sp2_IS);
     //voice.say(spPAUSE2);

/////////////////HOUR///////////////////////////////////
//HOUR = (UTC % 1 DAY) / 1 HOUR........1 DAY = 86400 SEC.
     hr = (epoch  % 86400L) / 3600 ;
     //int c=hr;
     Serial.print(hr);
     Serial.print(":");
     sayNumber(hr);
     //voice.say(spPAUSE1);
     
///////////////MIN///////////////////////////////////////
//MINUTE = (UTC % 1 HOUR) / 1 MIN.......1 Hr. = 3600 Sec.
     mint = (epoch  % 3600) / 60  ;
     Serial.print(mint);
     if (mint == 0){
        voice.say (spc_OCLOCK);
     }
     else sayNumber(mint);
     //voice.say(spPAUSE1);
///////////////////////////////////////////////////////
     //delay(3000);
     if(hr<=11){
        voice.say (spc_GOOD);
        //voice.say(spPAUSE1);
        voice.say (spc_MORNING);
        //voice.say(spPAUSE2);
     }
    if(hr>11 && hr<=17){
        voice.say (spc_GOOD);
        //voice.say(spPAUSE1);
        voice.say (spc_AFTERNOON);
        //voice.say(spPAUSE2);
     }
    if(hr>17){
        voice.say (spc_GOOD);
        //voice.say(spPAUSE1);
        voice.say (spc_EVENING);
        //voice.say(spPAUSE2);
     }
  }
}
