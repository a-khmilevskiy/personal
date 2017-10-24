#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define FirstButton 7;
#define SecondButton 8;
#define ThirdButton 9;

int command = 0;
int ECHO_PIN = 11; 
int TRIG_PIN = 10;
int cm;

Servo myServo;
AF_DCMotor motor1(1, MOTOR12_64KHZ); //RIGHT PARE
AF_DCMotor motor2(2, MOTOR12_64KHZ); //LEFT PARE

 
void setup() { 
  Serial.begin (9600); 
  myServo.attach(9);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  motor1.setSpeed(255); //set default speed
  motor2.setSpeed(235); //set default speed
} 
 
void loop() 
{
  if (Serial.available() > 0) { //if the Arduino detects incoming data
    // read the incoming byte:
    command = Serial.read();
    //Serial.print(command);
    UserCommandsInit();
  }
  else
  {
  AutoMove();
  }
}

int DistanceTOPath() 
{
  //Этот метод высчитывает расстояние до препятствий
  int duration; 
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH); 
  duration = pulseIn(ECHO_PIN, HIGH); 
  cm = duration / 58;
  delay(300);

  if (cm>=0){
  Serial.println(cm);
  Serial.print("cm");
  return cm; 

  }
}

//Метод крутит головой
char ServoControl()
{
  int first;
  int second;
  delay(500);
  //Поворачиваем датчик на 45 градусов
  myServo.write(45);
    //Проверяем что возвращаемое значение не отрицательное и записываем в переменную 
    first = DistanceTOPath();
  delay(500);
  //Поворачиваем датчик на 135 градусов
  myServo.write(135);
    //Проаверяем что возвращенное значение не отрицательное и записываем в переменную
    second=DistanceTOPath();
  
  //Сравниванием записанные переменные
  if (first>second)
  {
    return 'L';
    //Если число с угла в 45 градусов больше чем полученное со 135 - возвращаем команду L
  }
  if (first<second)
  {
    return 'R';
    //Если число с угла в 45 градусов меньше чем полученное со 135 - возвращаем команду R
  }
  else return 'S';
  while ((first==second)||(first&second<30)){
    
    return 'S';
  }
  
}

void WallReactor()
//Реакция на препятствие
{
  Stop();
  //Проверяем, что выдает наш датчик
  if (ServoControl() == 'L')
  {
    //Если получаем команду L - поворачиваем налево 200 милисекунд
    TurnLeft();
    //Возвращаем датчик в нейтральное положение
    ServoControlNeutral();
  }
  if (ServoControl() =='R')
  {
    //Если получаем R - поворачиваем направо 200 милисекунд
    TurnRight();
    //Возвращаем датчик в нейтральное положение
    ServoControlNeutral();
  }
  if (ServoControl() == 'S')
  {
  Backward();
  }
}

//Фперде!
void AutoMove()
{
  //Ставим сервомотор в нейтральное положение
  ServoControlNeutral();
    if (DistanceTOPath()>=30)
    {
      Forward();
    }
  //Если доезжаем до препятствия - тормозим
    if (DistanceTOPath()<30)
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
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

//поворот налево
void TurnLeft()
{
  motor2.run(BACKWARD);
  //motor1.run(FORWARD);
}

//поворот направо
void TurnRight()
{
  motor1.run(BACKWARD);
  //motor2.run(FORWARD);
}

//едем назад
void Backward()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD); 
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

