// Settings for audi_2afc_cued_1.ino

#ifndef settings_user_anm01_h
#define settings_user_anm01_h
// ============================================================
// Rig and animal ID
// ============================================================
const char rig_name[] = "rig01";
const char animalName[] = "ysg02";

// ============================================================
// Trial type Settings
// ============================================================
const int maxNumTrials = 500;
const int maxSameSides =3;
const int leftProb = 50; // 50% of trials are left side.
const int optoStimTrial_frac = 0;
int n_error_stay = 0;

const int post_cue_stim_delay = 0; // delay time after cue and before stimulus

// ============================================================
// Stimulus properties
// ============================================================
char stimType[] = "2ToneSteps"; // "noise";// "pureTone";//"sweep";//randompureTone; //"2ToneSteps";//"SAMtone"

const uint16_t referenceTones[2] = {10000, 20000};
const uint16_t testTones_hi[2] = {14000, 28000};
const uint16_t testTones_lo[2] = {7000, 14000};
const uint16_t toneInterval = 500;


const uint16_t fq_pureTone[2] = {10000, 28000};//{16000, 22000}; //
const int stimDur = 300;
const int Refer_stimDur = 600;
// make sure the total delay for stimulus is the same with or without cue.
int stimOnsetRange[2] = {500 + post_cue_stim_delay, 1000 + post_cue_stim_delay};
const uint16_t ranTone_left[3] = {6500, 7500, 8500};
const uint16_t ranTone_right[3] = {12500, 13500, 15500};

byte vol[2] = {140,140};
const int vol_deviation = 0;

const uint16_t fq_noise_lo[]={6000, 12000};
const uint16_t fq_noise_hi[]={12000, 24000};

const uint16_t fq_sweep_lo[] ={6000,12000};
const uint16_t fq_sweep_hi[] ={12000,24000};

// ============================================================
// Probe trial parameters
// ============================================================
char probe_stimType[] = "pureTone"; //"pureTone"; // "noise"; // "sweep"
const int probe_sound_vol = 140;
//char probe_stimType[] = "randompureTone";
const int probeTrial_frac = 0; // fraction of probe trials: 25% .
const uint16_t fq_pureTone_probe[4] ={3100,3200, 3300, 3400};

const uint16_t probe_noise_lo[2] = {1300, 1500};
const uint16_t probe_noise_hi[2] = {1200, 1600};

const uint16_t probe_sweep_lo[2] = {2100, 2200};
const uint16_t probe_sweep_hi[2] = {2300, 2400};

// ============================================================
// Cue parameters
// ============================================================
const int preStim_cue_frac = 0; // % of trials with pre-stimulus cues
const char preStim_cue_type[] = "noise"; 
const uint16_t preStim_cue_noise_band[2] = {3000, 40000};
const int preStim_cue_dur = 100; // in ms.
const int preStim_cue_onset_range[2] = {500, 2000};
const int preStim_cue_vol = 150;

// ============================================================
// Reward parameters
// ============================================================
const int leftWaterValveDuration =  25;
const int rightWaterValveDuration = 20;
const byte LEFT = 0;
const byte RIGHT = 1;
const byte MISS = 2;

// ============================================================
// Timing parameters
// ============================================================
const int answerPeriod = 3000; // time when response can be accepted after stimulus.
const int gracePeriod = 0;
const int waterValveDelay = 0;//temporally unused
const int responseDelay =300; // Delay after stimulus, where licking doens't count. Put responseDelay to 0, if want to test reaction time, but only after well trained.
// const int timeOutInterval = 2000;
const int ExtraITIDur = 500; // this should be 0 if there are probe trials.
const int interTrialInterval = 2000;
const int drinkPeriod = 2000;


const int errorLedDur = 10; 
const int errorAirDur =0 ;

//following settings will be used for SAM tone stimulus
int unit_dur = 1; //min time for each tone pips.
const uint16_t carrier_wave_freq = 10;
const double modu_depth=0.001;
/*
    mod_depth   DB
    0.1         20
    0.01        40
    0.001       60
    0.0001      80
  */

// ============================================================
// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

#endif
//==========================================================================

