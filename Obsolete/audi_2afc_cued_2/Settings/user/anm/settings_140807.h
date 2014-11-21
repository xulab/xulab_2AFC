// Settings for audi_2afc_cued_1.ino

#ifndef settings_user_anm01_h
#define settings_user_anm01_h
// Rig and animal ID
const char rig_name[] = "rig01";
const char animalName[] = "anm01";
// Trial type Settings
const int maxNumTrials = 500;
const int maxSameSides = 3;
const int leftProb = 70; // 50% of trials are left side.
const int probeTrial_frac = 0; // fraction of probe trials: 25% .
char probe_stimType[] = "pureTone";
char stimType[] = "pureTone"; // "noise";// "pureTone";//"sweep";//randompureTone
// Cue parameters
const int preStim_cue_prob = 100; // % of trials with pre-stimulus cues
const char preStim_cue_type[] = "noise"; 
const int preStim_cue_noise_band[2] = {5000, 40000};
const int preStim_cue_dur = 200; // in ms.
const int preStim_cue_onset_range[2] = {500, 2000};
const int preStim_cue_vol = 150;
const int post_cue_stim_delay = 1000; // delay time after cue and before stimulus

// Reward parameters
const int leftWaterValveDuration =  25;
const int rightWaterValveDuration = 25;
const int waterValveDelay = 700;//temporally unused
const int LEFT = 0;
const int RIGHT = 1;
const int MISS = 2;

// Timing parameters
const int answerPeriod = 4000; // time when response can be accepted after stimulus.
const int gracePeriod = 1000;
const int responseDelay = 500; // Delay after stimulus, where licking doens't count. Put responseDelay to 0, if want to test reaction time, but only after well trained.
// const int timeOutInterval = 2000;
const int ExtraITIDur = 0; // this should be 0 if there are probe trials.
const int interTrialInterval = 2000;
// Stimulus properties
const int stimDur = 300;
const int stimOnsetRange[2] = {500, 2000};
const uint16_t fq_pureTone[2] = {12000, 24000}; //{1000, 4000};//

const int ranTone_left[3] = {6500, 7500, 8500};
const int ranTone_right[3] = {12500, 13500, 15500};

byte vol[2] = {110,110};
const int vol_deviation = 20;

// Edge frequncy for bandpass noise or sweeps
const int fq_noise[4]={6000,12000,12000,24000};

const int fq_sweep[4] ={6000,12000,12000,24000};

const int errorLedDur = 10; 


// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

#endif
//==========================================================================

