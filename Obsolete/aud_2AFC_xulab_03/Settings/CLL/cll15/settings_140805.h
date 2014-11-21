#ifndef settings_user_anm01_h
#define settings_user_anm01_h

const char rig_name[] = "rig01";
const char animalName[] = "cll15";

const int maxNumTrials = 800;
const int maxSameSides = 3;
const int leftProb = 50; // 50% of trials are left side.
// Reward parameters
const int leftWaterValveDuration =  24;
const int rightWaterValveDuration = 25;
const int waterValveDelay = 200;
const int LEFT = 0;
const int RIGHT = 1;
const int MISS = 2;

char stimType[] = "noise"; // "noise";// "pureTone";//"sweep";//randompureTone

const int probeTrial_frac = 0; // fraction of probe trials: 25% .
char probe_stimType[] = "noise";

// Timing parameters
const int answerPeriod = 3000; // time when response can be accepted after stimulus.
const int gracePeriod = 1000;

// Delay after stimulus, where licking doens't count.
// put responseDelay to 0, if want to test reaction time, but only after well trained.
const int responseDelay = 500;

// const int timeOutInterval = 2000;
const int ExtraITIDur = 2000; // this should be 0 if there are probe trials.
const int interTrialInterval = 2000;
// Stimulus properties
const int stimDur = 300;
const uint16_t fq_pureTone[2] = {6000, 12000}; //{1000, 4000};//


const int ranTone_left[3] = {6500, 7500, 8500};
const int ranTone_right[3] = {12500, 13500, 15500};

byte vol[2] = {100, 100};
byte pureTone_ran[2] = {10,10};

// Edge frequncy for bandpass noise or sweeps
const int fq_noise[4]={6000,12000,12000,24000};

const int fq_sweep[4] ={6000,12000,12000,24000};

const int errorLedDur = 10; 


// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

#endif
//==========================================================================

