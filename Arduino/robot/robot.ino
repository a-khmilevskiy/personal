#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>

int grxPin = 0;
int gtxPin = 1;
int command = 0;
int ECHO_PIN = 11; 
int TRIG_PIN = 10;

SoftwareSerial BTSerial(grxPin, gtxPin);

Servo myServo;
AF_DCMotor motor1(3, MOTOR12_64KHZ); //RIGHT PARE
AF_DCMotor motor2(2, MOTOR12_64KHZ); //LEFT PARE

 
void setup() { 
  BTSerial.begin(38400);
  Serial.begin (9600); 
  myServo.attach(9);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  motor1.setSpeed(255); //set default speed
  motor2.setSpeed(255); //set default speed
} 
 
void loop() 
{
  if (BTSerial.available() > 0) { //if the Arduino detects incoming data
    Stop();
    BluetoothControl();

  }
  else
  {
  AutoMove();
  }
}

int DistanceTOPath() 
{
  //Этот метод высчитывает расстояние до препятствий
  unsigned long cm, micros_old_Ult; 
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  //этот кусок взял из интернета, предположительно он делает правильные задержки сигнала
  micros_old_Ult = micros();
  while(!digitalRead(ECHO_PIN) && micros()-micros_old_Ult < 500){
    micros_old_Ult = micros();
  }
    while(digitalRead(ECHO_PIN) && micros()-micros_old_Ult < 20000){
  //формула расета расстояния
    cm = (micros() - micros_old_Ult)/29.0/2;
    }
    delay(300);
    Serial.print("Distance = ");
    Serial.println(cm);
    if (0<cm){
    return(cm);
    }
}


//Метод крутит головой
char ServoControl()
{
  int first = 0;
  int second = 0;
  //Поворачиваем датчик на 45 градусов
  myServo.write(45);
    delay(500);
    //Проверяем что возвращаемое значение не отрицательное и записываем в переменную 
    first = DistanceTOPath();
    Serial.print("Right side ");
    Serial.println(first);
    
  //Поворачиваем датчик на 135 градусов
  myServo.write(135);
     delay(500);
    //Проаверяем что возвращенное значение не отрицательное и записываем в переменную
    second=DistanceTOPath();
    Serial.print("Left side ");
    Serial.println(second);    
  //Сравниванием записанные переменные
  if (first>second)
  {
    return 'R';
    //Если число с угла в 45 градусов больше чем полученное со 135 - возвращаем команду L
  }
  if (first<second)
  {
    return 'L';
    //Если число с угла в 45 градусов меньше чем полученное со 135 - возвращаем команду R
  }
  else return 'S';
  if (first and second<40){
    
    return 'S';
  }
}

void WallReactor()
//Реакция на препятствие
{
  Stop();
  //Проверяем, что выдает наш датчик
  char value = ServoControl();
  if (value == 'L')
  {
    //Если получаем команду L - поворачиваем налево 200 милисекунд
    TurnLeft();
    //Возвращаем датчик в нейтральное положение
    ServoControlNeutral();
  }
  if (value =='R')
  {
    //Если получаем R - поворачиваем направо 200 милисекунд
    TurnRight();
    //Возвращаем датчик в нейтральное положение
    ServoControlNeutral();
  }
  if (value == 'S')
  {
  Backward();
  }

}

//Фперде!
void AutoMove()
{
  //Ставим сервомотор в нейтральное положение
  ServoControlNeutral();
  Forward();
  //Если доезжаем до препятствия - тормозим
    if (DistanceTOPath()<40)
    { 
    WallReactor();
    }
 }
 
//СТОЯТЬ!!!
void Stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
//Вперде!
void Forward()
{
  motor2.run(FORWARD);
  motor1.run(FORWARD);
  
}

//поворот налево
void TurnLeft()
{
  motor2.run(BACKWARD);
  motor1.run(FORWARD);
  delay(600);
  motor2.run(RELEASE);
  motor1.run(RELEASE);
  
}

//поворот направо
void TurnRight()
{
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(600);
  motor2.run(RELEASE);
  motor1.run(RELEASE);
}

//едем назад
void Backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(600);
  motor1.run(RELEASE);
  motor2.run(RELEASE); 
}

void ServoControlNeutral()
{
  //Приводим датчик в нейтральное положение
  myServo.write(90);
}

void UserCommandsInit()
{
  if (command != 0)
    {
        BluetoothControl();
    }
  delay(5000);
}

void BluetoothControl(){
  command = BTSerial.read();
  Serial.print(command);
  switch (command) //set different cases of the "command" variable
  {
    case 'S': {
        Stop();
        command = '*';
      } break;   
      case 'F': { //go forward
        Forward();
      } break;   
      case 'B': { //go backward
        Backward();
      } break;   
      case 'R': { //spin right
        TurnRight();
      } break;   
      case 'L': { //spin left
        TurnLeft();
      } break;   
      //you can add more extra feautres by writing a case command (example: different lights, sound, etc...)
    }
  }


