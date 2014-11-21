#include <SPI_TGM.h>

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
const int maxNumTrials = 500;
const int maxSameSides = 3;
// Reward parameters
//const int waterValveDuration = 25; // 100 ms
const int leftWaterValveDuration = 22;
const int rightWaterValveDuration = 20;
const int waterValveDelay = 200;
//const int drinkTime = 50;
//const int leftDrinkTime = 25;
//const int rightDrinkTime = 15;

// Timing parameters
// const int timeOutTime = 3000;
// const int SamplingTime = 500;
const int answerPeriod = 2000; // time when response can be accepted after stimulus.
const int gracePeriod = 0;
// const int timeOutInterval = 2000;
const int ExtraITIDur = 4000;
const int interTrialInterval = 2000;
// Stimulus properties
const int stimDur = 300;
const int toneFreq[2] = {5651, 13086}; //{1000, 4000};//
const int soundAtt = 0; // 1/0.5db.
const int punishmentDur =1000;
 
// User defined trial side, to reduce bias.
// 1, right side; 0, left side
int isRandomSides = 1;
int userDefinedSide = 1; 
int currentSide = 1;

//int isRandomSides =0;
//int userDefinedSide = 0;//1(right)
//int currentSide =1;

unsigned long trialStartTime;
unsigned long stimOnTime;
unsigned long responseTime;
unsigned long waterValveOpenTime;

int  lickCountLeft = 0;
int  lickCountRight = 0;
int trialCount = 0;
int randNumber;
int doExtraITI;
int answer;


// Store trial sides to an array
int trialSides[500];

int trialStart = 0;
int trialEnd = 0;



void generate_random_sides(int seed) {
  int right_count = 0;
  int left_count = 0;
  int i = 0;
  randomSeed(seed);
  while (i < maxNumTrials) {
    randNumber = random(0, 100); // put a random number between 0 and 100 to the array
    if (randNumber < 50) {
      right_count = 0;
      left_count = left_count++;
      if (left_count <= maxSameSides) {
        trialSides[i] = 0;
        i = i++;
      }
    }
    else {
      left_count = 0;
      right_count = right_count++;
      if (right_count <= maxSameSides) {
        trialSides[i] = 1;
        i = i++;
      }
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
}

int stimulusDelivery(int sides) {

  digitalWrite(triggerPin, HIGH);
  delay(1);
  digitalWrite(triggerPin, LOW);

  if (sides == 0) { // right trial
    SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq[0], 192 - soundAtt);
    // tone(StimOutPin, toneFreq[0], stimDur);
    Serial.print("\nTone Freq--------------: ");
    Serial.println(toneFreq[0]);
  }

  if (sides == 1) { // right trial
    SPI_TGM.quick_tone_vol_cosramp_5ms(stimDur, toneFreq[1], 192 - soundAtt);
    Serial.print("\nTone Freq++++++++++++++: ");
    Serial.println(toneFreq[1]);
    // tone(StimOutPin, toneFreq[1], stimDur);
    //    delay(stimDur);
    //    digitalWrite(LED, LOW);
    //    delay(stimDur);
  }

}

int getAnswers(uint16_t answerPeriod) {
  // Left lick: answer = 0. Right lick: answer = 1.
  // Miss response: answer = 2.
  answer = 2;
  unsigned long t0;
  t0 = millis();

  while ((millis() - t0) < answerPeriod) {
    if (digitalRead(LeftLickPin) == HIGH) {
      answer = 0;
      responseTime = millis();
      break;
    }
    if (digitalRead(RightLickPin) == HIGH) {
      answer = 1;
      responseTime = millis();
      break;
    }
  }
  //  Serial.print(millis() - t0);
  //  Serial.println(" ms passed before answering!");
  return answer;
}

// Reward Section
int rewardDelivery(int answer) {
  if (answer == 1) {

    digitalWrite(rightWaterPort, HIGH);
    delay(rightWaterValveDuration);
    digitalWrite(rightWaterPort, LOW);
  }
  else if (answer == 0) {
    delay(waterValveDelay);
    waterValveOpenTime = millis();
    digitalWrite(leftWaterPort, HIGH);
    delay(leftWaterValveDuration);
    digitalWrite(leftWaterPort, LOW);
  }
  return waterValveOpenTime;
}

// Punishment section, currently only use extra ITI.
int punishmentSection(int doExtraITI) {
  unsigned long ExtraITIStart;
  if (doExtraITI == 1) {
    ExtraITIStart = millis();
    while ((millis() - ExtraITIStart) < ExtraITIDur) {
    }
  }
  //      doExtraITI = 0;
}

void flash_leftLed() {

  digitalWrite(led_lick_left, HIGH);
  delayMicroseconds(500);
  digitalWrite(led_lick_left, LOW);
  lickCountLeft ++ ;
}

void flash_rightLed() {

  digitalWrite(led_lick_right, HIGH);
  delayMicroseconds(500);
  digitalWrite(led_lick_right, LOW);
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
  trialResultsStr += '/';
  trialResultsStr += "trNum=";
  trialResultsStr += String(trialCount + 1);
  trialResultsStr += '/';
  trialResultsStr += "trialType=";
  trialResultsStr += String(currentSide);
  trialResultsStr += '/';
  trialResultsStr += "trialStartTime=";
  trialResultsStr += String(trialStartTime);
  trialResultsStr += '/';
  trialResultsStr += "Stim_On_Time=";
  trialResultsStr += String(stimOnTime);
  trialResultsStr += '/';
  trialResultsStr += "Answer_Time=";
  trialResultsStr += String(responseTime);
  trialResultsStr += '/';
  trialResultsStr += "Reward_Time=";
  trialResultsStr += String(waterValveOpenTime);
  trialResultsStr += '/';
  trialResultsStr += "Choice=";
  trialResultsStr += String(answer);
  Serial.println(trialResultsStr);

  Serial.print("leftLickCounts:");
  Serial.println( lickCountLeft);

  Serial.print("rightLickCounts:");
  Serial.println( lickCountRight);

}


void setup() {
  delay(1000);
  SPI_TGM.init(MEGA2560);
  Serial.begin(9600);
  Serial.print("#");
  Serial.println(maxNumTrials);
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
  generate_random_sides(123);
}


void loop() {
  int stimDelay; // for the delay time before stimulus onset
  int answer;
  
  //  Serial.println(maxNumTrials);

  while (trialCount < maxNumTrials) {
    // Wait for the first trial to start
    if (trialCount < 1) {
      while (digitalRead(initButtonPin) == HIGH) {
      }
    }

    trialStartTime = millis();
    sendTrialStartPulse();
    // Put a random delay between 0.5 - 1 seconds before stimulus
    stimDelay = 500 + random(1000);
    delay(stimDelay);

    if (isRandomSides == 1) {
      currentSide = trialSides[trialCount];
    }
    else {
      currentSide = userDefinedSide;
    }
    // Right side trial
    if (currentSide == 1) {
      stimOnTime = millis();
      stimulusDelivery(1);
      answer = getAnswers(answerPeriod);

      if (answer == 1) {
        delay(waterValveDelay);
        rewardDelivery(answer);
        waterValveOpenTime = millis();
      }
      else if (answer == 0) {
        // Wrong answer, enter grace period
        int gracePeriod_onset = millis();
        int gracePeriod_pass = 0;
        while (gracePeriod_pass < gracePeriod) {
          answer = getAnswers(gracePeriod);
          gracePeriod_pass = millis() - gracePeriod_onset;
          if (answer == 1) {
            break;
          }
        }
        if (answer == 1) {
          delay(waterValveDelay);
          rewardDelivery(answer);
          waterValveOpenTime = millis();
        }
        // Wrong side!
       digitalWrite(punishment_led, HIGH);
       delay(punishmentDur);
       digitalWrite(punishment_led,LOW);

        // punishmentSection(doExtraITI);
      }
      else if (answer == 2) {
        // Miss! Do nonthing.
      }
    }

    // Left side trial
    else if (currentSide == 0) {
      stimOnTime = millis();
      stimulusDelivery(0);
      answer = getAnswers(answerPeriod);
      if (answer == 0) {
        // Correct side!
        delay(waterValveDelay);
        rewardDelivery(answer);
        waterValveOpenTime = millis();
      }
      else if (answer == 1) {
         // Wrong answer, enter grace period
        int gracePeriod_onset = millis();
        int gracePeriod_pass = 0;
        while (gracePeriod_pass < gracePeriod) {
          answer = getAnswers(gracePeriod);
          gracePeriod_pass = millis() - gracePeriod_onset;
          if (answer == 0) {
            break;
          }
        }
        if (answer == 0) {
          delay(waterValveDelay);
          rewardDelivery(answer);
          waterValveOpenTime = millis();
        }
        // Wrong side!
        digitalWrite(punishment_led, HIGH);
        delay(punishmentDur);
        digitalWrite(punishment_led,LOW);
        // punishmentSection(doExtraITI);
      }
      else if (answer == 2) {
        // Miss! Do nonthing.
      }
    }

    sendOutResults();

    trialCount++;
    delay(interTrialInterval);
  }
}

// =================================================
// other functions




