#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// ports

uint8_t shoulder_one = 0;
uint8_t shoulder_two = 1;

uint8_t elbow = 2;

uint8_t wrist_one = 3;
uint8_t wrist_two = 4;

uint8_t gripper = 5;

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

int circle_offset = 190;
int up_offset = 280;
int forward_backward_offset = 220;

bool isWrist = false;

// starting angles (Create angles (start, min, max, current) --> set starting angles --> create function)

int shoulder_start_angle = 140;
int elbow_start_angle = 150;
int wrist_one_start_angle = 30;
int wrist_two_start_angle = 68;

// min & max angles

int shoulder_min = 40; // up
int shoulder_max = 140; // down
int current_shoulder_angle = 0;

int elbow_min = 20; // back
int elbow_max = 180; // forward
int current_elbow_angle = 0;

int wrist_one_min = 0; // forward
int wrist_one_max = 100; // back
int current_wrist_one_angle = 0;

int wrist_two_min = 0; // left
int wrist_two_max = 180; // right
int current_wrist_two_angle = 0;

int gripper_min = 40; // close
int gripper_max = 180; // open

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

int delay_origin = 3000;

// Logging

bool printed = false;

// motor

const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;

void setup() {
  Serial.begin(2000000);
  Serial.setTimeout(1);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 
  delay(10);

  // moving arm to starting position
  
  moveShoulder(shoulder_start_angle);
  current_shoulder_angle = shoulder_start_angle;

  delay(3000);

  moveToAngle(2, elbow_start_angle);
  current_elbow_angle = elbow_start_angle;

  delay(1000);
  
  moveToAngle(3, wrist_one_start_angle);
  current_wrist_one_angle = wrist_one_start_angle;

  delay(1000);

  moveToAngle(4, wrist_two_start_angle);
  current_wrist_two_angle = wrist_two_start_angle;

  delay(1000);
  moveToAngle(5, gripper_min);
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

void control(int input_angle, int min_angle, int max_angle, int servo_number, bool isShoulder) {
  if(isShoulder == false) {
    if(input_angle >= min_angle && input_angle <= max_angle) {
      moveToAngle(servo_number, input_angle);
      delay(10);
    }
  } else {
    if(input_angle >= min_angle && input_angle <= max_angle) {
      moveShoulder(input_angle);
      delay(700);
    }
  }
}

void moveShoulder(int angle) {
    int angle_one = 180-angle;
    int angle_two = 0+angle;
  
    moveToAngle(shoulder_one, angle_one);
    moveToAngle(shoulder_two, angle_two);
}

void open() {
  moveToAngle(gripper, gripper_max);
  delay(500);
}

void close() {
  moveToAngle(gripper, gripper_min);
  delay(500);
}

void spin(bool turnRight) {
  if(turnRight == true) {
    // Set motor direction clockwise
    digitalWrite(dirPin, HIGH);
  } else {
    // Set motor direction counterclockwise
    digitalWrite(dirPin, LOW);
  }

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); // Wait a second
}

void right() {
  Serial.println("RIGHT");
  spin(true);
}

void left() {
  Serial.println("LEFT");
  spin(false);
}

void up() {
  Serial.println("UP");
  current_shoulder_angle = current_shoulder_angle - 30;
  control(current_shoulder_angle, shoulder_min, shoulder_max, 0, true);
}

void down() {
  Serial.println("DOWN");
  current_shoulder_angle = current_shoulder_angle + 30;
  control(current_shoulder_angle, shoulder_min, shoulder_max, 0, true);
}

void forward() {
  Serial.println("FORWARD");
  current_elbow_angle = current_elbow_angle + 30;
  control(current_elbow_angle, elbow_min, elbow_max, 2, false);
}

void backward() {
  Serial.println("BACKWARD");
  current_elbow_angle = current_elbow_angle - 30;
  control(current_elbow_angle, elbow_min, elbow_max, 2, false);
}

void right_wrist() {
  Serial.println("RIGHT WRIST");
  current_wrist_two_angle = current_wrist_two_angle + 30;
  control(current_wrist_two_angle, wrist_two_min, wrist_two_max, 2, false);
}

void left_wrist() {
  Serial.println("LEFT WRIST");
  current_wrist_two_angle = current_wrist_two_angle - 30;
  control(current_wrist_two_angle, wrist_two_min, wrist_two_max, 2, false);
}

void position(int x, int y, int z, String trailing) {
  
  if(trailing == "rig" || trailing == "lef") {
    isWrist = true;
  } else {
    isWrist = false;
  }
  
  // x movement
  if(x > origin_x + circle_offset && isWrist == false) {
    left(); // motor
  }
  if(x < origin_x - circle_offset && isWrist == false) {    
    right(); // motor
  }
  
  // y movement
  if(y > origin_y + forward_backward_offset) {
    backward(); // shoulder
  }
  if(y < origin_y - forward_backward_offset) {
    forward(); // shoulder
  }
  
  // z movement
  if(z > origin_z + circle_offset) {
    down(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
  if(z < origin_z - up_offset) {
    up(); // elbow + MAKE WRIST JOIN ONE ALWAYS KEEP GRIPPER LEVEL
  }
}

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
  
    position(x.toInt(),y.toInt(),z.toInt(), trailing);
  
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
