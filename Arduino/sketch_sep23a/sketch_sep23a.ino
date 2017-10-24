
#include <AFMotor.h>

int command = 0;  //incoming serial data</afmotor.h></softwareserial.h></servo.h></p><p>AF_DCMotor motor1(1, MOTOR12_64KHZ); //initialize motor1 that is connected to the M1 port, this motor moves forward and backward
AF_DCMotor motor2(2, MOTOR12_64KHZ); //motor2 is connected to M2 and this turns BB-8 left and right
AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motorhead(3); //this motor turns the head left or right</p><p>Servo head;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps - this is the speed the serial interface will work all
  motor1.setSpeed(255); //set default speed
  motor2.setSpeed(255); //set default speed
  motorhead.setSpeed(1); //the motor should spin very slowly
  pinMode(13, OUTPUT); //connect here the blue LED
}
void loop() {
  
  if (Serial.available() > 0) { //if the Arduino detects incoming data
    // read the incoming byte:
    command = Serial.read();
    Serial.print(command);
  }
  switch (command) //set different cases of the "command" variable
  {
    case 'S': {
        motor1.run(RELEASE); // stopped
        motor2.run(RELEASE); // stopped
        command = '*';
      } break;   
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
      case 'G': { //forward left
        motor1.run(FORWARD);
        motor2.run(RELEASE);
      } break;   
      case 'I': { //forward right
        motor1.run(RELEASE);
        motor2.run(FORWARD);
      } break;   
      case 'H': { //backward left
        motor1.run(RELEASE);
        motor2.run(BACKWARD);
      } break;   case 'J': { //backward right
        motor1.run(BACKWARD);
        motor2.run(RELEASE);
      } break;   
      case 87: {
        digitalWrite(13, HIGH); //lights on
      } break;   
      case 'w': {
        digitalWrite(13, LOW); //lights off
      } break;   
      case 'U': {
        motor1.setSpeed(180); //set lower speed
      } break;   
      case 'u': {
        motor1.setSpeed(255); //set max speed
      } break;   
      case 'V': {
        motorhead.run(FORWARD); //turn head left
      } break;   
      case 'v': {
        motorhead.run(RELEASE); //stop spinning
      } break;   
      case 'X': {
        motorhead.run(BACKWARD); //turn head right
      } break;   
      case 'x': {
        motorhead.run(RELEASE); //stop spinning
      } break;
      //you can add more extra feautres by writing a case command (example: different lights, sound, etc...)
  }
}
