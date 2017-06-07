// Include the required Wire library for I2C
#include <Wire.h>

int slave = 0;
double number1 = 0;
double number2 = 0;
double number3 = 0;
//1 is ask position, 2 is move to position, 3 is move by, 4 is stop.
int choice = 0;

void setup() {
  // Start the I2C Bus as Master
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
}

void readNumbers()
{
  while (!Serial.available())
  {

  }
  number1 = Serial.parseFloat();
  number2 = Serial.parseFloat();
  number3 = Serial.parseFloat();
}

void receiveEvent(int bytes) {
  char r = Wire.read();
  if (r == 'd')
  {
    Serial.println('d');
  }
  else
  {
    Serial.println('c');
  }
}

void command()
{
  while (!Serial.available())
  {

  }
  slave = Serial.parseInt();
  choice = Serial.parseInt();
}

void askPos()
{
  //Serial.println('f');
  Wire.requestFrom(slave, 12);
  //Serial.println('f');
  while (!Wire.available())
  {

  }
  String number1S = "";
  for (int x = 0; x < 4; x++)
  {
    number1S = number1S + char(Wire.read());
  }
  number1 = number1S.toFloat();
  String number2S = "";
  for (int x = 0; x < 4; x++)
  {
    number2S += char(Wire.read());
  }
  number2 = number2S.toFloat();
  String number3S = "";
  for (int x = 0; x < 4; x++)
  {
    number3S += char(Wire.read());
  }
  number3 = number3S.toFloat();
}

void sendPos()
{
  askPos();
  Serial.println(number1);
  Serial.println(number2);
  Serial.println(number3);
}

void moveToPos()
{
  Wire.beginTransmission(slave); // transmit to device #2
  char number1c[5];
  String(number1, 2).toCharArray(number1c, 5);
  Wire.write(number1c);              // sends x
  char number2c[5];
  String(number2, 2).toCharArray(number2c, 5);
  Wire.write(number2c);
  char number3c[5];
  String(number3, 2).toCharArray(number3c, 5);
  Wire.write(number3c);
  Wire.endTransmission(slave);    // stop transmitting
}

void moveBy(double move1, double move2, double move3)
{
  //add adding the other numbers
  number1 += move1;
  number2 += move2;
  number3 += move3;
  Wire.beginTransmission(slave); // transmit to device #2
  char number1c[5];
  String(number1, 2).toCharArray(number1c, 5);
  Wire.write(number1c);              // sends x
  char number2c[5];
  String(number2, 2).toCharArray(number2c, 5);
  Wire.write(number2c);
  char number3c[5];
  String(number3, 2).toCharArray(number3c, 5);
  Wire.write(number3c);
  Wire.endTransmission(slave);    // stop transmitting
}

void reset()
{
  number1 = 0;
  number2 = 0;
  number3 = 0;
  moveToPos();
}

void loop() {
  command();
  //Serial.println('j');
  if (choice == 1)
  {
    //Serial.println('f');
    sendPos();
  }
  else if (choice == 2)
  {
    readNumbers();
    moveToPos();
  }
  else if (choice == 3)
  {
    readNumbers();
    double move1 = number1;
    double move2 = number2;
    double move3 = number3;
    askPos();
    moveBy(move1, move2, move3);
  }
  else if (choice == 4)
  {
    reset();
    sendPos();
  }
}
