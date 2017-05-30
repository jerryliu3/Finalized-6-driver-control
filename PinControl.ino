#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Arduino.h>
#include "DRV8834.h"

boolean forward = true;
double number1 = 0.0, number2 = 0.0, number3 = 0.0;
int reset = 0;
//Adafruit_MotorShield AFMSBOTTOM(0x60);
//Adafruit_MotorShield AFMSTOP(0x61); // Rightmost jumper closed
//Adafruit_StepperMotor *myMotor1 = AFMSTOP.getStepper(25, 1);
//Adafruit_StepperMotor *myMotor2 = AFMSTOP.getStepper(25, 2);
//Adafruit_StepperMotor *myMotor3 = AFMSBOTTOM.getStepper(25, 2);

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 20

//motor1
// All the wires needed for full functionality
#define DIR1 13
#define STEP1 12
//#define ENBL1 12

// 2-wire basic config, microstepping is hardwired on the driver
// BasicStepperDriver stepper(DIR, STEP);

// microstep control for DRV8834
#define M01 9
#define M11 8
DRV8834 stepper1(MOTOR_STEPS, DIR1, STEP1, M01, M11);
// DRV8834 stepper(MOTOR_STEPS, DIR, STEP, ENBL, M0, M1);

//motor2
// All the wires needed for full functionality
#define DIR2 9
#define STEP2 8 
//#define ENBL2 8

// 2-wire basic config, microstepping is hardwired on the driver
// BasicStepperDriver stepper(DIR, STEP);

// microstep control for DRV8834
#define M02 6
#define M12 7
DRV8834 stepper2(MOTOR_STEPS, DIR2, STEP2, M02, M12);
// DRV8834 stepper(MOTOR_STEPS, DIR, STEP, ENBL, M0, M1);

//motor3
// All the wires needed for full functionality
#define DIR3 5
#define STEP3 4
//#define ENBL3 4

// 2-wire basic config, microstepping is hardwired on the driver
// BasicStepperDriver stepper(DIR, STEP);

// microstep control for DRV8834
#define M03 2
#define M13 3
DRV8834 stepper3(MOTOR_STEPS, DIR3, STEP3, M03, M13);
//// DRV8834 stepper(MOTOR_STEPS, DIR, STEP, ENBL, M0, M1);


boolean go = false;

void setup() {
  //parameters are #steps, stepper# (port) M1&M2 = 1, M3&M4 = 2
  Serial.begin(9600);           // set up Serial library at 9600 bps
  //  Serial.println("begin");
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
   //AFMSTOP.begin();  // create with the default frequency 1.6KHz
//  AFMSBOTTOM.begin();
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  //  myMotor1->setSpeed(10);
  //  myMotor2->setSpeed(10);
//  myMotor3->setSpeed(10);
  //Serial.println("loop should start");
  digitalWrite(reset, HIGH);
  pinMode(reset, OUTPUT);
  stepper1.setRPM(60);
  stepper2.setRPM(120);
  stepper3.setRPM(120);
}

void receiveEvent(int bytes) {
  // Serial.println("good");
  Serial.println("reading");
  String number1S = "";
  for(int x=0;x<4;x++)
  {
    number1S = number1S+ char(Wire.read());
  }
  number1 = number1S.toFloat();
    String number2S = "";
 for(int x=0;x<4;x++)
  {
    number2S += char(Wire.read());
  }
  number2 = number2S.toFloat();
    String number3S = "";
  for(int x=0;x<4;x++)
  {
    number3S += char(Wire.read());
  }
  number3 = number3S.toFloat();
  //  myMotor1->step((int)number1, FORWARD, SINGLE);
  //      myMotor2->step((int)number2, FORWARD, SINGLE);
  //myMotor3->step((int)number3, FORWARD, SINGLE);

  
  Serial.println(number1);
 Serial.println(number2);
 Serial.println(number3);
  Serial.println("finished");
  go = true;
}

void loop() {
  // Serial.println("loop");
  if (go)
  {
    Wire.beginTransmission(1); // transmit to device #1
    if (number1 == -1||number1 == -1.00)
    {
      Wire.write('d');

    }
    else
    {
      Wire.write('c');
    }
    //Wire.write('d');
    Wire.endTransmission();
    go = false;

      stepper1.setMicrostep(1); // make sure we are in full speed mode

    // these two are equivalent: 180 degrees is 100 steps in full speed mode
    stepper1.move(20);
    //stepper1.rotate(180);
  delay(1000);
    // one full reverse rotation
    stepper1.move(-20);
    //stepper1.rotate(-180);

    /*
     * Microstepping mode: 1,2,4,8,16 or 32(DRV8834 only)
     * Mode 1 is full speed.
     * Mode 32 is 32 microsteps per step.
     * The motor should rotate just as fast (set RPM),
     * but movement precision is increased.
     */
//    stepper1.setMicrostep(8);
//
//    // 180 degrees now takes 100 * 8 microsteps
//    stepper1.move(100*8);
//    stepper1.rotate(180);
//
//    // as you can see, using degrees is easier
//    stepper1.move(-100*8);
//    stepper1.rotate(-180);

    
        stepper2.setMicrostep(1); // make sure we are in full speed mode

    // these two are equivalent: 180 degrees is 100 steps in full speed mode
    stepper2.move(100);
    stepper2.rotate(180);

    // one full reverse rotation
    stepper2.move(-100);
    stepper2.rotate(-180);

    /*
     * Microstepping mode: 1,2,4,8,16 or 32(DRV8834 only)
     * Mode 1 is full speed.
     * Mode 32 is 32 microsteps per step.
     * The motor should rotate just as fast (set RPM),
     * but movement precision is increased.
     */
    stepper2.setMicrostep(8);

    // 180 degrees now takes 100 * 8 microsteps
    stepper2.move(100*8);
    stepper2.rotate(180);

    // as you can see, using degrees is easier
    stepper2.move(-100*8);
    stepper2.rotate(-180);


      stepper3.setMicrostep(1); // make sure we are in full speed mode

    // these two are equivalent: 180 degrees is 100 steps in full speed mode
    stepper3.move(100);
    stepper3.rotate(180);

    // one full reverse rotation
    stepper3.move(-100);
    stepper3.rotate(-180);

    /*
     * Microstepping mode: 1,2,4,8,16 or 32(DRV8834 only)
     * Mode 1 is full speed.
     * Mode 32 is 32 microsteps per step.
     * The motor should rotate just as fast (set RPM),
     * but movement precision is increased.
     */
    stepper3.setMicrostep(8);

    // 180 degrees now takes 100 * 8 microsteps
    stepper3.move(100*8);
    stepper3.rotate(180);

    // as you can see, using degrees is easier
    stepper3.move(-100*8);
    stepper3.rotate(-180);

    digitalWrite(reset, LOW);

  }
}
