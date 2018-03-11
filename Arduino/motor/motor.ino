 #include <Servo.h>
#include <AFMotor.h>
#include <Multiservo.h>
int command = 0;  //incoming serial data</afmotor.h></softwareserial.h></servo.h></p><p>AF_DCMotor motor1(1, MOTOR12_64KHZ); //initialize motor1 that is connected to the M1 port, this motor moves forward and backward
AF_DCMotor motor2(3, MOTOR12_64KHZ); //motor2 is connected to M2 and this turns BB-8 left and right
AF_DCMotor motor1(4, MOTOR12_64KHZ);

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps - this is the speed the serial interface will work all
  motor1.setSpeed(255); //set default speed
  motor2.setSpeed(255); //set default speed
}
void loop() {
  if (Serial.available() > 0) { //if the Arduino detects incoming data
    // read the incoming byte:
    command = Serial.read();
    
  }
  else{
    command = 0;
    delay(100);
  }
  Serial.println(command);
  
  switch (command) //set different cases of the "command" variable
  {
     default:{
        motor1.run(RELEASE); // stopped
        motor2.run(RELEASE); // stopped
        }break;
      case 'F': { //go forward
        motor2.run(FORWARD);
        motor1.run(FORWARD);
      } break;   
      case 'B': { //go backward
        motor2.run(BACKWARD);
        motor1.run(BACKWARD); 
      } break;   
      case 'R': { //spin right
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
      } break;   
      case 'L': { //spin left
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
      } break;
     
      //you can add more extra feautres by writing a case command (example: different lights, sound, etc...)
  }
}
