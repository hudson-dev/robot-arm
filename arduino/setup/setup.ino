#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  95 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  460 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 15;

void setup() {

  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

//  moveShoulder(140);
//  delay(1000);

//  moveToAngle(2, 150); // 150
//  delay(500);
//  
//  moveToAngle(3, 30);
//  delay(500);
//  
//  moveToAngle(4, 68);
//  delay(500);
//  
//  moveToAngle(5, 180);
//  delay(500);
}

void moveToAngle(uint8_t servoNumber, int angle) {
  uint16_t pulselength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servoNumber, 0, pulselength);

  delay(1000);

//  pwm.setPWM(servoNumber, 0, 0 ); // freeing servo
}

void moveShoulder(int angle) {
    int angle_one = 180-angle;
    int angle_two = 0+angle;
  
    moveToAngle(0, angle_one);
    moveToAngle(1, angle_two);
}

void calibrate(int servoNumber, int pulse) {
  pwm.setPWM(servoNumber, 0, pulse);

  delay(1000);
}

void loop() {
  moveShoulder(0);
  delay(1000);

  moveShoulder(90);
  delay(1000);


//  moveToAngle(2, 40); // 150
//  delay(500);
//
//  moveToAngle(3, 140); // 150
//  delay(500);
//
//  moveToAngle(4, 82);
//  delay(500);
//
//  moveToAngle(5, 160);
//  delay(500);
}
