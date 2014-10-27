#define LED 13
#define leftWaterPort 11
#define rightWaterPort 9

#define LeftLickPin 2
#define RightLickPin 3
#define initButtonPin 8
#define StimOutPin 6

const int numTrials = 20;
const int waterValveDuration = 100; // 100 ms
const int waterValveDealy = 0;
const int drinkTime = 50;
const int timeOutTime = 2000;
const int SamplingTime = 500;
const int answerPeriod = 3000; // time when response can be accepted after stimulus.
const int timeOutInterval = 2000;
const int ExtraITIDur = 3000;
const int interTrialInterval = 2000;

unsigned long trialStartTime;
unsigned long stimOnTime;
unsigned long responseTime;
unsigned long waterValveOpenTime;

//int numTrials = 0;
int trialCount = 0;
int randNumber;
int doExtraITI;
int answer;

// Store trial sides to an array
int trialSides[500];

int trialStart = 0;
int trialEnd = 0;


void setup() {
  Serial.begin(9600);
  Serial.print("#");
  Serial.println(numTrials);
/*  // Wait for an serial input from Python program specifying the MaxNumTrials
  int incomingByte;   // for incoming serial data
  int count = 0;
  String trialNumStr = "";
  count = 0;
  
  while (count < 3) { 
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      trialNumStr += char(incomingByte);
      count = count + 1;
    }
  }
  char tempstr[trialNumStr.length()];
  trialNumStr.toCharArray(tempstr, trialNumStr.length()+1);
  numTrials = atoi(tempstr);
//  Serial.println(atoi(tempstr));
*/

  pinMode(leftWaterPort, OUTPUT);
  pinMode(rightWaterPort, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(StimOutPin, OUTPUT);
  pinMode(LeftLickPin, INPUT);
  pinMode(RightLickPin, INPUT);
  pinMode(initButtonPin, INPUT);

  digitalWrite(leftWaterPort, LOW);
  digitalWrite(rightWaterPort, LOW);

  
  // Use extra ITI as punishment
   doExtraITI = 1;

  randomSeed(123);
  for (int i = 0; i < numTrials; i++) {
    randNumber = random(0, 100); // put a random number between 0 and 100 to the array
    if (randNumber < 50) {
      trialSides[i] = 0;
    }
    else {
      trialSides[i] = 1;
    }
  }
  digitalWrite(LED, LOW);
}


void loop() {
  int randDelay; // for the delay time before stimulus onset
  int answer;
//  Serial.println(numTrials);
  
  while (trialCount < numTrials) {
    // Wait for the first trial to start
    if (trialCount < 1) {
      while (digitalRead(initButtonPin) == HIGH) {
      }
    }

    trialStartTime = millis();
    sendTrialStartPulse();
    //        Serial.print("Start time = ");
    //        Serial.print(trialStartTime);
    //        Serial.print("\tTrial#: ");
    //        Serial.println(trialCount);
    // Put a random delay between 0.5 - 1 seconds before stimulus
    randDelay = 500 + random(1000);
    delay(randDelay);

    // Right side trial
    if (trialSides[trialCount] == 1) {
      stimOnTime = millis();
      stimulusDelivery(1);
      answer = getAnswers(answerPeriod);

      if (answer == 1) {
        delay(waterValveDealy);
        rewardDelivery(answer);
        waterValveOpenTime = millis();
      }
      else if (answer == 0) {
        // Wrong side!
        punishmentSection(doExtraITI);
      }
      else if (answer == 2) {
        // Miss! Do nonthing.
      }
    }

    // Left side trial
    else if (trialSides[trialCount] == 0) {
      stimOnTime = millis();
      stimulusDelivery(0);
      answer = getAnswers(answerPeriod);
      if (answer == 0) {
        // Correct side!
        delay(waterValveDealy);
        rewardDelivery(answer);
        waterValveOpenTime = millis();
      }
      else if (answer == 1) {
        // Wrong side!
        punishmentSection(doExtraITI);
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

// Send a pulse to signal the start of trial.
int sendTrialStartPulse() {
  digitalWrite(LED, HIGH);
  delayMicroseconds(100);
  digitalWrite(LED, LOW);
  Serial.println("Trial Starts ...");
}

int stimulusDelivery(int sides) {
  int duration = 300;
  int toneFreq[2] = {1000, 2500};
  if (sides == 0) { // right trial
    // LED 13 blink 3 times
    //    digitalWrite(LED, HIGH);
    tone(StimOutPin, toneFreq[0], duration);
    //    delay(duration);
    //    digitalWrite(LED, LOW);
    //    delay(duration);
  }

  if (sides == 1) { // right trial
    //    digitalWrite(LED, HIGH);
    tone(StimOutPin, toneFreq[1], duration);
    //    delay(duration);
    //    digitalWrite(LED, LOW);
    //    delay(duration);
  }
}

int getAnswers(int answerPeriod) {
  answer = 2;
  unsigned long t0;
  t0 = millis();

  while (millis() - t0 < answerPeriod) {
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
    delay(waterValveDuration);
    digitalWrite(rightWaterPort, LOW);
  }
  else if (answer == 0) {
    delay(waterValveDealy);
    waterValveOpenTime = millis();
    digitalWrite(leftWaterPort, HIGH);
    delay(waterValveDuration);
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

void sendOutResults() {
  String trialResultsStr = String('o');
  trialResultsStr += '/';
  trialResultsStr += "trNum=";
  trialResultsStr += String(trialCount + 1);
  trialResultsStr += '/';
  trialResultsStr += "trialType=";
  trialResultsStr += String(trialSides[trialCount]);
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
}










