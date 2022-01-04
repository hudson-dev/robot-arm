#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t shoulder_one = 0;
uint8_t shoulder_two = 1;

uint8_t elbow = 2;

uint8_t wrist_one = 3;
uint8_t wrist_two = 4;

uint8_t gripper = 5;

int openAngle = 180;
int closeAngle = 52;

int min = 150;
int max = 640;

int i = 0;

int input_angle = 0;
int readAlready = 0;

void setup() {
  Serial.begin(9600);
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 
  delay(10);
  
}

void moveToAngle(uint8_t servoNumber, int angle) {
  if(angle > 180) {
    return;
  }

  int pulse = map(angle, 0, 180, min, max);
  pwm.setPWM(servoNumber, 0, pulse);
}

void moveShoulder(int angle) {
    int angle_one = 180-angle;
    int angle_two = 0+angle;
  
    moveToAngle(shoulder_one, angle_one);
    moveToAngle(shoulder_two, angle_two);

    delay(700);
}

void moveElbow(int angle) {
  moveToAngle(elbow, angle);
  delay(500);
}

void moveWristOne(int angle) {
  moveToAngle(wrist_one, angle);
  delay(500);
}

void moveWristTwo(int angle) {
  moveToAngle(wrist_two, angle);
  delay(500);
}


void moveGripper(int angle) {
  moveToAngle(gripper, angle);
  delay(500);
}

void openGripper() {
  moveToAngle(gripper, openAngle);
  delay(500);
}

void closeGripper() {
  moveToAngle(gripper, closeAngle);
  delay(500);
}

void inputAngle() {
  if (Serial.available() != 0 ) {
    input_angle = Serial.parseInt();
    if(input_angle == 0) {} 
    else {
      moveToAngle(5, input_angle);
//       moveShoulder(input_angle);
    }
  }
}

 
void loop() { 
  inputAngle();
}
