
// importing libraries
#include <NewPing.h>

// include the library code:
#include <LiquidCrystal.h>

//include libraries for the SD module
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(32, 30, 40, 38, 36, 34);


// define pin

int ledPin = 13; // choose the pin for the syringe pump
int inPin1 = 2; // choose the pin for the button 1
int inPin2 = 3; // choose the pin for the button 2
int inPin3 = 4; // choose the pin for the button 3


int start_button = 23; // green button

int switch_Pin_yellow = 27; // yellow button
int switch_Pin_black = 29; // black button

const int switchPin = 25; // white button
byte oldSwitchState = HIGH;  // assume switch open because of pull-up resistor
bool toggle;

// Rotary encoder 1 --> ITI
#define outputA 41
#define outputB 39


// Rotary encoder 2  --> distance threshold
#define outputA1 45
#define outputB1 43


// Rotary encoder 3 --> reward duration
#define outputA2 37
#define outputB2 35

// Rotary encoder 4 --> time of proximity threshold
#define outputA3 33
#define outputB3 31


// proximity sensor
#define TRIGGER_PIN 8
#define ECHO_PIN 9
#define MAX_DISTANCE 2000

// reed module
#define reedPin 10

// DC motor
#define enA 7
#define IN1 5
#define IN2 6

// NewPing setup of pins and maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// define variables
int val1 = 0;
int val2 = 0;
int val3 = 0;
int proximity_step = 1;
int proximity_threshold = 0;
int reward_duration = 0; //ms
bool reward_state = false; // flag for the reward state
int ITI = 5000; // Inter trial Interval
int ITI_step = 1000;
String monkey = "No monkey selected";
String rew_string = "Reward undefined";
String back_door = "No back door defined";

// variables for the rotatory encoder  1
int counter = 1000;
int aState;
int aLastState;

// variables for the rotatory encoder  2
int counter1 = 5;
int aState1;
int aLastState1;

// variables for the rotatory encoder  3 ---> reward duration
int counter2 = 500;
int aState2;
int aLastState2;


// variables for the rotatory encoder  4
int counter3 = 200;
int aState3;
int aLastState3;

// loop for the distance
int var = 0;
int prox_time = 0;

// button flags
bool reward_on_proximity_flag = false;
bool reward_on_proximity;
byte oldSwitchState_reward = HIGH;  // assume switch open because of pull-up resistor

bool door_closure_flag = false;
bool door_closure;
byte oldSwitchState_door = HIGH;  // assume switch open because of pull-up resistor

bool button_state = false; // flag for the start
File myFile;

// distance counter
int distance_counter;

// flag for the monkey presence
int monkey_already_in = false;


// definint setup
void setup() {

  SD.begin();

  // rotary encoder 1
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);
  aLastState = digitalRead(outputA);


  // rotary encoder 2
  pinMode (outputA1, INPUT);
  pinMode (outputB1, INPUT);
  aLastState1 = digitalRead(outputA1);

  // rotary encoder 3
  pinMode (outputA2, INPUT);
  pinMode (outputB2, INPUT);
  aLastState2 = digitalRead(outputA2);

  // rotary encoder 4
  pinMode (outputA3, INPUT);
  pinMode (outputB3, INPUT);
  aLastState3 = digitalRead(outputA3);

  pinMode (switchPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(inPin1, INPUT);
  pinMode(inPin2, INPUT);
  pinMode(inPin3, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode( reedPin, INPUT);

  pinMode(switch_Pin_black, INPUT_PULLUP);
  pinMode(switch_Pin_yellow, INPUT_PULLUP);
  pinMode(start_button, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}




void loop() {
  myFile = SD.open("test.txt");
  // DEFINITION OF VARIABLES SECTION


  while (button_state != true) {
    int start = digitalRead(start_button);  // reading the state of the green button
    byte switchState = digitalRead (switchPin); // reading the state of the white button
    byte switchState_yellow = digitalRead (switch_Pin_yellow); // reading the state of the yellow button
    byte switchState_black = digitalRead (switch_Pin_black);// reading the state of the black button

    aState = digitalRead(outputA); // Reads the "current" state of the outputA
    aState1 = digitalRead(outputA1); // Reads the "current" state of the outputA1
    aState2 = digitalRead(outputA2); // Reads the "current" state of the outputA2
    aState3 = digitalRead(outputA3); // Reads the "current" state of the outputA3


    // Choosing if we want to deliver the reward on the proxomity
    if (switchState == HIGH) {
      oldSwitchState_door =  switchState;
      reward_on_proximity = !reward_on_proximity;   // toggle the variable
      if (reward_on_proximity == 0) {
        lcd.clear();
        rew_string = String("No reward on prox");
        reward_on_proximity_flag = false;
        lcd.setCursor(0, 1);

      } else if (reward_on_proximity == 1) {
        lcd.clear();
        rew_string = String("Reward on prox");
        reward_on_proximity_flag = true;
        lcd.setCursor(0, 1);
      }

      lcd.print(rew_string);

    }


    // selecting the monkey
    if (switchState_yellow == HIGH) {
      oldSwitchState =  switchState_yellow;
      toggle = !toggle;   // toggle the variable
      if (toggle == 0) {
        lcd.clear();
        monkey = String("Larry");
        lcd.setCursor(0, 1);


      } else if (toggle == 1) {
        lcd.clear();
        lcd.setCursor(0, 1);
        monkey = String("Alan");

      }

      lcd.print(monkey);

    }

    // Chhosing if we want to activate the back door
    if (switchState_black == HIGH) {
      oldSwitchState_door =  switchState_black;
      door_closure = !door_closure;   // toggle the variable
      if (door_closure == 0) {
        lcd.clear();
        back_door = String("Door inactivated");
        lcd.setCursor(0, 1);
        door_closure_flag = false;


      } else if (door_closure == 1) {
        lcd.clear();
        lcd.setCursor(0, 1);
        back_door = String("Door actived");
        door_closure_flag = true;
      }

      lcd.print(back_door);

    }



    // If the previous and the current state of the outputA are different, that means a Pulse has occured
    if (aState != aLastState) {
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB) != aState) {
        counter = counter + 100;
      } else {
        counter = counter - 100;
      }
      lcd.clear();
      lcd.print("ITI: ");
      lcd.print(counter);
      lcd.print(" ms");
    }
    aLastState = aState; // Updates the previous state of the outputA with the current state
    ITI = counter;




    // rotary encoder for the proximity threshold
    if (aState1 != aLastState1) {
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(outputB1) != aState1) {
        counter1 ++;
      } else {
        counter1 --;
      }
      lcd.clear();
      lcd.print("distance: ");
      lcd.print(counter1);
      lcd.print(" cm");
    }
    aLastState1 = aState1; // Updates the previous state of the outputA with the current state
    proximity_threshold = counter1;


    // If the previous and the current state of the outputA2 are different, that means a Pulse has occured
    if (aState2 != aLastState2) {
      // If the outputB state is different to the outputA2 state, that means the encoder is rotating clockwise
      if (digitalRead(outputB2) != aState2) {
        counter2 = counter2 + 100;
      } else {
        counter2 = counter2 - 100;
      }
      lcd.clear();
      lcd.print("Reward : ");
      lcd.print(counter2);
      lcd.print(" ms");
    }
    aLastState2 = aState2; // Updates the previous state of the outputA with the current state
    reward_duration = counter2;


    // If the previous and the current state of the outputA2 are different, that means a Pulse has occured
    if (aState3 != aLastState3) {
      // If the outputB state is different to the outputA2 state, that means the encoder is rotating clockwise
      if (digitalRead(outputB3) != aState3) {
        counter3 = counter3 + 100;
      } else {
        counter3 = counter3 - 100;
      }
      lcd.clear();
      lcd.print("Prox. time: ");
      lcd.print(counter3);

    }
    aLastState3 = aState3; // Updates the previous state of the outputA with the current state
    prox_time = counter3;




    if (start == HIGH) {
      myFile.println (rew_string);
      myFile.println (monkey);
      myFile.println (back_door);
      myFile.println (ITI);
      myFile.println (proximity_threshold);
      myFile.println (reward_duration);
      myFile.println (prox_time);
      myFile.close();
      button_state = true;
      break;
    }

  }



  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Waiting for ");
  lcd.print(monkey);
  delay(1000);
  lcd.clear();

  ///////////////////////////////////////////////////////////////////////////////// HERE THE CODE FOR THE EXPERIMENT BEGINS ///////////////////////////////////////////////////////////////////////////////////////////
  // code for the experiment
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Waiting for ");
  lcd.print(monkey);
  delay(1000);
  lcd.clear();
  while (door_closure_flag == false) {
    while (reward_state == false) {
      unsigned int distance = sonar.ping_cm(); // proximity sensor
      val1 = digitalRead(inPin1);   // current state of pushbotton 1
      val2 = digitalRead(inPin2);   // current state of pushbotton 2
      val3 = digitalRead(inPin3);   // current state of pushbotton 3

      // if button 1 is pressed, reward is delivered
      if (val1 == HIGH && val2 == LOW && val3 == LOW) {
        Serial.println("button1");
        Serial.println(digitalRead(inPin1));
        digitalWrite (ledPin, HIGH);
        delay (reward_duration);
        reward_state = true;

        break;
      }

      // if button 2 is pressed, reward is delivered
      else if (val1 == LOW && val2 == HIGH && val3 == LOW) {
        Serial.println("button2");
        digitalWrite (ledPin, HIGH);
        delay (reward_duration);
        reward_state = true;

        break;
      }
      // if button 3 is pressed, reward is delivered
      else if (val1 == LOW && val2 == LOW && val3 == HIGH) {
        Serial.println("button3");
        digitalWrite (ledPin, HIGH);
        delay (reward_duration);

        reward_state = true;

        break;
      }

      // if the monkey is near AND WE WANT TO DELIVER REWARD ON THE PROXIMITY (JUST FOR THE FIRST TIMES, reward is delivered)
      else if (distance <= proximity_threshold && distance > 0 && val1 == LOW && val2 == LOW && val3 == LOW && reward_on_proximity_flag == true) {
        for (int var = 0; var < 10; var++) {
          delay(prox_time / 10);
          if (distance > proximity_threshold)
            digitalWrite (ledPin, LOW);

        }
        digitalWrite (ledPin, HIGH);
        delay (reward_duration);
        reward_state = true;
        break;
      }
      else {
        digitalWrite (ledPin, LOW);

      }
    }

    digitalWrite (ledPin, LOW);
    // inter trial interval
    delay(ITI);

    // idle flag
    reward_state = false;
    int var = 0;
    lcd.clear();

  }
  while (door_closure_flag == true) {
    unsigned int distance = sonar.ping_cm(); // proximity sensor
    val1 = digitalRead(inPin1);   // current state of pushbotton 1
    val2 = digitalRead(inPin2);   // current state of pushbotton 2
    val3 = digitalRead(inPin3);   // current state of pushbotton 3

    if (distance <= proximity_threshold && distance > 0 ) {
      Serial.println("you entered the section where the monkey is near to the proximity sensor");
      while (var < 10) {
        var++;
        if (distance > proximity_threshold) {
          return;
        }

        delay(prox_time / 10);
      }

      Serial.println("the monkey is in the right position from a sufficient amount of time: let's start with the door closure");
      if (digitalRead(reedPin) == HIGH && distance <= proximity_threshold) {
        Serial.println("if the monkey is in the right position and the reed is not reached, keep moving the door");
        Serial.println (digitalRead(reedPin));
        distance_counter = distance_counter+200;
        analogWrite(enA, 100);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);

      }

      else if (digitalRead(reedPin) == LOW && (distance <= proximity_threshold)) {
        // the monkey is still here when the motor has reached the reed module, so I stop the motor and
        // give the reward
        analogWrite(enA, 0);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite (ledPin, HIGH);
        delay(reward_duration);
        digitalWrite (ledPin, LOW);
        monkey_already_in = true;


        while (monkey_already_in == true) {
          while (reward_state == false) {

            //unsigned int distance = sonar.ping_cm(); // proximity sensor
            distance = sonar.ping_cm(); // proximity sensor
            val1 = digitalRead(inPin1);   // current state of pushbotton 1
            val2 = digitalRead(inPin2);   // current state of pushbotton 2
            val3 = digitalRead(inPin3);   // current state of pushbotton 3

            // now the monkey is in the right position, and can start to press the buttons
            Serial.println("stato 1");
            // if button 1 is pressed, reward is delivered
            if (val1 == HIGH && val2 == LOW && val3 == LOW) {
              Serial.println("button 1 pressed");
              digitalWrite (ledPin, HIGH);
              delay (reward_duration);
              reward_state = true;

              break;

            }

            //if button 2 is pressed, reward is delivered
            else if (val1 == LOW && val2 == HIGH && val3 == LOW) {
              Serial.println("button 2 pressed");
              digitalWrite (ledPin, HIGH);
              delay (reward_duration);
              reward_state = true;

              break;

            }
            // if button 3 is pressed, reward is delivered
            else if (val1 == LOW && val2 == LOW && val3 == HIGH) {
              Serial.println("button 3 pressed");
              digitalWrite (ledPin, HIGH);
              delay (reward_duration);
              reward_state = true;

              break;
            } else if (distance > proximity_threshold) {
              delay (50);
            Serial.println("coming back2");
            
           
            break;
            }


          }

          if (distance > proximity_threshold) {
            delay (50);
            Serial.println("coming back");
            analogWrite(enA, 100);
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            delay (distance_counter);
           
            monkey_already_in = false;
            break;
          }
          // idle flag
          reward_state = false;
          delay(ITI);
          digitalWrite (ledPin, LOW);
          

        }

      } else if (distance > proximity_threshold) {
        Serial.println("stato 3");
        delay (50);
        analogWrite(enA, 100);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        delay (distance_counter);
        
        break;
      }


    }

    else {
      analogWrite(enA, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      Serial.println("stato 4");


    }
  }
  // idle flag
  reward_state = false;
  int var = 0;
  lcd.clear();
  // inter trial interval
  delay(ITI);
  monkey_already_in = false;
   int distance_counter = 0;


}
