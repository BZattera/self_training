// importing libraries
#include <NewPing.h>

// include the library code:
#include <LiquidCrystal.h>

// include clock library
#include <Wire.h>
#include <ds3231.h>
struct ts t;



//include libraries for the SD module
#include <SPI.h>
#include <SD.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(32, 30, 40, 38, 36, 34);


// define button pin

int inPin1 = 2; // choose the pin for the button 1
int inPin2 = 3; // choose the pin for the button 2
int inPin3 = 4; // choose the pin for the button 3

// define syringe pump pin
int speed_reward = 28;
int pin1_rew = 26;
int pin2_rew = 24;

int drain_button = 22; // select pin for the drain


// previous pin state for monkey button
byte oldSwitchState_pin1 = LOW;  // assume switch open because of pull-up resistor

byte oldSwitchState_pin2 = LOW;  // assume switch open because of pull-up resistor

byte oldSwitchState_pin3 = LOW;  // assume switch open because of pull-up resistor



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

#define reedPin_starting_pos 44

// DC motor
#define enA 19
#define IN1 18
#define IN2 17

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
int counter2 = 0;
int aState2;
int aLastState2;

//check for the previous distance
int previous_distance;

// variables for the rotatory encoder  4
int counter3 = 0;
int aState3;
int aLastState3;

// loop for the distance
int var = 0;
int closure_time = 0;
int total_push = 0;
int var_closure = 0;

// button flags
bool reward_on_proximity_flag = false;
bool reward_on_proximity;
byte oldSwitchState_reward = HIGH;  // assume switch open because of pull-up resistor

bool door_closure_flag = false;
bool door_closure;
byte oldSwitchState_door = HIGH;  // assume switch open because of pull-up resistor

bool button_state = false; // flag for the start

// SD card
const int chipSelect = 53;

// define clock variables
int t0;
int t1;
int t2;
int t3;
int t4;
int t5;
int t6;
int t7;
int t8;

int motor_activity = 25 ;

// flag for the monkey presence
int monkey_already_in = false;

// control the presence of the monkey in the current position



// definint setup
void setup() {

  // clock
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  t.hour = 12;
  t.min = 30;
  t.sec = 0;


  DS3231_set(t);


  // drain button
  pinMode(drain_button, INPUT);


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
  pinMode(speed_reward, OUTPUT);
  pinMode(pin1_rew, OUTPUT);
  pinMode(pin2_rew, OUTPUT);


  pinMode(inPin1, INPUT);
  pinMode(inPin2, INPUT);
  pinMode(inPin3, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode( reedPin, INPUT);
  pinMode (reedPin_starting_pos, INPUT);

  pinMode(switch_Pin_black, INPUT_PULLUP);
  pinMode(switch_Pin_yellow, INPUT_PULLUP);
  pinMode(start_button, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {

    // don't do anything more:
    while (1);
  }


}




void loop() {

  // DEFINITION OF VARIABLES SECTION


  while (button_state != true) {
    // enable drain if button is pushed
    int drain_state = digitalRead(drain_button);  // reading the state of the drain button
    if (drain_state == HIGH) {
      analogWrite(speed_reward, 255);
      digitalWrite(pin1_rew, LOW);
      digitalWrite(pin2_rew, HIGH);
    } else {
      analogWrite(speed_reward, 0);
      digitalWrite(pin1_rew, LOW);
      digitalWrite(pin2_rew, LOW);
    }


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
      oldSwitchState_reward =  switchState;
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
        counter2 = counter2 + 5;
      } else {
        counter2 = counter2 - 5;
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
        counter3 = counter3 + 1;
      } else {
        counter3 = counter3 - 1;
      }
      lcd.clear();
      lcd.print("Closure time: ");
      lcd.print(counter3);

    }
    aLastState3 = aState3; // Updates the previous state of the outputA with the current state
    closure_time = counter3;




    if (start == HIGH) {

      button_state = true;

      File dataFile = SD.open(monkey + ".csv", FILE_WRITE);


      if (dataFile) {
        dataFile.println("New day of training");
        dataFile.println(rew_string);
        dataFile.println(back_door);
        dataFile.println(ITI);
        dataFile.println(proximity_threshold);
        dataFile.println(reward_duration);
        dataFile.println(closure_time);
        dataFile.close();


      }
      // if the file isn't open, pop up an error:
      else {
        lcd.print("SD card ERROR");
      }
      break;
    }

  }



  ///////////////////////////////////////////////////////////////////////////////// HERE THE CODE FOR THE EXPERIMENT BEGINS ///////////////////////////////////////////////////////////////////////////////////////////
  // code for the experiment

  while (door_closure_flag == false) {
    oldSwitchState_pin1 = digitalRead(inPin1);   // current state of pushbotton 1
    oldSwitchState_pin2 = digitalRead(inPin2);   // current state of pushbotton 2
    oldSwitchState_pin3 = digitalRead(inPin3);   // current state of pushbotton 3

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Waiting for ");
    lcd.print(monkey);
    //delay(1000);
    lcd.clear();
    while (reward_state == false) {
      File dataFile = SD.open(monkey + ".csv", FILE_WRITE);

      unsigned int distance = sonar.ping_cm(); // proximity sensor
      val1 = digitalRead(inPin1);   // current state of pushbotton 1
      val2 = digitalRead(inPin2);   // current state of pushbotton 2
      val3 = digitalRead(inPin3);   // current state of pushbotton 3

      // if button 1 is pressed, reward is delivered
      if (val1 == HIGH && val2 == LOW && val3 == LOW) {

        if (val1 != oldSwitchState_pin1) {
          oldSwitchState_pin1 =  val1;
          dataFile.println("button1");
          dataFile.close();
          analogWrite(speed_reward, 255);
          digitalWrite(pin1_rew, HIGH);
          digitalWrite(pin2_rew, LOW);
          analogWrite(12, 50); // playing a sound to strength the association
          delay (reward_duration);
          reward_state = true;

          break;
        } else {

          break;
        }
      }

      // if button 2 is pressed, reward is delivered
      else if (val1 == LOW && val2 == HIGH && val3 == LOW) {

        if (val2 != oldSwitchState_pin2) {
          oldSwitchState_pin2 =  val2;
          dataFile.println("button2");
          dataFile.close();
          analogWrite(speed_reward, 255);
          digitalWrite(pin1_rew, HIGH);
          digitalWrite(pin2_rew, LOW);
          analogWrite(12, 50); // playing a sound to strength the association
          delay (reward_duration);
          reward_state = true;

          break;
        } else {

          break;
        }
      }
      // if button 3 is pressed, reward is delivered
      else if (val1 == LOW && val2 == LOW && val3 == HIGH) {
        if (val3 != oldSwitchState_pin3) {
          oldSwitchState_pin3 =  val3;

          dataFile.println("button3");
          dataFile.close();
          analogWrite(speed_reward, 255);
          digitalWrite(pin1_rew, HIGH);
          digitalWrite(pin2_rew, LOW);
          analogWrite(12, 50); // playing a sound to strength the association
          delay (reward_duration);
          reward_state = true;
          break;
        } else {


          break;
        }
      }

      // if the monkey is near AND WE WANT TO DELIVER REWARD ON THE PROXIMITY (JUST FOR THE FIRST TIMES, reward is delivered)
      else if (distance <= proximity_threshold && distance > 0 && val1 == LOW && val2 == LOW && val3 == LOW && reward_on_proximity_flag == true && previous_distance > proximity_threshold) {

        for (int var = 0; var < 10; var++) {

          delay(closure_time / 10);
          if (distance > proximity_threshold)
            analogWrite(speed_reward, 0);
          digitalWrite(pin1_rew, LOW);
          digitalWrite(pin2_rew, LOW);

        }
        analogWrite(speed_reward, 255);
        digitalWrite(pin1_rew, HIGH);
        digitalWrite(pin2_rew, LOW);

        delay (reward_duration);
        analogWrite(speed_reward, 0);
        digitalWrite(pin1_rew, LOW);
        digitalWrite(pin2_rew, LOW);
        dataFile.println("reward for proximity");
        previous_distance = distance;
        dataFile.println(int (distance));
        dataFile.close();
        reward_state = true;
        break;
      }
      else {
        analogWrite(speed_reward, 0);
        digitalWrite(pin1_rew, LOW);
        digitalWrite(pin2_rew, LOW);
        digitalWrite(12, LOW);
        previous_distance = distance;

      }

    }




    analogWrite(speed_reward, 0);
    digitalWrite(pin1_rew, LOW);
    digitalWrite(pin2_rew, LOW);
    digitalWrite(12, LOW);
    // inter trial interval
    delay(ITI);

    // idle flag
    reward_state = false;
    int var = 0;
    lcd.clear();

  }

  while (door_closure_flag == true) {

    lcd.clear();
    File dataFile = SD.open(monkey + ".csv", FILE_WRITE);
    
    unsigned int distance = sonar.ping_cm(); // proximity sensorc
    oldSwitchState_pin1 = digitalRead(inPin1);   // current state of pushbotton 1
    oldSwitchState_pin2 = digitalRead(inPin2);   // current state of pushbotton 2
    oldSwitchState_pin3 = digitalRead(inPin3);   // current state of pushbotton 3

    val1 = digitalRead(inPin1);   // current state of pushbotton 1
    val2 = digitalRead(inPin2);   // current state of pushbotton 2
    val3 = digitalRead(inPin3);   // current state of pushbotton 3

    if (val1 == HIGH || val2 == HIGH || val3 == HIGH && distance <= proximity_threshold && distance > 0) {
      DS3231_get(&t);
      int t0 = t.sec;
      Serial.print(t2);
      
      while (t2 < motor_activity) {
        DS3231_get(&t);
        int t1 = t.sec;
        int t2 = abs(t1 - t0);
        Serial.print(t2);
        analogWrite(enA, 255);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(speed_reward, 80);
        digitalWrite(pin1_rew, HIGH);
        digitalWrite(pin2_rew, LOW);
        if (t2 > motor_activity) {
          
          break;
        }

      }
      analogWrite(enA, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(speed_reward, 0);
      digitalWrite(pin1_rew, LOW);
      digitalWrite(pin2_rew, LOW);
      dataFile.println("door closed");
      dataFile.close();

      DS3231_get(&t);
      int t3 = t.sec;

      while (t5 < closure_time) {
        DS3231_get(&t);
        int t4 = t.sec;
        int t5 = abs(t4 - t3);
        while (reward_state == false) {
          unsigned int distance = sonar.ping_cm(); // proximity sensor
          val1 = digitalRead(inPin1);   // current state of pushbotton 1
          val2 = digitalRead(inPin2);   // current state of pushbotton 2
          val3 = digitalRead(inPin3);   // current state of pushbotton 3

          // if button 1 is pressed, reward is delivered
          if (val1 == HIGH && val2 == LOW && val3 == LOW) {

            
            dataFile.println("button1");
            dataFile.close();
            analogWrite(speed_reward, 255);
            digitalWrite(pin1_rew, HIGH);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 50); // playing a sound to strength the association
            delay (reward_duration);
            analogWrite(speed_reward, 0);
            digitalWrite(pin1_rew, LOW);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 0);
            reward_state = true;

            break;

          }

          // if button 2 is pressed, reward is delivered
          else if (val1 == LOW && val2 == HIGH && val3 == LOW) {

            dataFile.println("button2");
            dataFile.close();
            analogWrite(speed_reward, 255);
            digitalWrite(pin1_rew, HIGH);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 50); // playing a sound to strength the association
            delay (reward_duration);
            analogWrite(speed_reward, 0);
            digitalWrite(pin1_rew, LOW);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 0);
            reward_state = true;

            break;

          }
          // if button 3 is pressed, reward is delivered
          else if (val1 == LOW && val2 == LOW && val3 == HIGH) {


            dataFile.println("button3");
            dataFile.close();
            analogWrite(speed_reward, 255);
            digitalWrite(pin1_rew, HIGH);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 50); // playing a sound to strength the association
            delay (reward_duration);
            analogWrite(speed_reward, 0);
            digitalWrite(pin1_rew, LOW);
            digitalWrite(pin2_rew, LOW);
            analogWrite(12, 0);
            reward_state = true;
            break;

          } else {
            break;
          }

        }
        // idleing reward variables
        reward_state = false;
        if (t5 > closure_time) {
          break;
        }
      }
      DS3231_get(&t);
      int t6 = t.sec;

      // opening the door
      while (t8 < (motor_activity )) {
        DS3231_get(&t);
        int t7 = t.sec;
        int t8 = abs(t7 - t6);


        analogWrite(enA, 255);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        if (t8 > motor_activity) {
          break;
        }

      }

      dataFile.println("door opened");
      dataFile.close();
      analogWrite(enA, 255);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      

    }




    dataFile.close();
    // idle flag
    reward_state = false;
    int t2 = 0;
    int t5 = 0;
    int t8 = 0;
    
    int motor_activity = 25;
    
    lcd.clear();
    // inter trial interval
    delay(ITI);

  }




}
