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

// movement

int min = 150;
int max = 640;

int circle_offset = 200;

// origin

int sum_x = 0;
int sum_y = 0;
int sum_z = 0;

int origin_x = 0;
int origin_y = 0;
int origin_z = 0;

int total = 0;

// timer

unsigned long currentTime;
unsigned long startTime;

bool isStart = true;
bool startedTimer = false;

int delay_origin = 5000;

// Logging

bool printed = false;

void setup() {
  Serial.begin(2000000);
  Serial.setTimeout(1);
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 
  delay(10);
  
}

void printOnce(String statement) {
  if(printed == false) {
    Serial.println(statement);
    printed = true;
  }
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

void open() {
  moveToAngle(gripper, openAngle);
  delay(500);
}

void close() {
  moveToAngle(gripper, closeAngle);
  delay(500);
}

void right() {
  Serial.println("RIGHT");
}

void left() {
  Serial.println("LEFT");
}

void up() {
  Serial.println("UP");
}

void down() {
  Serial.println("DOWN");
}

void forward() {
  Serial.println("FORWARD");
}

void backward() {
  Serial.println("BACKWARD");
}

void position(int x, int y, int z) {
  
  // x movement
  if(x > origin_x + circle_offset) {
    left(); // motor
  }
  if(x < origin_x - circle_offset) {    
    right(); // motor
  }
  
  // y movement
  if(y > origin_y + circle_offset) {
    backward(); // shoulder
  }
  if(y < origin_y - circle_offset) {
    forward(); // shoulder
  }
  
  // z movement
  if(z > origin_z + circle_offset) {
    down(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
  if(z < origin_z - circle_offset) {
    up(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
}

void right_wrist() {}

void left_wrist() {}

void loop() { 
 while (!Serial.available()); // no data

 // data
 input = Serial.readString();
 identifier = input.substring(0,3);
 remaining = input.substring(3);
 
 char Buf[50];
 remaining.toCharArray(Buf, 50);

 // delay for origin

 if(isStart == true) {
  if(startedTimer == false) {
    startTime = millis();
    startedTimer = true;
  }
  currentTime = millis();
  if(currentTime - startTime < delay_origin) { // still in delay
    if(identifier == "xyz") {
      x = strtok(Buf, ",");
      y = strtok(NULL, ",");
      z = strtok(NULL, ",");

      Serial.println("XYZ: " + x + "," + y + "," + z);

      sum_x = sum_x + x.toInt();
      sum_y = sum_y + y.toInt();
      sum_z = sum_z + z.toInt();

      total++;

      delay(200);
    } 
  } else {
    origin_x = sum_x / total;
    origin_y = sum_y / total;
    origin_z = sum_z / total;
    
    Serial.println("sum_x: " + String(sum_x));
    Serial.println("sum_y: " + String(sum_y));
    Serial.println("sum_z: " + String(sum_z));

    Serial.println("total: " + String(total));

    Serial.println("origin_x: " + String(origin_x));
    Serial.println("origin_y: " + String(origin_y));
    Serial.println("origin_z: " + String(origin_z));

    Serial.println("SETTING ORIGINS");
    
    isStart = false;
  }
 } else {
   // START

    
   printOnce("---------------CALIBRATED!---------------");
   
   if(identifier == "xyz") {
    
    x = strtok(Buf, ",");
    y = strtok(NULL, ",");
    z = strtok(NULL, ",");
    
    trailing = strtok(NULL, ",");
  
    position(x.toInt(),y.toInt(),z.toInt());
  
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
}
