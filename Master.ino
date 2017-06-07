  // Include the required Wire library for I2C
#include <Wire.h>

boolean go = false;
int slave = 0;
double number1=0;
double number2=0;
double number3=0;
int reset = 0;
char skip = 's';

void setup() {
  // Start the I2C Bus as Master
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(1); 
  Wire.onReceive(receiveEvent);
}

void readNumbers()
{
  while(!Serial.available())
  {
    
  }
  slave = Serial.parseInt();
  number1 = Serial.parseFloat();
  number2 = Serial.parseFloat();
  number3=Serial.parseFloat();
  go = true;
}

void receiveEvent(int bytes) {
    char r = Wire.read();
    if(r == 'd')
    {
        Serial.println('d');
    }
    else
    {
      Serial.println('c');
    }
}   

void loop() {
  readNumbers();
//  if(go)
//  {
    Wire.beginTransmission(slave); // transmit to device #2
    char number1c[5];
    String(number1,2).toCharArray(number1c,5);
    Wire.write(number1c);              // sends x 
    char number2c[5];
    String(number2,2).toCharArray(number2c,5);
    Wire.write(number2c);  
    char number3c[5];
    String(number3,2).toCharArray(number3c,5);
    Wire.write(number3c);  
    Wire.endTransmission(slave);    // stop transmitting
    go = false;
//  }
}
