//always reward during probe trials
// This is based on a previous version: Tone_2AFC_06_combined_2
//#include "Settings/ZY/settings_141024.h"
//#include "Settings/xx/settings_cued_purTone_141104.h"
//#include "C:\Users\lab02\Documents\BehavData_2PRig\xinyu\b02g03\test1202\settings_xinyu_b02g03_141201a.h"
//#include "C:\Users\lab02\Documents\BehavData_2PRig\BehavData_2PRig\CLL\cll_G01\Settings\settings_cll_G01_141216.h"

//#include "/home/xulab/Behavior_rig_11/behavior_data_rig11/ztt/som04/settings/settings_20150112.h"
//#include "Settings_stepTones_150121.h"
//#include "Settings/CLL/cll_B11/settings_cued_purTone_141009.h"
#include "/home/xulab/Behavior_rig_04/arduino-1.5.6-r2/Sketchbook_rig04/Local_2AFC_rig04/aud_2AFC_master_3_opto/settings_aud_2AFC_master_3_opto_150803.h"
// #include "Settings/user/anm/settings_140808.h"

//#include "Settings/useer/anm/settings_140808.h"
// #include "Settings/user/anm/settings_140807.h"
//#include "Settings/user/anm/settings_cued_sweep_140813.h"

#include <SPI_TGM.h>
#include <SPLC.h>
#include <RCM.h>
#include <Wire.h>
#include <SPI.h>
#include <Pulse.h>



#define leftWaterPort 11
#define rightWaterPort 12

#define initButtonPin 8
#define LeftLickPin 2 
#define RightLickPin 3
#define pauseButtonPin 38
// #define StimOutPin 9
//#define led_lick_left 6
#define punishment_air 7
// manual water delivery sensor pins
#define man_water_left 4
#define man_water_right 5

#define triggerPin 9
#define punishment_led 29
#define TriggerOutPin 36

#define LaserOutPin 41
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
unsigned long optoStim_onset;
unsigned long optoStim_offTime;

byte  lickCountLeft = 0;
byte  lickCountRight = 0;
int trialCount = 0;
int randNumber;
int randNumber_2;
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
byte isOptoProbeTrial[maxNumTrials];
// byte preStim_cue_on[maxNumTrials];
byte isOptoTrainingTrial[maxNumTrials];

byte trialStart = 0;
byte trialEnd = 0;
// Sound intensity in DB, can be varied.
int toneFreq = 0;
int SPL_in_DB = 0;
int toneVolume = 0;

String lickTimeLeft;
String lickTimeRight;

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

String probe_param_str = "";
String stim_param_str = "";

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

  Serial.print("Error_stay_number = ");
  Serial.println(n_error_stay);
  
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
    Serial.print("toneFreq_L = ");
    Serial.println(fq_pureTone[0]); 
    Serial.print("toneFreq_R = ");
    Serial.println(fq_pureTone[1]); 
  }
  else if (strcmp(stimType, "noise") == 0)
  {
    Serial.print("noiseBand_L = ");
    Serial.print(fq_noise_lo[0]);Serial.print("\t"); 
    Serial.println(fq_noise_hi[0]);
    Serial.print("noiseBand_R = ");
    Serial.print(fq_noise_lo[1]);Serial.print("\t"); 
    Serial.println(fq_noise_hi[1]);
  }

  else if (strcmp(stimType, "sweep") == 0)
  {
    Serial.print("sweepRange_L = ");
    Serial.print(fq_sweep_lo[0]);Serial.print("\t"); 
    Serial.println(fq_sweep_hi[0]);
    Serial.print("sweepRange_R = ");
    Serial.print(fq_sweep_lo[1]);Serial.print("\t"); 
    Serial.println(fq_sweep_hi[1]);
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

   Serial.print("freqProbe_L = ");
   Serial.println(fq_pureTone_probe[0]);
   Serial.print("freqProbe_R = ");
   Serial.println(fq_pureTone_probe[1]);
  

  Serial.print("stimDuration = ");
  Serial.println(stimDur);
  Serial.print("preStim_cue_frac = ");
  Serial.println(preStim_cue_frac);
  // Serial.print("optoStimTrial_frac = ");
  // Serial.println(optoStimTrial_frac);


  Serial.println("###################");

}

// 1, right side; 0, left side
// void generate_random_sides(int leftProb) {
int define_trial_types(int leftProb, int probeTrial_frac, int probeOptoTrial_frac) {
  
  // int trialType[maxNumTrials];
  // int isProbeTrial[maxNumTrials];
  // int preStim_cue_on[maxNumTrials];

  int right_count = 0;
  int left_count = 0;
  int consecutive_probe_trial_count = 0;
  int consecutive_cued_trial_count = 0;
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
    if (consecutive_probe_trial_count < 2) {
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

i=0;
while (i < maxNumTrials) {
  randNumber_2 = random(0, 100);

  if (randNumber_2 < probeOptoTrial_frac) {
    
   
      isOptoProbeTrial[i] = 1;
      i = i++;
    
  }
  else {
    isOptoProbeTrial[i] = 0;
       
    i = i ++;
  }
}



}
// Serial.print( isProbeTrial[trialCount] );
void define_Optotrial_types( int TrainingOptoTrial_frac){

int i = 0;

while (i < maxNumTrials) {
  randNumber_2 = random(0, 100);

  if (randNumber_2 < TrainingOptoTrial_frac) {
    
   
      isOptoTrainingTrial[i] = 1;
      i = i++;
    
  }
  else {
      
      isOptoTrainingTrial[i] = 0;
       
    i = i ++;
  }
 }
//  Serial.print( isOptoTrainingTrial[trialCount] );

// Generate random number array specifying play cue or not for each trial.

}


// Send a pulse to signal the start of trial.
int sendTrialStartPulse() {
  // digitalWrite(LED, HIGH);
  // delayMicroseconds(100);
  // digitalWrite(LED, LOW);
  lickCountLeft = 0;
  lickCountRight = 0;

  lickTimeLeft = "|";
  lickTimeRight = "|"; 

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

  probe_param_str = "";
  probe_param_str += "/Stim_Probe_stimType=";
  probe_param_str += probe_stimType;

  stim_param_str = "";

  Serial.print("\nTrial_Start....................#");
  Serial.print(trialCount + 1);
  Serial.println("\t");
    
}

uint32_t trial_millis(){
  return millis() - trialStartMillis;
}


void optoStimDelivery(int optoStimType) {
  optoStim_onset = trial_millis();
  if (optoStimType == 0) {
    PULSE.p1_sqr_wave(LaserOutPin, optoStim_duration, pulse_freq, pulse_width, TRG_H);
    optoStim_offTime = optoStim_onset + optoStim_duration;
  }
  if (optoStimType == 1) {
    PULSE.p1_pulse(LaserOutPin,optoStim_duration,TRG_H);
    optoStim_offTime = optoStim_onset + optoStim_duration;
  }
  if (optoStimType == 2) {
    PULSE.p1_cancel();
    optoStim_offTime = trial_millis();
  }
  
}

void probeStimDelivery() {

  // int probeToneFreq
 
  if (strcmp(probe_stimType, "pureTone") == 0){

        //digitalWrite(LaserOutPin, HIGH);
        // Serial.print("\nLaser On Time:\t");
       
        // Serial.print(trial_millis());

        int nProbe_sounds = sizeof(fq_pureTone_probe) / sizeof(fq_pureTone_probe[0]);

        int probeToneFreq = fq_pureTone_probe[random(0,nProbe_sounds)];
        // Play pure tone
        Serial.print("\nProbe pureTone Freq--------------: ");
        Serial.println(probeToneFreq);
        //  Serial.print("Sound in DB = ");   
        // Serial.println(SPL_in_DB);
        Serial.print("Volume = ");   
        Serial.println(probe_sound_vol);
        probe_param_str += "/Stim_Probe_pureTone_freq=";
        probe_param_str += String(probeToneFreq);

       
    // // int optoStim_Start_Time = trial_millis();
    // // pulse(optoStim_duration, pulse_freq, pulse_width, LaserOutPin);
        SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, probeToneFreq, probe_sound_vol);
        delay(stimDur);
        // PULSE.p1_sqr_wave(LaserOutPin, optoStim_duration, pulse_freq, pulse_width, TRG_H);
        

        //SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, probeToneFreq, probe_sound_vol);
     }   

    if (strcmp(probe_stimType, "noise") == 0){
        
        int nProbe_sounds = sizeof(probe_noise_lo) / sizeof(probe_noise_lo[0]);
        int fq_lo = 0;
        int fq_hi = 0;
        int noiseID = random(0,nProbe_sounds);

        fq_lo = probe_noise_lo[noiseID];
        fq_hi = probe_noise_hi[noiseID];
        Serial.print("\nProbe Noise Freq--------------: ");
          Serial.print(fq_lo);Serial.print("\t"); 
          Serial.println(fq_hi);
          Serial.print("Volume = ");   
          Serial.println(probe_sound_vol);

        probe_param_str += "/Stim_probeNoiseFreq_lo=";
        probe_param_str += String(fq_lo);
        probe_param_str += "/Stim_probeNoiseFreq_hi=";
        probe_param_str += String(fq_hi);
          
       SPI_TGM.quick_noise_cosramp_5ms(stimDur, fq_lo, fq_hi, probe_sound_vol, SWEEP_NOISE_WHITE);
          
    }
    if (strcmp(probe_stimType, "sweep") == 0){

          int nProbe_sounds = sizeof(probe_sweep_lo) / sizeof(probe_sweep_lo[0]);
          int fq_on = 0;
          int fq_off = 0;
          int sweepID = random(0,nProbe_sounds);

          fq_on = probe_sweep_lo[sweepID];
          fq_off = probe_sweep_hi[sweepID];
        
          Serial.print("\nProbe Sweep Freq--------------: ");
          Serial.print(fq_on);Serial.print("\t"); 
          Serial.println(fq_off);
          Serial.print("Volume = ");   
          Serial.println(probe_sound_vol);
          probe_param_str += "/Stim_probeSweepFreq_on=";
          probe_param_str += String(fq_on);
          probe_param_str += "/Stim_probeSweepFreq_off=";
          probe_param_str += String(fq_off);

          SPI_TGM.quick_sweep_exp_cosramp_5ms(stimDur, fq_on, fq_off, probe_sound_vol);
      
      } 
}



  void stimulusDelivery(int trialCount, int currentSide, char* stim_type) {

  //  Serial.print("/Stim_Type=");
  // Serial.print(stimType);
  // Serial.print("/Stim_toneFreq=");
  // Serial.print(toneFreq);
  // Serial.print("/Stim_toneIntensity=");
  // Serial.print(SPL_in_DB);
  // Serial.print("/Stim_setVolume=");
  // Serial.print(vol[currentSide]);
  // Serial.print("/stimDuration=");
  // Serial.print(stimDur);
  // Serial.print(stim_param_str);
  // Play the cue sound if cue is on for this trial
  // if (preStim_cue_on[trialCount] == 1) {
  //   int f1 = preStim_cue_noise_band[0]; 
  //   int f2 = preStim_cue_noise_band[1];
  
  //   delay(rand_cueOnset);

  //     Serial.print("\nPlay Cue Noise............ ");
      
  //   cueOnTime = trial_millis();
  //   // play cue sound
  //   SPI_TGM.quick_noise_cosramp_5ms(preStim_cue_dur, f1, f2, preStim_cue_vol, SWEEP_NOISE_WHITE);
  //   // delay time after cue and before stimulus
  //   delay(post_cue_stim_delay);

  //   stim_param_str += "/Stim_cueType=";
  //   stim_param_str += String(preStim_cue_type);
  //   stim_param_str += "/Stim_cueDelay=";
  //   stim_param_str += String(rand_cueOnset);
  
  // }

  stimOnTime = trial_millis();
  // Play sound stimulus  
      if (strcmp(stim_type, "pureTone") == 0){
        
          // Sound intensity (volume) is set to a random value
          // int DB_side[]  = {50,50};
          // DB_side[0] = 50 + random(0, 10);
          // DB_side[1] = 50 + random(0, 10);
          toneVolume = vol[currentSide]; // + random(0, vol_deviation);
          toneFreq = fq_pureTone[currentSide];
        // Play pure tone
        Serial.print("\nTone Freq--------------: ");
        Serial.println(toneFreq);
        //  Serial.print("Sound in DB = ");   
        // Serial.println(SPL_in_DB);
        Serial.print("setVolume = ");   
        Serial.println(toneVolume);

        

        SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq, toneVolume);
        delay(stimDur);

        stim_param_str += "/Stim_Type=";
        stim_param_str += String(stim_type);
        stim_param_str += "/Stim_toneFreq=";
        stim_param_str += String(toneFreq);
        stim_param_str += "/Stim_toneIntensity=";
        stim_param_str += String(SPL_in_DB);
        stim_param_str += "/Stim_setVolume=";
        stim_param_str += String(vol[currentSide]);
        stim_param_str += "/stimDuration=";
        stim_param_str += String(stimDur);

     }   
    if (strcmp(stim_type, "noise") == 0){
    
        int fq_lo = 0;
        int fq_hi = 0;

          if (currentSide == LEFT){ // upward sweeps
            fq_lo = fq_noise_lo[0];
            fq_hi = fq_noise_hi[0];
          }
          if (currentSide == RIGHT){ // downward sweeps
            fq_lo = fq_noise_lo[1];
            fq_hi = fq_noise_hi[1];
          }
          SPI_TGM.quick_noise_cosramp_5ms(stimDur, fq_lo, fq_hi,vol[currentSide], SWEEP_NOISE_WHITE);
          Serial.print("\nNoise Freq--------------: ");
          Serial.print(fq_lo);Serial.print("\t"); 
          Serial.println(fq_hi);
          Serial.print("setVolume = ");   
          Serial.println(vol[currentSide]);
          stim_param_str += "/Stim_Type=";
          stim_param_str += String(stim_type);
          stim_param_str += "/Stim_noiseBand_lo=";
          stim_param_str += String(fq_lo);
          stim_param_str += "/Stim_noiseBand_hi=";
          stim_param_str += String(fq_hi);
          stim_param_str += "/Stim_setVolume=";
          stim_param_str += String(vol[currentSide]);
          stim_param_str += "/stimDuration=";
          stim_param_str += String(stimDur);
    }
    if (strcmp(stim_type, "sweep") == 0){

        // else if (strcmp(stim_type, "sweep") == 0)
        // {
          int fq_on = 0;
          int fq_off = 0;

          if (currentSide == LEFT){ // upward sweeps
            fq_on = fq_sweep_lo[0];
            fq_off = fq_sweep_hi[0];
          }
          if (currentSide == RIGHT){ // downward sweeps
            fq_on = fq_sweep_lo[1];
            fq_off = fq_sweep_hi[1];
          }
          
          SPI_TGM.quick_sweep_exp_cosramp_5ms(stimDur, fq_on, fq_off, vol[currentSide]);

          Serial.print("\nSweep Freq--------------: ");
          Serial.print(fq_on);Serial.print("\t"); 
          Serial.println(fq_off);
          Serial.print("setVolume = ");   
          Serial.println(vol[currentSide]);
          stim_param_str += "/Stim_Type=";
          stim_param_str += String(stim_type);
          stim_param_str += "/Stim_SweepOnsetFreq=";
          stim_param_str += String(fq_on);
          stim_param_str += "/Stim_SweepOffsetFreq=";
          stim_param_str += String(fq_off);
          stim_param_str += "/Stim_setVolume=";
          stim_param_str += String(vol[currentSide]);
          stim_param_str += "/stimDuration=";
          stim_param_str += String(stimDur);
        // }
        }

      if (strcmp(stim_type, "randompureTone") == 0){

        // else if (strcmp(stim_type, "randompureTone") == 0)
        // {
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

          Serial.print("\nTone Freq--------------: ");
          Serial.println(toneFreq);
          Serial.print("setVolume = ");   
          Serial.println(toneVolume);
          stim_param_str += "/Stim_Type=";
          stim_param_str += String(stim_type);
          stim_param_str += "/Stim_toneFreq=";
          stim_param_str += String(toneFreq);
          stim_param_str += "/Stim_toneIntensity=";
          stim_param_str += String(SPL_in_DB);
          stim_param_str += "/Stim_setVolume=";
          stim_param_str += String(vol[currentSide]);
          stim_param_str += "/stimDuration=";
          stim_param_str += String(stimDur);
        }

      if (strcmp(stim_type, "2ToneSteps") == 0){
        stim_param_str += "/Stim_Type=";
        stim_param_str += String(stim_type);
        two_tone_step(currentSide);
      }

} // void



void two_tone_step(int currentSide) {

// toneInterval defined in settings.
  int nRef_Tones = sizeof(referenceTones) / sizeof(referenceTones[0]);
  int randID = random(0,nRef_Tones);
  int currentRefTone = referenceTones [randID];


  //int randID = random(0,2);

  uint16_t currentTestTone = 0;
  //uint16_t referenceTone = 0;
  //uint16_t currentRefTone = 0;

  toneVolume = vol[currentSide];

  if (currentSide == LEFT) {
    //currentRefTone = referenceTones [ referenceID_lo ];
    currentTestTone = testTones_lo [ randID ];
  }

  if (currentSide == RIGHT) {
    //currentRefTone = referenceTones [ referenceID_hi ];
      currentTestTone = testTones_hi [ randID ];
  }


  Serial.print("playing reference tone -------- ");
  Serial.println(currentRefTone);
  SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, currentRefTone, toneVolume);
  delay(stimDur + toneInterval);
  Serial.print("playing test tone -------- ");
  Serial.println(currentTestTone);
  SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, currentTestTone, toneVolume);

  stim_param_str += "/Stim_RefTone=";
  stim_param_str += String(currentRefTone);
  stim_param_str += "/Stim_TestTone=";
  stim_param_str += String(currentRefTone);
  stim_param_str += "/Stim_toneIntensity=";
  stim_param_str += String(SPL_in_DB);
  stim_param_str += "/Stim_setVolume=";
  stim_param_str += String(vol[currentSide]);
  stim_param_str += "/stimDuration=";
  stim_param_str += String(stimDur);
}


// Reward Section
void rewardDelivery(int rewardSide) {
  if (rewardSide == 1) {
    digitalWrite(rightWaterPort, HIGH);
    delay(rightWaterValveDuration);
    digitalWrite(rightWaterPort, LOW);
  }
  else if (rewardSide == 0) {
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
    digitalWrite(punishment_air, LOW);
  }
  //      doExtraITI = 0;
}

void OnLickLeft() {
  answer = LEFT;
  lickCountLeft ++ ;
  // lickTimeLeft[lickCountLeft-1] = trial_millis();
  lickTimeLeft += "|";
  lickTimeLeft += String(trial_millis());

}


void OnLickRight() {
  answer = RIGHT;
  lickCountRight ++ ;
  // lickTimeRight[lickCountRight-1] = trial_millis();
  lickTimeRight += "|";
  lickTimeRight += String(trial_millis());
}

 void sendOutResults() {
  int deadTime = 1000;
  char* stimType_str;
  
  delay(deadTime);
  
  Serial.print("o");
  Serial.print("/Trial_Num=");
  Serial.print(trialCount + 1);
  Serial.print("/Trial_Type=");
  Serial.print(currentSide);
  Serial.print("/Trial_GPeriod=");
  Serial.print(gracePeriod);
  Serial.print("/Trial_isProbeTrial=");
  Serial.print(isProbeTrial[trialCount]);
  
 Serial.print(stim_param_str);
  Serial.print(probe_param_str);


  if(isProbeTrial[trialCount] ==1){

    Serial.print("/Trial_isOptoProbeTrial=");
    Serial.print(isOptoProbeTrial[trialCount]);
    Serial.print("/Trial_isOptoTraingTrial=");
    Serial.print(0);

    }
   else {

    Serial.print("/Trial_isOptoProbeTrial=");
    Serial.print(0);

    Serial.print("/Trial_isOptoTraingTrial=");
    Serial.print(isOptoTrainingTrial[trialCount]);


   }





  // Stimulus properties
  // stim_param_str += "/Stim_Cue_ON=";
  // stim_param_str += String(preStim_cue_on[trialCount]);
  // Serial.print(stim_param_str);
  // Serial.print(probe_param_str);

  // events times
  Serial.print("/Time_trialStart=");
  Serial.print(trialStartTime);
  Serial.print("/Time_stimOnset=");
  Serial.print(stimOnTime);
  Serial.print("/Time_optoStimOnset=");
  Serial.print(optoStim_onset);
  Serial.print("/Time_optoStimOffTime=");
  Serial.print(optoStim_offTime);
  Serial.print("/Time_answer=");
  Serial.print(responseTime);
  Serial.print("/Time_reward=");
  Serial.print(waterValveOpenTime);
  // animal responses
  Serial.print("/Action_numLickLeft=");
  Serial.print(lickCountLeft);
  Serial.print("/Action_lickTimeLeft=");
  Serial.print(lickTimeLeft);

  Serial.print("/Action_numLickRight=");
  Serial.print(lickCountRight);
  Serial.print("/Action_lickTimeRight=");
  Serial.print(lickTimeRight);

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

void drinkTime() {
  int drinkStart = millis();
  int timePassed = 0;
  while (timePassed < drinkPeriod) {
    timePassed = millis() - drinkStart;
  }
}

void choice_outcome_state() {
  // Start answer judgement
  unsigned long timePassed = 0;
  unsigned long timeAnswerStart = millis();
  
  int gracePeriod_onset = trial_millis();
  int gracePeriod_pass = 0;
    // Initialize answer value as MISS.
    // "answer" changes upon every lick, 
  // use "choice" to record the final behavioral response.
  answer = MISS;

  choice = MISS; 
  
  // Enter Answer period
  while (timePassed < answerPeriod) {
    // Deliver water when manually press the sensors

    man_waterValve();

    timePassed = millis() - timeAnswerStart;

    if (answer != MISS) {
      responseTime = trial_millis();
      // Probe trials
      if (isProbeTrial[trialCount] == 1) {
         
        choice = answer;
        delay(waterValveDelay);
        rewardDelivery(answer);
        
        break;
      }

      if (answer == Correct) {
        delay(waterValveDelay);
        rewardDelivery(Correct);
        waterValveOpenTime = trial_millis();
        choice = Correct;
        drinkTime();
        break;
      }
      if (answer == Wrong) {
        // responseTime = trial_millis();
        // Wrong answer, enter grace period
        error_led(); // give feedback to animal of Wrong choice.
        gracePeriod_pass = trial_millis() - gracePeriod_onset;
        if (gracePeriod_pass > gracePeriod) {
              // Use extra ITI as punishment
              doExtraITI = 1;
              punishmentSection(doExtraITI);
              choice = Wrong;
              // This is for "Error Stay" training, 
              // repeat the same side in the next n trials
              for (int counts=1; counts<=n_error_stay; counts++) {
                trialType[ trialCount + counts ] = currentSide;  
              }
          
          break;
        }
      }
    // answer = getAnswers(answerPeriod);
    if (answer == 2) {
    // Miss! Do nonthing.
      }
    }
  }
}

void setup() {
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
 PULSE.init();
 delay(1000);
 SPI_TGM.init(MEGA2560);
 define_trial_types(leftProb, probeTrial_frac, probeOptoTrial_frac);
 define_Optotrial_types(TrainingOptoTrial_frac);



//  Serial.println("================================================="); 
// Serial.println("====================");
//   Serial.print("/isOptoTrainingTrial=");
//   for(int i=0; i<maxNumTrials; i++)
//   {
//     Serial.print(isOptoTrainingTrial[i]);
//   }
//   Serial.println("====================");
//     Serial.print("/isOptoProbeTrial=");
//     for(int i=0; i<maxNumTrials; i++)
//   {
//     Serial.print(isOptoProbeTrial[i]);
//   }
//   // i

 Serial.println("=================================================");  
 Serial.print("#");
 Serial.println("Push the start button to start and make sure the pause button being pressed");

 print_settings();
//*
attachInterrupt(0, OnLickLeft, RISING);
attachInterrupt(1, OnLickRight, RISING);

pinMode(leftWaterPort, OUTPUT);
pinMode(rightWaterPort, OUTPUT);
  // pinMode(StimOutPin, OUTPUT);
  pinMode(LaserOutPin, OUTPUT);
  digitalWrite(LaserOutPin, LOW);
  pinMode(LeftLickPin, INPUT);
  pinMode(RightLickPin, INPUT);
  pinMode(initButtonPin, INPUT);
  pinMode(pauseButtonPin, INPUT);

  pinMode(punishment_led, OUTPUT);
  pinMode(punishment_air, OUTPUT);
  pinMode(TriggerOutPin, OUTPUT);
  digitalWrite(initButtonPin, LOW);
  digitalWrite(pauseButtonPin, LOW);
  digitalWrite(leftWaterPort, LOW);
  digitalWrite(rightWaterPort, LOW);
  //pinMode(led_lick_left, OUTPUT);
  //pinMode(led_lick_right, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
}


void error_led() {
  digitalWrite(punishment_led, HIGH);
  delay(errorLedDur); 
  digitalWrite(punishment_led,LOW);

 digitalWrite(punishment_air, HIGH);
  delay(errorAirDur); 
  digitalWrite(punishment_air,LOW);
}


void loop() {
  // int stimDelay; // for the delay time before stimulus onset
  //  Serial.println(maxNumTrials);
 
  while (trialCount < maxNumTrials) {

    if (digitalRead(pauseButtonPin) ==  LOW)
    {
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

    // Start stimulus delivery
    // stimOnTime = trial_millis();
    digitalWrite(TriggerOutPin, HIGH);
    delay(1);
    digitalWrite(TriggerOutPin, LOW);

    // if (preStim_cue_on[trialCount] != 1) {
    //   delay(rand_stimOnset);
    // }

    if (isProbeTrial[trialCount] == 1) {

        if(isOptoProbeTrial[trialCount] == 1){


          delay(rand_stimOnset - opto_PreSound_delay);

          optoStimDelivery(optoStimType); // Light stimulation only in probe trials.
      
          delay(opto_PreSound_delay);
          probeStimDelivery();

          }
        else{

            delay(rand_stimOnset);
            probeStimDelivery();

        } 
    }
    


    if (isProbeTrial[trialCount] == 0){

      if (isOptoTrainingTrial[trialCount] == 1) {

          delay(rand_stimOnset - opto_PreSound_delay);

          optoStimDelivery(optoStimType); // Light stimulation only in probe trials.
      
          delay(opto_PreSound_delay);
          stimulusDelivery(trialCount, currentSide, stimType_str);

      }

      else{

      delay(rand_stimOnset);
      stimulusDelivery(trialCount, currentSide, stimType_str);

      }

    }
    
    // Delay after stimulus, where licking doens't count.
    // Some sort of grace period for making decision.
    delay(responseDelay);

    choice_outcome_state();

    // If need to stop the laser stimulation at some point, call the following function.
    //
    //optoStimDelivery(2);

    sendOutResults();
    trialCount++;
    delay(interTrialInterval);
    }
  }
}
