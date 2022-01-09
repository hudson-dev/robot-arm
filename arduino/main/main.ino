#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// servos

#define SERVOMIN  95 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  460 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

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

int min = 95;
int max = 460;

int circle_offset = 190;
int up_offset = 280;
int forward_backward_offset = 180;

bool isWrist = false;

int angleChange = 1;

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
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates
  
  delay(10);

  startup();
}

void startup() {
  moveShoulder(shoulder_start_angle);
  current_shoulder_angle = shoulder_start_angle;

  moveToAngle(2, 150);
  current_elbow_angle = elbow_start_angle;
  delay(500);
  
  moveToAngle(3, 30);
  current_wrist_one_angle = wrist_one_start_angle;
  delay(500);
  
  moveToAngle(4, 68);
  current_wrist_two_angle = wrist_two_start_angle;
  delay(500);
  
  moveToAngle(5, 180);
  delay(500);
}

void moveToAngle(int servoNumber, int angle) {
  int pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNumber, 0, pulselength);
}

void printOnce(String statement) {
  if(printed == false) {
    Serial.println(statement);
    printed = true;
  }
}

void control(int input_angle, int min_angle, int max_angle, int servo_number, bool isShoulder) {
  if(isShoulder == false) {
    if(input_angle >= min_angle && input_angle <= max_angle) {
      moveToAngle(servo_number, input_angle);
    }
  } else {
    if(input_angle >= min_angle && input_angle <= max_angle) {
      moveShoulder(input_angle);
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
  Serial.println("---------------OPEN---------------");
  moveToAngle(gripper, gripper_max);
}

void close() {
  Serial.println("---------------CLOSE---------------");
  moveToAngle(gripper, gripper_min);
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
//  spin(true);
}

void left() {
  Serial.println("LEFT");
//  spin(false);
}

void up() {
  Serial.println("UP");
  current_shoulder_angle = current_shoulder_angle - angleChange;
  control(current_shoulder_angle, shoulder_min, shoulder_max, 0, true);
}

void down() {
  Serial.println("DOWN");
  current_shoulder_angle = current_shoulder_angle + angleChange;
  control(current_shoulder_angle, shoulder_min, shoulder_max, 0, true);
}

void forward() {
  Serial.println("FORWARD");
  current_elbow_angle = current_elbow_angle + angleChange;
  control(current_elbow_angle, elbow_min, elbow_max, elbow, false);

  // TODO: ADD IN WRIST ONE TO MAINTAIN LEVEL POSITION
}

void backward() {
  Serial.println("BACKWARD");
  current_elbow_angle = current_elbow_angle - angleChange;
  control(current_elbow_angle, elbow_min, elbow_max, elbow, false);

  // TODO: ADD IN WRIST ONE TO MAINTAIN LEVEL POSITION
}

void right_wrist() {
  Serial.println("RIGHT WRIST");
  current_wrist_two_angle = current_wrist_two_angle + angleChange;
  control(current_wrist_two_angle, wrist_two_min, wrist_two_max, wrist_two, false);
}

void left_wrist() {
  Serial.println("LEFT WRIST");
  current_wrist_two_angle = current_wrist_two_angle - angleChange;
  control(current_wrist_two_angle, wrist_two_min, wrist_two_max, wrist_two, false);
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

//    Serial.println("x,y,z: " + String(x) + ", " + String(y) + ", " + String(z));
//    Serial.println(String(input));
//    Serial.println(String(trailing));
  
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
