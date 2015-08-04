// Settings for audi_2afc_cued_1.ino

#ifndef settings_user_anm01_h
#define settings_user_anm01_h
// ============================================================
// Rig and animal ID
// ============================================================
const char rig_name[] = "rig4";
const char animalName[] = "ztt_ppc02";

// ============================================================
// Trial type Settings
// ============================================================
const int maxNumTrials = 500;
const int maxSameSides =3;
const int leftProb = 50; // 50% of trials are left side.
const int optoStimTrial_frac =100;
int n_error_stay = 0;

const int post_cue_stim_delay = 0; // delay time after cue and before stimulus

// ============================================================
// Stimulus properties
// ============================================================
char stimType[] = "pureTone"; // "noise";// "pureTone";//"sweep";//randompureTone; //"2ToneSteps";

const uint16_t referenceTones[2] = {10000, 17700};
const uint16_t testTones_hi[2] = {14000, 28000};
const uint16_t testTones_lo[2] = {7000, 14000};
const uint16_t toneInterval = 500;


const uint16_t fq_pureTone[2] = {8000, 32000};//{16000, 22000}; //
const int stimDur = 300;
const int Refer_stimDur = 600;
const uint16_t ranTone_left[3] = {6500, 7500, 8500};
const uint16_t ranTone_right[3] = {12500, 13500, 15500};

byte vol[2] = {160,160};
const int vol_deviation = 0;

const uint16_t fq_noise_lo[]={6000, 12000};
const uint16_t fq_noise_hi[]={12000, 24000};

const uint16_t fq_sweep_lo[] ={6000,12000};
const uint16_t fq_sweep_hi[] ={12000,24000};

// ============================================================
// Probe trial parameters
// ============================================================
char probe_stimType[] = "pureTone"; //"pureTone"; // "noise"; // "sweep"
const int probe_sound_vol = 160;
//char probe_stimType[] = "randompureTone";
const int probeTrial_frac = 30; // fraction of probe trials: 25% .
const uint16_t fq_pureTone_probe[6] ={9756,11888,14492,17665,21534,26250};

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
const int answerPeriod = 2000; // time when response can be accepted after stimulus.
const int gracePeriod = 0;
// A random delay + a fixed delay before sound stimulation.
int stimOnsetRange[2] = {0, 500};
// const uint16_t preSound_time = 500; 

const uint16_t opto_PreSound_delay = 0;

const int waterValveDelay = 0;//temporally unused
const int responseDelay =0; // Delay after stimulus, where licking doens't count. Put responseDelay to 0, if want to test reaction time, but only after well trained.
// const int timeOutInterval = 2000;
const int ExtraITIDur = 2000; // this should be 0 if there are probe trials.
const int interTrialInterval = 2000;
const int drinkPeriod = 2000;


const int errorLedDur = 0; 
const int errorAirDur =0 ;

// ============================================================
// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 

// ============================================================
// Optical stimulation parameters
// ============================================================
// 50% of trials giving optic stimulation
#define multi_pulse 0
#define single_pulse 1
const byte optoStimType = multi_pulse; // 0: multiple pulses
const uint16_t pulse_width = 5; // in ms
const float pulse_freq = 40; // in Hz
// const uint16_t optoStim_duration = 1000; // total duration of optic stimulation
//#define optoStim_duration (unsigned long)(preSound_time+stimDur+responseDelay+answerPeriod)
#define optoStim_duration (unsigned long)(1000)
const int opto_side  = 0; // 0"LEFT"; 1"RIGHT"
const uint16_t optoStim_postTrial_time = 300; 


#endif
//==========================================================================

