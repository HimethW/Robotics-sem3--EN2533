#include <Stepper.h>

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(100);
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(4,INPUT);
}

void loop() {
  if(digitalRead(2)==1){
    myStepper.step(stepsPerRevolution);
    Serial.println("pin2 = High");
  }


  if(digitalRead(4)==1){
    Serial.println("pin4 = High");
    myStepper.step(-stepsPerRevolution);
  }

}
