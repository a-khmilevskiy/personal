#define LDR_PIN A0
#define LED_PIN 13
#define TEMP_PIN A1
void setup() {
  // put your setup code here, to run once:
pinMode(LED_PIN, OUTPUT);
Serial.begin(9600);
}

void loop() {
  Temperature();
  lightning();
  // put your main code here, to run repeatedly:
  
}

float Temperature(){
  float val;
  val = analogRead(TEMP_PIN)*5.0/1024.0;
  float temp = 14.46*log((10000.0*val)/(5.0-val)/27074.0);
  Serial.print("Температура окружающей среды: ");
  Serial.println(temp);
  delay (500);
  return temp; 
}

void lightning(){
int val = analogRead(LDR_PIN);
if (val>=900){
    digitalWrite(LED_PIN, HIGH);
  }
  else{
    digitalWrite(LED_PIN, LOW);
  }
}


