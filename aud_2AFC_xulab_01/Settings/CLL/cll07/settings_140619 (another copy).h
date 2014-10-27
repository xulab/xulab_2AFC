// settings_user_anm01.h
// This section needs to be consistent with the file name.
#ifndef SETTINGS_H
#define SETTINGS_H

// The settings can be put here
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Setion Info, Need to be checked for every animal!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char rig_name[] = "rig01";
const char animalName[] = "cll07";

// Trial settings
const char stimType[] = "sweep"; // "noise";// "pureTone";//"sweep";//randompureTone
const int maxNumTrials = 500;
const int maxSameSides = 3;
const int leftProb = 50; // 50% of trials are left side.
// Reward parameters
//const int waterValveDuration = 25; // 100 ms
const int leftWaterValveDuration =  25;
const int rightWaterValveDuration = 25;
const int waterValveDelay = 200;
const int LEFT = 0;
const int RIGHT = 1;
const int MISS = 2;

//const int drinkTime = 50;
//const int leftDrinkTime = 25;
//const int rightDrinkTime = 15;


// Timing parameters
// const int timeOutTime = 3000;
// const int SamplingTime = 500;
const int answerPeriod = 3000; // time when response can be accepted after stimulus.
const int gracePeriod = 0;

// Delay after stimulus, where licking doens't count.
// put responseDelay to 0, if want to test reaction time, but only after well trained.
const int responseDelay = 500;

// const int timeOutInterval = 2000;
const int ExtraITIDur = 4000;
const int interTrialInterval = 3000;
// Stimulus properties
const int stimDur = 300;

byte vol[2] = {150, 150};


// Edge frequncy for bandpass noise or sweeps
//const int  fq_lo[2] ={6000,12000}; 
//const int  fq_hi[2] ={12000,24000};

const uint16_t fq_pureTone[2] = {6000, 12000}; //{1000, 4000};//

const int fq_noise[4]={6000,12000,12000,24000};

const int fq_sweep[4] ={6000,12000,12000,24000};

const int ranTone_left[3] = {6500, 7500, 8500};
const int ranTone_right[3] = {12500, 13500, 15500};







const int errorLedDur = 10; 

// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

#endif
