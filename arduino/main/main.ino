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

// communication

String input;
String identifier;
String remaining;

String x;
String y;
String z;

String last;
String trailing;

int min = 150;
int max = 640;

int i = 0;

// tracking (CAMERA SHOULD BE POINTING TO PERSON, NOT CEILING!)

void setup() {
  Serial.begin(2000000);
  Serial.setTimeout(1);
  
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

void open() {
  openGripper();
}

void close() {
  closeGripper();
}

void right() {
  Serial.println("RIGHT");
}

void left() {
  Serial.println("LEFT");
}

void position(String x, String y, String z) {
  
  // x movement
  if(x > origin_x + circle_offset) {
    right(); // motor
  }
  if(y < origin_x - circle_offset) {
    left(); // motor
  }
  
  // y movement
  if(y > origin_y + circle_offset) {
    up(); // shoulder
  }
  if(y < origin_y - circle_offset) {
    down(); // shoulder
  }

  // z movement
  if(z > origin_z + circle_offset) {
    forward(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
  if(y < origin_z - circle_offset) {
    backward(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
}

void loop() { 
 while (!Serial.available());
 input = Serial.readString();
 identifier = input.substring(0,3);
 remaining = input.substring(3);
 
 char Buf[50];
 remaining.toCharArray(Buf, 50);

 // get origin (x,y,z) after delay
 
 if(identifier == "xyz") {
  x = String(strtok(Buf, ","));
  y = String(strtok(NULL, ","));

  last = String(strtok(NULL, ","));
  z = last.substring(0,3);
  trailing = last.substring(3);

  position(x,y,z);

  if(trailing == "ope") {
    open();
  }

  if(trailing == "clo") {
    close();
  }

  if(trailing == "rig") {
    right_wrist(); // rotate wrist
  }

  if(trailing == "lef") {
    left_wrist(); // rotate wrist
  }
 }
}
