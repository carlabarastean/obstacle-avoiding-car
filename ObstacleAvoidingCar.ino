#include <Servo.h>          
#include <NewPing.h>        

const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 5;
const int RightMotorBackward = 4;


#define trig_pin A1 
#define echo_pin A2 

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;

NewPing sonar(trig_pin, echo_pin, maximum_distance); 
Servo servo_motor; 


const int buttonPin = 2; 
boolean robotEnabled = false; 
boolean lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;   

void setup() {
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
  servo_motor.attach(10); 
  servo_motor.write(115);
  delay(2000);

  pinMode(buttonPin, INPUT_PULLUP);

}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading == LOW) {
        robotEnabled = !robotEnabled;
        if (robotEnabled) {
          moveForward();
        } else {
          moveStop(); 
        }
      }
      lastDebounceTime = millis();
    }
  }
  lastButtonState = reading;

if (robotEnabled) {
    distance = readPing();

    if (distance <= 35) {
      moveStop();
      delay(300);
      moveBackward();
      delay(400);
      moveStop();
      delay(300);
      int distanceRight = lookRight();
      delay(300);
      int distanceLeft = lookLeft();
      delay(300);

      if (distanceRight >= distanceLeft) {
        turnRight();
      } else {
        turnLeft();
      }
    } else {
      moveForward();
    }
  } else {
    moveStop(); 
  }
}

int lookRight(){  
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0){
    cm = 250;
  }
  return cm;
}

void moveStop(){
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW); 
}

void moveForward(){
  if(robotEnabled){ 
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
  
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW); 
  }
}

void moveBackward(){
  goesForward = false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
}

void turnRight(){
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(250);
  
  moveForward();
}

void turnLeft(){
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(250);
  
  moveForward();
}