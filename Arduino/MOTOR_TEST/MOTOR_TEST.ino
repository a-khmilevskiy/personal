#include <SoftwareSerial.h> // TX RX software library for bluetooth
#include <Servo.h> // servo library 
#include <Multiservo.h>

#define SPEED_1      5
#define DIR_1       4

#define SPEED_2      6
#define DIR_2        7

Multiservo myservo1, myservo2, myservo3, myservo4; // servo name
int bluetoothTx = 10; // bluetooth tx to 10 pin
int bluetoothRx = 11; // bluetooth rx to 11 pin
int s;
boolean control = false;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
void setup()
{
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  myservo1.attach(10); // attach servo signal wire to pin 9
  myservo2.attach(11);
  myservo3.attach(9);
  //myservo4.attach(3);
  //Setup usb serial connection to computer
  Serial.begin(9600);
  //Setup Bluetooth serial connection to android
  bluetooth.begin(19200);
  myservo1.write(45);
  myservo2.write(45);
  myservo3.write(45);

}
void loop()
{
  if (control==false){
    motorControl();
  }else{
    towerControl();
  }
  
}

void controlToggle() {
  if (bluetooth.available() >= 1 ) {
    int n = bluetooth.read();
    delay(10);
    if (n == 0) {
      if (control == false) {
        control = true;
      }else{
        control = false;
      }
      Serial.println(control);
    }
  }
}


void motorControl() {
  controlToggle();
  if (bluetooth.available() >= 1 ) {
    int n = bluetooth.read();
    delay(10);
    //Serial.println(n);
    switch (n) {
      case 126: {             //Catch 'W'
          motorForward();
        } break;
      case 30: {              //Catch 'S'
          motorBackward();
        } break;

      default : {              //Catch 'B'
          motorStop();
        } break;
      case 6: {               //Catch 'A'
          motorLeft();
        } break;
      case 96: {              //Catch 'D'
          motorRight();
        } break;
    }
  }
}

void towerControl() {
  controlToggle();
  //Read from bluetooth and write to usb serial
  if (bluetooth.available() >= 1 ) {
    int n = bluetooth.read();
    delay(10);
    //Serial.println(n);
    switch (n) {
      case 126: {         //Catch 'W'
          towerUp();
        } break;
      case 30: {          //Catch 'S'
          towerDown();

        } break;

      case 6: {           //Catch 'A'
          towerLeft();
        } break;

      case 96: {          //Catch 'D'
          towerRight();
        } break;

      case 51: {
          armUp();
        } break;

      case 50: {
          armDown();
        } break;

      case 52: {
          grab();
        }
    }
  }
}

void motorForward() {
  analogWrite(SPEED_1, 255);
  analogWrite(SPEED_2, 255);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, LOW);
}

void motorBackward() {
  analogWrite(SPEED_1, 255);
  analogWrite(SPEED_2, 255);
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, HIGH);
}

void motorLeft() {
  analogWrite(SPEED_1, 255);
  analogWrite(SPEED_2, 255);
  digitalWrite(DIR_1, HIGH);
  digitalWrite(DIR_2, LOW);
}
void motorRight() {
  analogWrite(SPEED_1, 255);
  analogWrite(SPEED_2, 255);
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
}
void motorStop() {
  analogWrite(SPEED_1, 0);
  analogWrite(SPEED_2, 0);
}

void towerUp() {
  s = myservo1.read();
  if (s <= 55) {
    myservo1.write(s + 3);
  }
}

void towerDown() {
  s = myservo1.read();
  if (s >= 10) {
    myservo1.write(s - 3);
  }
}
void towerLeft() {
  s = myservo2.read();
  if (s <= 170) {
    myservo2.write(s + 10);
  }
}
void towerRight() {
  s = myservo2.read();
  if (s >= 10) {
    myservo2.write(s - 10);
  }
}

void armUp() {
  s = myservo3.read();
  if (s <= 170) {
    myservo3.write(s + 10);
  }
}

void armDown() {
  s = myservo3.read();
  if (s >= 10) {
    myservo3.write(s - 10);
  }
}

void grab() {
  s = myservo4.read();
  if (s >= 90) {
    myservo4.write(10);
  }
  else {
    myservo4.write(170);
  }
}









