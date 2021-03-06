// Settings for audi_2afc_cued_1.ino

#ifndef settings_user_anm01_h
#define settings_user_anm01_h
// Rig and animal ID
const char rig_name[] = "epys_01";
const char animalName[] = "cll_VIP02";
const uint16_t fq_pureTone[2] = {12000, 20000};//{16000, 22000}; //

// Trial type Settings
const int maxNumTrials = 800;
const int maxSameSides = 3;
const int leftProb = 50; // 50% of trials are left side.
const int probeTrial_frac = 0; // fraction of probe trials: 25% .
char probe_stimType[] = "randompureTone";
char stimType[] = "pureTone"; // "noise";// "pureTone";//"sweep";//randompureTone

// Cue parameters
const int preStim_cue_frac = 0; // % of trials with pre-stimulus cues
const char preStim_cue_type[] = "noise"; 
const int preStim_cue_noise_band[2] = {3000, 40000};
const int preStim_cue_dur = 100; // in ms.
const int preStim_cue_onset_range[2] = {500, 2000};
const int preStim_cue_vol = 120;
const int post_cue_stim_delay = 0; // delay time after cue and before stimulus


// Optogenetic stimulation settings
const byte optoStimTrial_frac = 100; // 50% of trials giving optic stimulation
const uint16_t pulse_width = 10; // in ms
const float pulse_freq = 20; // in Hz
const uint16_t optoStim_duration = 1000; // total duration of optic stimulation
const int opto_side  = 0; // 0"LEFT"; 1"RIGHT"

const uint16_t BeforeTrialOptoDuration = 500; 
const uint16_t AfterTrialOptoDuration = 300; 


// Reward parameters
const int leftWaterValveDuration =  30;
const int rightWaterValveDuration = 25;
const byte LEFT = 0;
const byte RIGHT = 1;
const byte MISS = 2;

// Timing parameters
const int answerPeriod = 2000; // time when response can be accepted after stimulus.
const int gracePeriod = 0;
const int waterValveDelay = 0;//temporally unused
const int responseDelay = 0; // Delay after stimulus, where licking doens't count. Put responseDelay to 0, if want to test reaction time, but only after well trained.
// const int timeOutInterval = 2000;
const int ExtraITIDur = 4000; // this should be 0 if there are probe trials.
const int drinkPeriod = 2000; 
const int interTrialInterval = 3000;

// Stimulus properties
	// make sure the total delay for stimulus is the same with or without cue.
//int stimOnsetRange[2] = {500 + post_cue_stim_delay, 2000 + post_cue_stim_delay};
int stimOnsetRange[2] = {0 + post_cue_stim_delay, 1000 + post_cue_stim_delay};
const int fixedStimOnset = 1000; // fixed 
const int stimDur = 300;


byte vol[2] = {80,80};
const int vol_deviation = 0;

const int ranTone_left[3] = {10000, 10000, 10000};
const int ranTone_right[3] = {20000, 20000, 20000};

// Edge frequncy for bandpass noise or sweeps
const int fq_noise[4]={6000,12000,12000,24000};

const int fq_sweep[4] ={6000,12000,12000,24000};

const int fq_pureTone_probe[2] ={10000,20000};

// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

const int errorLedDur = 0; 
const int errorAirDur =0 ;

#endif
//==========================================================================

