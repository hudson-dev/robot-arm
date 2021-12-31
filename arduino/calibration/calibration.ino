#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servo_num = 1;

int min = 150;
int max = 640;

int iterations = 10;

void setup() {
  Serial.begin(9600);
  
  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
 
  delay(10);
}

void moveToAngle(int angle) {
  if(angle > 180) {
    return;
  }

  int pulse = map(angle, 0, 180, min, max);
  pwm.setPWM(servo_num, 0, pulse);

  delay(1000);
}

void setMin() {
  pwm.setPWM(servo_num, 0, min); 
}

void setMax() {
  pwm.setPWM(servo_num, 0, max); 
}

void findMin() {
  for(int i = 0; i < iterations; i++) {
    min = min - 10;
    Serial.println("MIN VALUE: " + String(min));
    
    setMin();
    delay(7000);
  }
}

void findMax() {
  for(int i = 0; i < iterations; i++) {
    max = max + 10;
    Serial.println("MAX VALUE: " + String(max));
    
    setMax();
    delay(7000);
  }
}

void minToMax() {
  pwm.setPWM(servo_num, 0, min);
  delay(1000);

  pwm.setPWM(servo_num, 0, max);
  delay(1000);
}

 
void loop() {
//  setMin();
//  setMax();
//  findMin();
//  findMax();

//  minToMax();
  moveToAngle(0);
  moveToAngle(180);
}
