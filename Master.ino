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
//  while(Serial.available()==0)
//  {
//  number1 = Serial.parseFloat();
//  }
//while(Serial.available()==0)
//  {
//  number2 = Serial.parseFloat();
//  }
//while(Serial.available()==0)
//  {
//  number3 = Serial.parseFloat();
//  }


while(slave ==0)
{
slave = Serial.parseInt();
}
Serial.read();
number1 = Serial.parseFloat();
Serial.read();
number2 = Serial.parseFloat();
Serial.read();
number3=Serial.parseFloat();
//  Serial.println(number1);
//  Serial.println(number2);
//  Serial.println(number3);
  go = true;
  Wire.begin(1); 
  Wire.onReceive(receiveEvent);
  digitalWrite(reset,HIGH);
  pinMode(reset,OUTPUT);
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
        digitalWrite(reset,LOW);
}   

void loop() {
  if(go)
  {
        Wire.beginTransmission(slave); // transmit to device #2
            // Serial.println('d');
           char number1c[5];
    String(number1,2).toCharArray(number1c,5);
    Wire.write(number1c);              // sends x 
               // Serial.println((int)(number1*100));
                           char number2c[5];
    String(number2,2).toCharArray(number2c,5);
    Wire.write(number2c);  
   // Serial.println((int)(number2*100));
    // Serial.println(number2c);
               char number3c[5];
    String(number3,2).toCharArray(number3c,5);
    Wire.write(number3c);  
  //  Serial.println((int)(number3*100));
   //  Serial.println(number3c);
  //             Serial.println('d');
    Wire.endTransmission(slave);    // stop transmitting

   // Serial.println("done");
    go = false;
  }
    while(!Serial.available())
    {
      //Serial.println('u');
    }
  number1 = Serial.parseFloat();
 // Serial.println(number1);
  number2 = Serial.parseFloat();
  number3 = Serial.parseFloat();
  go = true;
}


