// #include <SPI_TGM.h>
// #include "RCM.h"

// #define LED 13
#define leftWaterPort 11
#define rightWaterPort 12

#define LeftLickPin 2
#define RightLickPin 3
#define initButtonPin 8
// #define StimOutPin 9
#define led_lick_left 6
#define led_lick_right 7

#define triggerPin 9
#define punishment_led 13

// Trial settings
const int maxNumTrials = 300;
const int maxSameSides = 3;
const int leftProb = 50; // 50% of trials are left side.
// Reward parameters
//const int waterValveDuration = 25; // 100 ms
const int leftWaterValveDuration = 30;
const int rightWaterValveDuration = 30;
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
const int answerPeriod = 2000; // time when response can be accepted after stimulus.
const int gracePeriod = 500;
// const int timeOutInterval = 2000;
const int ExtraITIDur = 8000;
const int interTrialInterval = 6000;
// Stimulus properties
const int stimDur = 300;
const int toneFreq[2] = {6000, 12000}; //{1000, 4000};//
const int soundAtt = 0; // 1/0.5db.
const int errorLedDur = 10;

// String startYMDHM;
// User defined trial side, to reduce bias.
// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 0; 
int currentSide = 1;
unsigned long clockSyncTime;
//int isRandomSides =0;
//int userDefinedSide = 0;//1(right)
//int currentSide =1;
unsigned long trialStartMillis; // For offsetting trial events times.

String trialStartTime; // Trial start absolute time.
unsigned long stimOnTime;
unsigned long responseTime;
unsigned long waterValveOpenTime;

int  lickCountLeft = 0;
int  lickCountRight = 0;
int trialCount = 0;
int randNumber;
int doExtraITI;
int answer = 2;
extern uint32_t timer0_millis;

#define STR_LENGTH 20
char animalName[STR_LENGTH] = "anm01";
char expDate[STR_LENGTH] = "20140318";
char sessionType[STR_LENGTH] = "tone_2AFC";


// Store trial sides to an array
int trialSides[500];

int trialStart = 0;
int trialEnd = 0;

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

/*
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
*/
// uint32_t q_millis(){
//   return millis() - clockSyncTime;
// }

void print_settings() {
  String trialSettingsStr = "SS";
  trialSettingsStr += " / animalName = ";
  trialSettingsStr += String(animalName);

  trialSettingsStr += " / expDate = ";
  trialSettingsStr += String(expDate);

  trialSettingsStr += " / sessionType = ";
  trialSettingsStr += String(sessionType);

  trialSettingsStr += " / maxNumTrials = ";
  trialSettingsStr += String(maxNumTrials);
  trialSettingsStr += " / maxSameSides = ";
  trialSettingsStr += String(maxSameSides);

  trialSettingsStr += " / leftProb = ";
  trialSettingsStr += String(leftProb);
  
  trialSettingsStr += " / waterDur_L = ";
  trialSettingsStr += String(leftWaterValveDuration);
  
  trialSettingsStr += " / waterDur_R = ";
  trialSettingsStr += String(rightWaterValveDuration);
  
  trialSettingsStr += " / waterValveDelay = ";
  trialSettingsStr += String(waterValveDelay);
  
  trialSettingsStr += " / answerPeriod = ";
  trialSettingsStr += String(answerPeriod);
  
  trialSettingsStr += " / gracePeriod = ";
  trialSettingsStr += String(gracePeriod);
  
  trialSettingsStr += " / ExtraITIDur = ";
  trialSettingsStr += String(ExtraITIDur);
  
  trialSettingsStr += " / interTrialInterval = ";
  trialSettingsStr += String(interTrialInterval);
  
  trialSettingsStr += " / toneFreq_L = ";
  trialSettingsStr += String(toneFreq[0]);
  trialSettingsStr += " / toneFreq_R = ";
  trialSettingsStr += String(toneFreq[1]);
  
  Serial.println(trialSettingsStr);
}

// 1, right side; 0, left side
void generate_random_sides(int leftProb) {
  int seed = 111;
  int right_count = 0;
  int left_count = 0;
  int i = 0;
  randomSeed(seed);
  while (i < maxNumTrials) {
    randNumber = random(0, 100); // put a random number between 0 and 100 to the array
    if (randNumber < leftProb) {
      right_count = 0;
      left_count = left_count++;
      if (left_count <= maxSameSides) {
        trialSides[i] = LEFT;
        i = i++;
      }
    }
    else {
      left_count = 0;
      right_count = right_count++;
      if (right_count <= maxSameSides) {
        trialSides[i] = RIGHT;
        i = i++;
      }
    }
  }
}


// Send a pulse to signal the start of trial.
void sendTrialStartPulse() {
  // digitalWrite(LED, HIGH);
  // delayMicroseconds(100);
  // digitalWrite(LED, LOW);
  lickCountLeft = 0;
  lickCountRight = 0;
  // realtime();
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


int stimulusDelivery(int sides) {

  digitalWrite(triggerPin, HIGH);
  delay(1);
  digitalWrite(triggerPin, LOW);

  if (sides == 0) { // left trial
    // SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq[0], 192 - soundAtt);
    // tone(StimOutPin, toneFreq[0], stimDur);
    Serial.print("\nTone Freq--------------: ");
    Serial.println(toneFreq[0]);
  }

  if (sides == 1) { // right trial
    // SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq[1], 192 - soundAtt);
    Serial.print("\nTone Freq++++++++++++++: ");
    Serial.println(toneFreq[1]);
  }

}

// int getAnswers(uint16_t answerPeriod) {
//   // Left lick: answer = 0. Right lick: answer = 1.
//   // Miss response: answer = 2.
//   answer = 2;
//   unsigned long t0;
//   t0 = millis();

//   while ((millis() - t0) < answerPeriod) {
//     if (digitalRead(LeftLickPin) == HIGH) {
//       answer = 0;
//       responseTime = millis();
//       break;
//     }
//     if (digitalRead(RightLickPin) == HIGH) {
//       answer = 1;
//       responseTime = millis();
//       break;
//     }
//   }
  //  Serial.print(millis() - t0);
  //  Serial.println(" ms passed before answering!");
//   return answer;
// }

// Reward Section
int rewardDelivery(int answer) {
  if (answer == 1) {

    digitalWrite(rightWaterPort, HIGH);
    delay(rightWaterValveDuration);
    digitalWrite(rightWaterPort, LOW);
  }
  else if (answer == 0) {
    // delay(waterValveDelay);
    digitalWrite(leftWaterPort, HIGH);
    delay(leftWaterValveDuration);
    digitalWrite(leftWaterPort, LOW);
  }
  // return waterValveOpenTime;
}

// Punishment section, currently only use extra ITI.
int punishmentSection(int doExtraITI) {
  unsigned long ExtraITIStart;
  if (doExtraITI == 1) {
    ExtraITIStart = millis();
    while ((millis() - ExtraITIStart) < ExtraITIDur) {
      error_led();
    }
  }
  //      doExtraITI = 0;
}

void flash_leftLed() {

  digitalWrite(led_lick_left, HIGH);
  delayMicroseconds(100);
  digitalWrite(led_lick_left, LOW);
  answer = LEFT;
  lickCountLeft ++ ;
}


void flash_rightLed() {

  digitalWrite(led_lick_right, HIGH);
  delayMicroseconds(100);
  digitalWrite(led_lick_right, LOW);
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
  delay(deadTime);
  String trialResultsStr = String('o');
  trialResultsStr += "/trNum=";
  trialResultsStr += String(trialCount + 1);
  trialResultsStr += "/trialType=";
  trialResultsStr += String(currentSide);
  trialResultsStr += "/trialStartTime=";
  trialResultsStr += trialStartTime;
  trialResultsStr += "/Stim_On_Time=";
  trialResultsStr += String(stimOnTime);
  trialResultsStr += "/Answer_Time=";
  trialResultsStr += String(responseTime);
  trialResultsStr += "/Reward_Time=";
  trialResultsStr += String(waterValveOpenTime);
  trialResultsStr += "/Choice=";
  trialResultsStr += String(answer);
  Serial.println(trialResultsStr);

  Serial.print("leftLickCounts:");
  Serial.println( lickCountLeft);

  Serial.print("rightLickCounts:");
  Serial.println( lickCountRight);

}


void setup() {
   Serial.begin(9600);
   // init_time();
   // realtime();

   // sendTrialStartPulse();
   // Serial.println(startYMDHM);
    
    // All later time should be offset to masterClockSecond-clockSyncTime
   
  // Serial.println("***********************");
  // Serial.print("year:");
  // Serial.println(_year_);
  // Serial.print("month:");
  // Serial.println(_month_);
  // Serial.print("date:");
  // Serial.println(_date_);
  // // Serial.print("day:");
  // // Serial.println(RCM.day());
  // Serial.print("hour:");
  // Serial.println(_hour_);
  // Serial.print("minute:");
  // Serial.println(_min_);
  // Serial.print("second:");
  // Serial.println(_second_); 
  // Serial.print("millis:" );
  // Serial.println(_millis_);
  // Serial.println("***********************");
  delay(1000);
  // SPI_TGM.init(MEGA2560);
  Serial.println("=================================================");  
  Serial.print("#");
  Serial.println("Push the start button to start!");
  print_settings();

  attachInterrupt(0, flash_leftLed, RISING);
  attachInterrupt(1, flash_rightLed, RISING);


  pinMode(leftWaterPort, OUTPUT);
  pinMode(rightWaterPort, OUTPUT);
  // pinMode(StimOutPin, OUTPUT);
  pinMode(LeftLickPin, INPUT);
  pinMode(RightLickPin, INPUT);
  pinMode(initButtonPin, INPUT);
  pinMode(punishment_led, OUTPUT);
  digitalWrite(initButtonPin, LOW);

  digitalWrite(leftWaterPort, LOW);
  digitalWrite(rightWaterPort, LOW);
  pinMode(led_lick_left, OUTPUT);
  pinMode(led_lick_right, OUTPUT);

  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);

  // Use extra ITI as punishment
  doExtraITI = 1;

  // Generate a seriers of number for trial sides, with a random seed.
  // Consecutive sides not exceeding maxSameSides
  generate_random_sides(leftProb);

}

void error_led() {
  digitalWrite(punishment_led, HIGH);
  delay(errorLedDur); 
  digitalWrite(punishment_led,LOW);

}


void loop() {
  int stimDelay; // for the delay time before stimulus onset
  int Correct;
  int Wrong;

  
  //  Serial.println(maxNumTrials);

  while (trialCount < maxNumTrials) {
    // Wait for the first trial to start
    if (trialCount < 1) {
      while (digitalRead(initButtonPin) == HIGH) {
      }
    }


    // trialStartTime =trial_millis();
    sendTrialStartPulse();
    // Put a random delay between 0.5 - 1 seconds before stimulus
    stimDelay = 500 + random(1000);
    delay(stimDelay);

  // Define trial type and Correct response
  if (isRandomSides == 1) {
    currentSide = trialSides[trialCount];
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
stimOnTime = trial_millis();
stimulusDelivery(currentSide);

// Start answer judgement
unsigned long timePassed = 0;
unsigned long timeAnswerStart = millis();
    // Initialize answer value as MISS.
  answer = MISS;
  int gracePeriod_onset = trial_millis();
  int gracePeriod_pass = 0;
      
  while (timePassed < answerPeriod) {
    timePassed = millis() - timeAnswerStart;
    if (answer == Correct) {
      responseTime = trial_millis();
      //delay(waterValveDelay);
      rewardDelivery(answer);
      waterValveOpenTime = trial_millis();
      break;
    }
    else if (answer == Wrong) {
      responseTime = trial_millis();
      // Wrong answer, enter grace period
      error_led(); // give feedback to animal of Wrong choice.
      
      gracePeriod_pass = trial_millis() - gracePeriod_onset;
      if (gracePeriod_pass > gracePeriod) {
          punishmentSection(doExtraITI);
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
