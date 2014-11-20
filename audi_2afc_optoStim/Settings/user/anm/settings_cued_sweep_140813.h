// Settings for audi_2afc_cued_1.ino

#ifndef settings_user_anm01_h
#define settings_user_anm01_h
// Rig and animal ID
const char rig_name[] = "epys_01";
const char animalName[] = "anm01";
// Trial type Settings
const byte maxNumTrials = 700;
const byte maxSameSides = 3;
const byte leftProb = 50; // 50% of trials are left side.
const byte probeTrial_frac = 0; // fraction of probe trials: 25% .
char probe_stimType[] = "pureTone";
char stimType[] = "pureTone"; // "noise";// "pureTone";//"sweep";//randompureTone

// Optogenetic stimulation settings
const byte optoStimTrial_frac = 50; // 50% of trials giving optic stimulation
const byte pulse_width = 10; // in ms
const byte pulse_freq = 50; // in Hz
const byte optoStim_duration = 500; // total duration of optic stimulation

// Cue parameters
const int preStim_cue_frac = 0; // % of trials with pre-stimulus cues
const char preStim_cue_type[] = "noise"; 
const int preStim_cue_noise_band[2] = {3000, 40000};
const byte preStim_cue_dur = 100; // in ms.
const int preStim_cue_onset_range[2] = {500, 2000};
const byte preStim_cue_vol = 120;
const byte post_cue_stim_delay = 1200; // delay time after cue and before stimulus

// Reward parameters
const byte leftWaterValveDuration =  25;
const byte rightWaterValveDuration = 25;
const byte LEFT = 0;
const byte RIGHT = 1;
const byte MISS = 2;

// Timing parameters
const byte answerPeriod = 2000; // time when response can be accepted after stimulus.
const byte gracePeriod = 0;
const byte waterValveDelay = 500;//temporally unused
const byte responseDelay = 0; // Delay after stimulus, where licking doens't count. Put responseDelay to 0, if want to test reaction time, but only after well trained.
// const int timeOutInterval = 2000;
const byte ExtraITIDur = 0; // this should be 0 if there are probe trials.
const byte interTrialInterval = 2000;

// Stimulus properties
const byte stimDur = 300;
	// make sure the total delay for stimulus is the same with or without cue.
int stimOnsetRange[2] = {500 + post_cue_stim_delay, 2000 + post_cue_stim_delay};
const uint16_t fq_pureTone[2] = {5000, 8000};//{16000, 22000}; //
const int ranTone_left[3] = {6500, 7500, 8500};
const int ranTone_right[3] = {12500, 13500, 15500};

byte vol[2] = {100,100};
const byte vol_deviation = 20;

// Edge frequncy for bandpass noise or sweeps
const int fq_noise[4]={6000,12000,12000,24000};

const int fq_sweep[4] ={6000,12000,12000,24000};

const int errorLedDur = 10; 


// 1, right side; 0, left side
byte isRandomSides = 1;
byte userDefinedSide = 0; 

#endif
//==========================================================================

