// This is based on a previous version: Tone_2AFC_06_combined_2
 // #include "Settings/ZTT/new11/settings_cued_sweep_140822.h"

// #include "Settings/user/anm/settings_140808.h"

//#include "Settings/user/anm/settings_140808.h"
// #include "Settings/user/anm/settings_140807.h"
#include "/home/xulab/Ephys_rig_01/Ephys_rig_01_data/CLL/cll_VIP04/Settings/settings_optoStim_cll_VIP04_150106_2.h"
// #include "Settings/ZTT/new11/settings_cued_sweep_140822.h"

#include <SPI_TGM.h>
#include <SPLC.h>
#include <RCM.h>
#include <Wire.h>
#include <SPI.h>
#include <Pulse.h>



#define leftWaterPort 11
#define rightWaterPort 12

#define LeftLickPin 2 
#define RightLickPin 3
#define initButtonPin 39
// #define StimOutPin 9
//#define led_lick_left 6
//#define led_lick_right 7
// manual water delivery sensor pins
#define man_water_left 4
#define man_water_right 5

#define triggerPin 9
#define punishment_led 29

#define LaserOutPin 31
// #define pulse_dur 499
// #define pusle_fq 20
// #define pulse_width 10


byte currentSide = 1;
byte choice =3 ;
unsigned int clockSyncTime;
//int isRandomSides =0;
//int userDefinedSide = 0;//1(right)
//int currentSide =1;
unsigned long trialStartMillis; // For offsetting trial events times.

String trialStartTime; // Trial start absolute time.
String sessionStartTime;
unsigned long stimOnTime;
unsigned long responseTime;
unsigned long waterValveOpenTime;

byte  lickCountLeft = 0;
byte  lickCountRight = 0;
int trialCount = 0;
int randNumber;
byte doExtraITI;
int cueOnTime;
volatile int answer = 2;
volatile uint8_t lick_flag = 0;

// Correct or Wrong (value: 0 or 1) is defined based on trial type.
byte Correct = 3;
byte Wrong = 3;

// extern uint32_t timer0_millis;

// Initialize timing variables, to be set in setTimes()
int rand_stimOnset = 0;
int rand_cueOnset = 0;


// Store trial sides to an array
// int trialSides[maxNumTrials];
byte trialType[maxNumTrials];
byte isProbeTrial[maxNumTrials];
byte preStim_cue_on[maxNumTrials];
byte isOptoTrial[maxNumTrials];

byte trialStart = 0;
byte trialEnd = 0;
// Sound intensity in DB, can be varied.
int toneFreq = 0;
int SPL_in_DB = 0;
int toneVolume = 0;

uint32_t _millis_;
uint32_t _second_;
uint32_t _min_;
uint32_t _hour_;
uint32_t _date_;
uint32_t _month_;
uint32_t _year_;
uint32_t _init_second_;
uint32_t _init_min_;
uint32_t _init_hour_;
uint32_t _init_date_;
uint32_t _init_month_;
uint32_t _init_year_;

uint32_t delay_after_opto;

// void pulse(uint32_t duration, float fq, uint32_t width,uint32_t pin)
// {
//   uint32_t time0, cycle;
//   cycle = 1000/fq;
//   time0 = millis();
//   while((millis()-time0) < duration)
//   {
//     digitalWrite(pin, HIGH);
//     delay(width);
//     digitalWrite(pin, LOW);
//     delay(cycle - width);
//   }
//   digitalWrite(pin, LOW);
// }

void setRandomTimes() {
  rand_stimOnset = random(stimOnsetRange[0], stimOnsetRange[1]);
  rand_cueOnset = random(preStim_cue_onset_range[0], preStim_cue_onset_range[1]);
}

void init_time(){
  RCM.init();
  byte temp_cm_sec = RCM.second();
  while(temp_cm_sec == RCM.second());
  // timer0_millis = 0;

  clockSyncTime = millis();
  _init_second_= RCM.second();
  _init_min_= RCM.minute();
  _init_hour_= RCM.hour();
  _init_date_= RCM.date();
  _init_month_= RCM.month();
  _init_year_ = RCM.year();
  _year_ = _init_year_;
  _month_ = _init_month_;
  _date_ = _init_date_;
  
}

void realtime(){
  uint32_t temp_millis = millis() - clockSyncTime; 
  uint32_t temp_sec = temp_millis /1000;
  uint32_t temp_min = temp_sec/60;
  uint32_t temp_hour = temp_min/60;
  _second_ = _init_second_ + temp_sec;
  _min_ = _init_min_ + (_second_ / 60);
  _hour_ =  _init_hour_ + (_min_ / 60);
  _second_ = _second_ % 60;
  _min_ = _min_ % 60;
  _hour_ = _hour_ % 24;
  _millis_ = temp_millis % 1000;
}

// uint32_t q_millis(){
//   return millis() - clockSyncTime;
// }

void print_settings() {
  Serial.print("Rig_name = ");
  Serial.println(rig_name);
  
  Serial.print("animalName = ");
  Serial.println(animalName);

  Serial.print("sessionStartTime = ");
  Serial.println(sessionStartTime);

  Serial.print("maxNumTrials = ");
  Serial.println(maxNumTrials);
  Serial.print("maxSameSides = ");
  Serial.println(maxSameSides);

  Serial.print("leftProb = ");
  Serial.println(leftProb);
  
  Serial.print("leftWaterValveDuration = ");
  Serial.println(leftWaterValveDuration);
  
  Serial.print("rightWaterValveDuration = ");
  Serial.println(rightWaterValveDuration);
  
  Serial.print("waterValveDelay = ");
  Serial.println(waterValveDelay);


  Serial.print("responseDelay = ");
  Serial.println(responseDelay);

  Serial.print("answerPeriod = ");
  Serial.println(answerPeriod);
  
  Serial.print("gracePeriod = ");
  Serial.println(gracePeriod);
  
  Serial.print("ExtraITIDur = ");
  Serial.println(ExtraITIDur);
  
  Serial.print("interTrialInterval = ");
  Serial.println(interTrialInterval);
  
  if (strcmp(stimType, "pureTone") == 0)
  {
    Serial.print("toneFreq (L, R) = ");
    Serial.print(fq_pureTone[0]); Serial.print("\t"); 
    Serial.println(fq_pureTone[1]);
  }
  else if (strcmp(stimType, "noise") == 0)
  {
    Serial.print("Noise Freq(L,R)= ");
    
    Serial.print(fq_noise[0]);Serial.print("\t"); 
    Serial.print(fq_noise[1]);Serial.print(":"); 
    Serial.print(fq_noise[2]);Serial.print("\t"); 
    Serial.println(fq_noise[3]);
    

  }

  else if (strcmp(stimType, "sweep") == 0)
  {
    Serial.print("Sweep Freq(Low,High)= ");
    Serial.print(fq_sweep[0]);Serial.print("\t"); 
    Serial.print(fq_sweep[1]);Serial.print(":"); 
    Serial.print(fq_sweep[2]);Serial.print("\t"); 
    Serial.println(fq_sweep[3]);
  }
  else if (strcmp(stimType, "randompureTone") == 0)
  {
    Serial.print("randompureTone(left,right)= ");
    Serial.print(ranTone_left[0]);Serial.print("\t"); 
    Serial.print(ranTone_left[1]);Serial.print("\t");
    Serial.print(ranTone_left[2]);Serial.print("\t"); Serial.print(":"); 
    Serial.print(ranTone_right[0]);Serial.print("\t"); 
    Serial.print(ranTone_right[1]);Serial.print("\t"); 
    Serial.println(ranTone_right[2]);
  }

  Serial.print("Frac_Probe_Trials = ");
  Serial.println(probeTrial_frac);
  Serial.print("probe_stimType = ");
  Serial.println(probe_stimType);
  Serial.print("stimDuration = ");
  Serial.println(stimDur);
  Serial.print("preStim_cue_frac = ");
  Serial.println(preStim_cue_frac);
  Serial.print("optoStimTrial_frac = ");
  Serial.println(optoStimTrial_frac);


  Serial.println("###################");

}

// 1, right side; 0, left side
// void generate_random_sides(int leftProb) {
int define_trial_types(int leftProb, int probeTrial_frac, int preStim_cue_frac, int optoStimTrial_frac) {
  
  // int trialType[maxNumTrials];
  // int isProbeTrial[maxNumTrials];
  // int preStim_cue_on[maxNumTrials];

  int right_count = 0;
  int left_count = 0;
  int consecutive_probe_trial_count = 0;
  int consecutive_cued_trial_count = 0;
  int consecutive_opto_trial_count = 0;
  int tempval = 0;

  int i = 0;

  while (i < maxNumTrials) {
  randNumber = random(0, 100); // put a random number between 0 and 100 to the array
  if (randNumber < leftProb) {
    right_count = 0;
    left_count = left_count++;
    if (left_count <= maxSameSides) {
      trialType[i] = LEFT;
      i = i++;
    }
  }
  else {
    left_count = 0;
    right_count = right_count++;
    if (right_count <= maxSameSides) {
      trialType[i] = RIGHT;
      i = i++;
    }
  }

}

i = 0;
while (i < maxNumTrials) {
  randNumber = random(0, 100);

  if (randNumber < probeTrial_frac) {
    consecutive_probe_trial_count = consecutive_probe_trial_count ++;
    if (consecutive_probe_trial_count <= 2) {
      isProbeTrial[i] = 1;
      i = i++;
    }
  }
  else {
    isProbeTrial[i] = 0;
    consecutive_probe_trial_count = 0;        
    i = i ++;
  }
}

// Generate random number array specifying play cue or not for each trial.
i = 0;
while (i < maxNumTrials) {
  randNumber = random(0, 100);
  if (randNumber < preStim_cue_frac) {
    consecutive_cued_trial_count = consecutive_cued_trial_count ++;
    if (consecutive_cued_trial_count <= 5) {
      preStim_cue_on[i] = 1;
      i = i ++;
    }
  }
  else {
      preStim_cue_on[i] = 0;
      consecutive_cued_trial_count = 0;
      i = i ++;
  }
 }

i = 0;
while (i < maxNumTrials) {
  randNumber = random(0, 100);
  if (randNumber <= optoStimTrial_frac) {
    consecutive_opto_trial_count = consecutive_opto_trial_count ++;
    // if (consecutive_opto_trial_count <= 1) {
      isOptoTrial[i] = 1;
      i = i ++;
    // }
  }
  else {
      isOptoTrial[i] = 0;
      consecutive_opto_trial_count = 0;
      i = i ++;
  }
 }
}


// Send a pulse to signal the start of trial.
int sendTrialStartPulse() {
  // digitalWrite(LED, HIGH);
  // delayMicroseconds(100);
  // digitalWrite(LED, LOW);
  lickCountLeft = 0;
  lickCountRight = 0;
  realtime();
  trialStartTime = String(_year_);
  trialStartTime += "_";
  trialStartTime += String(_month_);
  trialStartTime += "_";
  trialStartTime += String(_date_);
  trialStartTime += "_";
  trialStartTime += String(_hour_);
  trialStartTime += "_";
  trialStartTime += String(_min_);
  trialStartTime += "_";
  trialStartTime += String(_second_);
  trialStartTime += "_";
  trialStartTime += String(_millis_);
  trialStartMillis = millis() - clockSyncTime;
  stimOnTime = 0;
  responseTime = 0;
  waterValveOpenTime = 0;
}

uint32_t trial_millis(){
  return millis() - trialStartMillis;
}


void stimulusDelivery(int trialCount, int currentSide, char* stim_type) {

  // Play the cue sound if cue is on for this trial
  if (preStim_cue_on[trialCount] == 1) {
    int f1 = preStim_cue_noise_band[0]; 
    int f2 = preStim_cue_noise_band[1];
    // Display cue info
    // String cue_on_str = String("\n");
    // cue_on_str += "Cue type: ";
    // cue_on_str += preStim_cue_type;
    // cue_on_str += " ........ Cue delay: ";
    // cue_on_str += String(rand_cueOnset);
    // cue_on_str += " ........ ";
    // Serial.println(cue_on_str);
    
    delay(rand_cueOnset);

      Serial.print("\nPlay Cue Noise............ ");
      
    cueOnTime = trial_millis();
    // play cue sound
    SPI_TGM.quick_noise_cosramp_5ms(preStim_cue_dur, f1, f2, preStim_cue_vol, SWEEP_NOISE_WHITE);
    // delay time after cue and before stimulus
    delay(post_cue_stim_delay);
  }
  else {
      if (isOptoTrial[trialCount] == 1) {
      // int optoStim_Start_Time = trial_millis();
      // pulse(optoStim_duration, pulse_freq, pulse_width, LaserOutPin);
      PULSE.p1_sqr_wave(LaserOutPin, optoStim_duration, pulse_freq, pulse_width, TRG_H);
      }
    delay(rand_stimOnset);
  }

  // Play sound stimulus  
  stimOnTime = trial_millis();
  if (strcmp(stim_type, "pureTone") == 0)
  {
      // Sound intensity (volume) is set to a random value
      // int DB_side[]  = {50,50};
      // DB_side[0] = 50 + random(0, 10);
      // DB_side[1] = 50 + random(0, 10);
      toneVolume = vol[currentSide] + random(0, vol_deviation);
      toneFreq = fq_pureTone[currentSide];
      SPL_in_DB = (vol[currentSide]+20)/2;

      int toneVolume_SPIC = 0;
      toneVolume_SPIC = SPLC.get_D_SPL(toneFreq, SPL_in_DB);

      Serial.print("\nTone Freq--------------: ");
      Serial.println(fq_pureTone[currentSide]);
      //  Serial.print("Sound in DB = ");   
      // Serial.println(SPL_in_DB);
      Serial.print("setVolume = ");   
      Serial.println(toneVolume);
      // SPL_in_DB = DB_side[currentSide];
      // toneVolume = SPLC.get_D_SPL(toneFreq, SPL_in_DB);
      // Calculate the volume settings based on the target SPL value and the calibraion curve
      // vol[currentSide] = SPLC.get_D_SPL(toneFreq, SPL_in_DB);
      // digitalWrite(LaserOutPin, HIGH);
      int sound_Start_Time = trial_millis();

      SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq, toneVolume_SPIC);

      delay (stimDur);


      // if (isOptoTrial[trialCount] == 1 and isProbeTrial) {
      // // int optoStim_Start_Time = trial_millis();
      // // pulse(optoStim_duration, pulse_freq, pulse_width, LaserOutPin);
      // PULSE.p1_sqr_wave(LaserOutPin, optoStim_duration, pulse_freq, pulse_width, TRG_H);
      // }

      // int optoStim_total1 = trial_millis() - optoStim_Start_Time;
      //int test_delay = trial_millis();

      // delay(uint16_t(stimDur - optoStim_duration));
      //int test_delay_dur = trial_millis() - test_delay;
      // delay(stimDur);
      // uint16_t delay_Start_Time = trial_millis();
      // if (stimDur > optoStim_duration){
      //   delay_after_opto = stimDur - optoStim_duration;
      // }
      // else{
      //   delay_after_opto=0;
      // }
      // // uint16_t delay_after_opto =((stimDur - optoStim_duration)+abs(stimDur - optoStim_duration))/2;
      // delay (delay_after_opto);
      // int delay_time = trial_millis() - delay_Start_Time;

      // Display stim time info
    String stim_time_info_str = String("\n\n");
    stim_time_info_str += "sound_Start_Time = ";
    stim_time_info_str += String(sound_Start_Time);
    // stim_time_info_str += "\noptoStim_Start_Time = ";
    // stim_time_info_str += String(optoStim_Start_Time);
    // stim_time_info_str += "\noptoStim_total1 = ";
    // stim_time_info_str += String(optoStim_total1);
    // stim_time_info_str += "\ndelay_Start_Time = ";
    // stim_time_info_str += String(delay_Start_Time);
    // stim_time_info_str += "\ndelay_time = ";
    // stim_time_info_str += String(delay_time);

    //stim_time_info_str += "\ntest_delay_dur = ";
    //stim_time_info_str += String(test_delay_dur);

    // stim_time_info_str += "\noptoStim_duration_Set= ";
    // stim_time_info_str += String(optoStim_duration);

    stim_time_info_str += "\nSound_Dur_Set= ";
    stim_time_info_str += String(stimDur);

    

    stim_time_info_str += " ........ \n";
    //Serial.println(stim_time_info_str);
    
      // delay(stimDur - optoStim_duration);
      // digitalWrite(LaserOutPin, LOW);

      

  }
  else if (strcmp(stim_type, "noise") == 0)
  {

    int fq_lo = 0;
    int fq_hi = 0;

      if (currentSide == LEFT){ // upward sweeps
        fq_lo = fq_noise[0];
        fq_hi = fq_noise[1];
      }
      if (currentSide == RIGHT){ // downward sweeps
        fq_lo = fq_noise[2];
        fq_hi = fq_noise[3];
      }


      SPI_TGM.quick_noise_cosramp_5ms(stimDur, fq_lo, fq_hi,vol[currentSide], SWEEP_NOISE_WHITE);
      Serial.print("\nNoise Freq--------------: ");
      Serial.print(fq_lo);Serial.print("\t"); 
      Serial.println(fq_hi);
      Serial.print("setVolume = ");   
      Serial.println(vol[currentSide]);


    }

    else if (strcmp(stim_type, "sweep") == 0)
    {
      int fq_on = 0;
      int fq_off = 0;

      if (currentSide == LEFT){ // upward sweeps
        fq_on = fq_sweep[0];
        fq_off = fq_sweep[1];
      }
      if (currentSide == RIGHT){ // downward sweeps
        fq_on = fq_sweep[2];
        fq_off = fq_sweep[3];
      }
      SPI_TGM.quick_sweep_exp_cosramp_5ms(stimDur, fq_on, fq_off, vol[currentSide]);

      Serial.print("\nSweep Freq--------------: ");
      Serial.print(fq_on);Serial.print("\t"); 
      Serial.println(fq_off);
      Serial.print("setVolume = ");   
      Serial.println(vol[currentSide]);
    }

    else if (strcmp(stim_type, "randompureTone") == 0)
    {
      int vol_side[] = {0, 0};
      
      toneVolume = vol[currentSide] + random(0, vol_deviation);

      if (currentSide == LEFT){ // upward sweeps
        toneFreq = ranTone_left[random(0,3)];
      }
      if (currentSide == RIGHT){ // downward sweeps
       toneFreq = ranTone_right[random(0,3)];
     }
      // Calculate the volume settings based on the target SPL value and the calibraion curve
      // vol[currentSide] = SPLC.get_D_SPL(toneFreq, SPL_in_DB);

      SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq, toneVolume);
      delay (stimDur);



      Serial.print("\nTone Freq--------------: ");
      Serial.println(toneFreq);
      Serial.print("setVolume = ");   
      Serial.println(toneVolume);
    }
    else {
      Serial.println("Error! Wrong stimType in Settings!");
    }
  }


// Reward Section
int rewardDelivery(int answer) {
  if (answer == 1) {
    digitalWrite(rightWaterPort, HIGH);
    delay(rightWaterValveDuration);
    digitalWrite(rightWaterPort, LOW);
  }
  else if (answer == 0) {
    digitalWrite(leftWaterPort, HIGH);
    delay(leftWaterValveDuration);
    digitalWrite(leftWaterPort, LOW);
  }
  // return waterValveOpenTime;
}


// Punishment section, currently only use extra ITI.
void punishmentSection(int doExtraITI) {
  unsigned long ExtraITIStart;
  unsigned long eITI_passed;
  // Serial.println("Enter Punishment!");
  if (doExtraITI == 1) {

    ExtraITIStart = millis();
    eITI_passed = 0;
    
    answer = MISS;
    while (eITI_passed < ExtraITIDur) {
      eITI_passed = (millis() - ExtraITIStart);
      // error_led();
      if (answer == Wrong) {
        error_led();
        eITI_passed = 0;
        ExtraITIStart = millis();
        answer = MISS;
      }
    }
    digitalWrite(punishment_led, LOW);
    
  }
  //      doExtraITI = 0;
}

void flash_leftLed() {

  answer = LEFT;
  lickCountLeft ++ ;
}


void flash_rightLed() {

  answer = RIGHT;
  lickCountRight ++ ;
}

/*void record_lickCountLeft(){

 digitalWrite(led_lick_left, HIGH);
 delayMicroseconds(500);
 digitalWrite(led_lick_left, LOW);
 lickCountLeft ++ ;
 }
 void record_lickCountRight(){


 lickCountRight ++ ;
 }
 */
 void sendOutResults() {
  int deadTime = 1000;
  //  unsigned long deadTimeStart = millis();
  //    while ((millis() - deadTimeStart) < deadTime) {
  //    }
  char* stimType_str;

  if (isProbeTrial[trialCount] == 1) {
    stimType_str = probe_stimType;
  }
  else {
    stimType_str = stimType;
  }
  delay(deadTime);
  // /*
  // String trialResultsStr = String('o');
  // // Trial settings
  // trialResultsStr += "/Trial_Num=";
  // trialResultsStr += String(trialCount + 1);
  // trialResultsStr += "/Trial_Type=";
  // trialResultsStr += currentSide;
  // trialResultsStr += "/Trial_GPeriod=";
  // trialResultsStr += gracePeriod;
  // trialResultsStr += "/Trial_isProbeTrial=";
  // trialResultsStr += isProbeTrial[trialCount];
  // trialResultsStr += "/Trial_preStimCueOn=";
  // trialResultsStr += preStim_cue_on[trialCount];
  // // Stimulus properties
  // trialResultsStr += "/Stim_Cue_ON=";
  // trialResultsStr += preStim_cue_on[trialCount];
  // trialResultsStr += "/Stim_cueType=";
  // trialResultsStr += preStim_cue_type;
  // trialResultsStr += "/Stim_cueDelay=";
  // trialResultsStr += String(rand_cueOnset);

  // trialResultsStr += "/Stim_Type=";
  // trialResultsStr += stimType;
  // trialResultsStr += "/Stim_toneFreq=";
  // trialResultsStr += toneFreq;
  // trialResultsStr += "/Stim_toneIntensity=";
  // trialResultsStr += SPL_in_DB;
  // trialResultsStr += "/Stim_setVolume=";
  // trialResultsStr += vol[currentSide];
  // trialResultsStr += "/stimDuration=";
  // trialResultsStr += stimDur;
  // // events times
  // trialResultsStr += "/Time_trialStart=";
  // trialResultsStr += trialStartTime;
  // trialResultsStr += "/Time_stimOnset=";
  // trialResultsStr += stimOnTime;
  // trialResultsStr += "/Time_answer=";
  // trialResultsStr += responseTime;
  // trialResultsStr += "/Time_reward=";
  // trialResultsStr += waterValveOpenTime;
  // // animal responses
  // trialResultsStr += "/Action_numLickLeft=";
  // trialResultsStr += lickCountLeft;
  // trialResultsStr += "/Action_numLickRight=";
  // trialResultsStr += lickCountRight;
  // trialResultsStr += "/Action_choice=";
  // trialResultsStr += choice;
  // trialResultsStr += "/EOL";

  
  Serial.print("o");
  Serial.print("/Trial_Num=");
  Serial.print(trialCount + 1);
  Serial.print("/Trial_Type=");
  Serial.print(currentSide);
  Serial.print("/Trial_GPeriod=");
  Serial.print(gracePeriod);
  Serial.print("/Trial_isProbeTrial=");
  Serial.print(isProbeTrial[trialCount]);
  Serial.print("/Trial_isOptoTrial=");
  Serial.print(isOptoTrial[trialCount]);
  
  // Serial.print("/Trial_preStimCueOn=");
  // Serial.print(preStim_cue_on[trialCount]);
  // Stimulus properties
  Serial.print("/Stim_Cue_ON=");
  Serial.print(preStim_cue_on[trialCount]);
  Serial.print("/Stim_cueType=");
  Serial.print(preStim_cue_type);
  Serial.print("/Stim_cueDelay=");
  Serial.print(rand_cueOnset);

  Serial.print("/Stim_Type=");
  Serial.print(stimType);
  Serial.print("/Stim_toneFreq=");
  Serial.print(toneFreq);
  Serial.print("/Stim_toneIntensity=");
  Serial.print(SPL_in_DB);
  Serial.print("/Stim_setVolume=");
  Serial.print(vol[currentSide]);
  Serial.print("/stimDuration=");
  Serial.print(stimDur);
  // events times
  Serial.print("/Time_trialStart=");
  Serial.print(trialStartTime);
  Serial.print("/Time_stimOnset=");
  Serial.print(stimOnTime);
  Serial.print("/Time_answer=");
  Serial.print(responseTime);
  Serial.print("/Time_reward=");
  Serial.print(waterValveOpenTime);
  // animal responses
  Serial.print("/Action_numLickLeft=");
  Serial.print(lickCountLeft);
  Serial.print("/Action_numLickRight=");
  Serial.print(lickCountRight);
  Serial.print("/Action_choice=");
  Serial.print(choice);
  Serial.println("/EOL");
  
  // print performance
  if (choice == Correct) {
    Serial.println("CORRECT!");
  }
  else if (choice == Wrong){
    Serial.println("ERROR!");
  }
  else {
    Serial.println("MISS!");
  }
}


void man_waterValve() {
  if (digitalRead(man_water_left) == HIGH) {
    // Open left water valve
    //soundDelivery(0);
    digitalWrite( leftWaterPort, HIGH);
    delay(leftWaterValveDuration);
    digitalWrite(leftWaterPort, LOW);
    delay (500);
  }
  if (digitalRead(man_water_right) == HIGH) {
    // Open right water valve
    //soundDelivery(1);
    digitalWrite( rightWaterPort, HIGH);
    delay(rightWaterValveDuration);
    digitalWrite(rightWaterPort, LOW);
    delay(500);
  }

}


void setup() {
 PULSE.init();
 Serial.begin(115200);
 SPLC.init();
 init_time();
 realtime();
 randomSeed(_second_);
 sessionStartTime = String(_year_);
 sessionStartTime += "_";
 sessionStartTime += String(_month_);
 sessionStartTime += "_";
 sessionStartTime += String(_date_);
 sessionStartTime += "_";
 sessionStartTime += String(_hour_);
 sessionStartTime += "_";
 sessionStartTime += String(_min_);

 delay(1000);
 SPI_TGM.init(MEGA2560);
 define_trial_types(leftProb, probeTrial_frac, preStim_cue_frac, optoStimTrial_frac);

 // Serial.print("\noptoStim_duration_Set= ");
 // Serial.println(String(optoStim_duration));

 Serial.println("=================================================");  
 Serial.print("#");
 Serial.println("Push the start button to start!");

 print_settings();
//*
attachInterrupt(0, flash_leftLed, RISING);
attachInterrupt(1, flash_rightLed, RISING);
pinMode(leftWaterPort, OUTPUT);
pinMode(rightWaterPort, OUTPUT);
  // pinMode(StimOutPin, OUTPUT);
  pinMode(LaserOutPin, OUTPUT);
  digitalWrite(LaserOutPin, LOW);
  pinMode(LeftLickPin, INPUT);
  pinMode(RightLickPin, INPUT);
  pinMode(initButtonPin, INPUT);
  pinMode(punishment_led, OUTPUT);
  digitalWrite(initButtonPin, LOW);
  digitalWrite(leftWaterPort, LOW);
  digitalWrite(rightWaterPort, LOW);
  // pinMode(led_lick_left, OUTPUT);
  // pinMode(led_lick_right, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
}


void error_led() {
  digitalWrite(punishment_led, HIGH);
  delay(errorLedDur); 
  digitalWrite(punishment_led,LOW);

}


void loop() {
  // int stimDelay; // for the delay time before stimulus onset
  //  Serial.println(maxNumTrials);

  while (trialCount < maxNumTrials) {
    // Wait for the first trial to start
    if (trialCount < 1) {
      while (digitalRead(initButtonPin) == HIGH) {
      }
    }
    setRandomTimes();
    // trialStartTime =trial_millis();
    sendTrialStartPulse();
    // Put a random delay between 0.5 - 1 seconds before stimulus
    // stimDelay = 500 + random(1000);
    // delay(stimDelay);

    char* stimType_str = stimType;
  // Define trial type and Correct response
  if (isRandomSides == 1) {
    currentSide = trialType[trialCount];
  }
  else {
    currentSide = userDefinedSide;
  }
  if (currentSide == LEFT) {
    Correct  = LEFT;
    Wrong = RIGHT;
  }
  else if (currentSide == RIGHT) {
    Correct  = RIGHT;
    Wrong = LEFT;
  }

// For probe trials, Correct trials is randomly defined as 0 or 1.
// Stimulus type is also defined differently. 
if (isProbeTrial[trialCount] == 1) {
  stimType_str = probe_stimType;
  Correct = round(random(0, 100)/50);
  Wrong = 1 - Correct;
}

// Start stimulus delivery
// stimOnTime = trial_millis();

stimulusDelivery(trialCount, currentSide, stimType_str);

uint16_t afterSound_Time = trial_millis();

     
// Delay after stimulus, where licking doens't count.
// Some sort of grace period for making decision.
delay(responseDelay);
uint16_t after_responseDelay_Time = trial_millis();

 // Display stim time info
    String test_time_info_str = String("\n\n");
    test_time_info_str += "afterSound_Time = ";
    test_time_info_str += String(afterSound_Time);
    test_time_info_str += "\nafter_responseDelay_Time = ";
    test_time_info_str += String(after_responseDelay_Time);
    test_time_info_str += " ........ \n";
    //Serial.println(test_time_info_str);

// Start answer judgement
unsigned long timePassed = 0;
unsigned long timeAnswerStart = millis();
    // Initialize answer value as MISS.
    // "answer" changes upon every lick, 
    // use "choice" to record the final behavioral response.
    answer = MISS;

    choice = MISS; 
    // Default value of choice is MISS. 
    // It will be changed if lick happens during answer period.
    
    int gracePeriod_onset = trial_millis();
    int gracePeriod_pass = 0;
    // Enter Answer period
    while (timePassed < answerPeriod) {
      // Deliver water when manually press the sensors
      man_waterValve();

      timePassed = millis() - timeAnswerStart;

      if (answer != MISS) {
        responseTime = trial_millis();
      }

      if (answer == Correct) {
        delay(waterValveDelay);
        rewardDelivery(Correct);
        waterValveOpenTime = trial_millis();
        choice = Correct;

        break;
      }
      else if (answer == Wrong) {
      // responseTime = trial_millis();
      // Wrong answer, enter grace period
      error_led(); // give feedback to animal of Wrong choice.
      
      gracePeriod_pass = trial_millis() - gracePeriod_onset;
      if (gracePeriod_pass > gracePeriod) {
            // Use extra ITI as punishment
            doExtraITI = 1;
            punishmentSection(doExtraITI);
            choice = Wrong;
            break;
          }
          else { 
       // Serial.println(answer);
     }
     answer = MISS;  
   }
 }
  // answer = getAnswers(answerPeriod);
  if (answer == 2) {
  // Miss! Do nonthing.
}
sendOutResults();
trialCount++;
delay(interTrialInterval);
}

}
