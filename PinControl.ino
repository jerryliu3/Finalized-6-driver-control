#include <Wire.h>
#include <Arduino.h>
#include "DRV8834.h"

boolean forward = true;
double number1 = 0.0, number2 = 0.0, number3 = 0.0;
int master = 1;
int reset = 0;

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 20

//motor1
// All the wires needed for full functionality
#define DIR1 13
#define STEP1 12
// microstep control for DRV8834
#define M01 9
#define M11 8
DRV8834 stepper1(MOTOR_STEPS, DIR1, STEP1, M01, M11);

//motor2
// All the wires needed for full functionality
#define DIR2 9
#define STEP2 8 

// microstep control for DRV8834
#define M02 6
#define M12 7
DRV8834 stepper2(MOTOR_STEPS, DIR2, STEP2, M02, M12);

//motor3
// All the wires needed for full functionality
#define DIR3 5
#define STEP3 4

// microstep control for DRV8834
#define M03 2
#define M13 3
DRV8834 stepper3(MOTOR_STEPS, DIR3, STEP3, M03, M13);


boolean go = false;

void setup() {
  //parameters are #steps, stepper# (port) M1&M2 = 1, M3&M4 = 2
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  digitalWrite(reset, HIGH);
  pinMode(reset, OUTPUT);
  stepper1.setRPM(60);
  stepper2.setRPM(120);
  stepper3.setRPM(120);
}

void receiveEvent(int bytes) {
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
  
  Serial.println(number1);
  Serial.println(number2);
  Serial.println(number3);
  Serial.println("finished");
  go = true;
}

void requestEvent()
{
    Serial.println("sending");
    Wire.beginTransmission(master); // transmit to device #1
    char number1c[5];
    String(number1,2).toCharArray(number1c,5);
    Wire.write(number1c);              // sends x 
    char number2c[5];
    String(number2,2).toCharArray(number2c,5);
    Wire.write(number2c);  
    char number3c[5];
    String(number3,2).toCharArray(number3c,5);
    Wire.write(number3c);  
    Wire.endTransmission(master);    // stop transmitting    
    Serial.println("sent");
}

void loop() {
  if (go)
  {
    Wire.beginTransmission(master); // transmit to device #1
    if (number1 == -1||number1 == -1.00)
    {
      Wire.write('d');
    }
    else
    {
      Wire.write('c');
    }
    Wire.endTransmission(master);
    go = false;

    stepper1.setMicrostep(1); // make sure we are in full speed mode

    // these two are equivalent: 180 degrees is 100 steps in full speed mode
    stepper1.move(20);
    //stepper1.rotate(180);
    // one full reverse rotation
    stepper1.move(-20);
    //stepper1.rotate(-180);

    
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
