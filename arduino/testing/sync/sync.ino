#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servo_one = 0;
uint8_t servo_two = 1;

int min = 150;
int max = 640;

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

void moveSync(int angle) {
  int position = 0;

  moveToAngle(servo_one, position);
  moveToAngle(servo_two, position);
  
  while(position != angle) {
    position = position + 10;
    
    moveToAngle(servo_one, position);
    moveToAngle(servo_two, position);
  }
}
 
void loop() { 
//  moveToAngle(servo_one, 0);
//  moveToAngle(servo_one, 180);
//  
//  moveToAngle(servo_two, 0);
//  moveToAngle(servo_two, 180);

//   pwm.setPWM(0, 0, 400);
//   delay(500);
//   pwm.setPWM(0, 0, 300);

//  for(int servoPos = 0; servoPos < 180; servoPos+=1) {
//    moveToAngle(servo_one, servoPos);
//    moveToAngle(servo_two, servoPos);
//  }

    moveToAngle(servo_one, 0);
    moveToAngle(servo_two, 0);

    delay(1000);

    moveToAngle(servo_one, 180);
    moveToAngle(servo_two, 180);

    delay(1000);
}
