
#define IR1 A0 //D2
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4
#define IR6 A5 //D7

#define RMotorA 8 //2 to 3  side change
#define RMotorB 7 //3 to 2 side change
#define RMotorPWM 9

#define LMotorA 5 //4 to 5  side change
#define LMotorB 4 //5 to 4  side change
#define LMotorPWM 3

#define MAX_SPEED 100

int MotorBasespeed = 80;

int IR_val[6] = {0, 0, 0, 0, 0, 0};
int IR_weights[6] = {-20,-10,-5,5,10,20};

int LMotorSpeed = 0;
int RMotorSpeed = 0;
int speedAdjust = 0;

float P, I, D;
float error = 0;
float previousError = 0;
float Kp = 5;
float Kd = 2; 
float Ki = 0.001;

void PID_control();
void read_IR();
void set_speed();
void set_forward();


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  pinMode(IR6, INPUT);

  pinMode(LMotorA, OUTPUT);
  pinMode(LMotorB, OUTPUT);
  pinMode(LMotorPWM, OUTPUT);

  pinMode(RMotorA, OUTPUT);
  pinMode(RMotorB, OUTPUT);
  pinMode(RMotorPWM, OUTPUT);

  set_forward();
  delay(2000);
}


void loop()
{
  read_IR();
  if (IR_val[0] ==0 && IR_val[1] ==0 && IR_val[2] ==0 && IR_val[3] ==0 && IR_val[4] ==0 && IR_val[5] ==0){
    stop();
  while(1);
  }
  if (IR_val[0] ==1 && IR_val[1] ==1 && IR_val[2] ==1 && IR_val[3] ==1 && IR_val[4] ==1 && IR_val[5] ==1){
    stop();
    while(1);
  }
  PID_control();
  //  LMotorSpeed = MAX_SPEED;
  
  // RMotorSpeed = MAX_SPEED;
  set_speed();
  printIR_value();
}

void PID_control() {

  error =0;

  for (int i = 0; i < 6; i++)
  {
    error += IR_weights[i] * IR_val[i];
  }

  P = error;
  I = I + error;
  D = error - previousError;

  previousError = error;

  speedAdjust = (Kp * P + Ki * I + Kd * D);

  LMotorSpeed = MotorBasespeed - speedAdjust;
  RMotorSpeed = MotorBasespeed + speedAdjust;

  if (LMotorSpeed < 0)
  {
    LMotorSpeed = 0;
  }
  if (RMotorSpeed < 0)
  {
    RMotorSpeed = 0;
  }
  if (LMotorSpeed > MAX_SPEED)
  {
    LMotorSpeed = MAX_SPEED;
  }
  if (RMotorSpeed > MAX_SPEED)
  {
    RMotorSpeed = MAX_SPEED;
  }


}

void read_IR(){
  IR_val[0] = digitalRead(IR1);
  IR_val[1] = digitalRead(IR2);
  IR_val[2] = digitalRead(IR3);
  IR_val[3] = digitalRead(IR4);
  IR_val[4] = digitalRead(IR5);
  IR_val[5] = digitalRead(IR6);
}

void set_speed(){
  analogWrite(LMotorPWM, LMotorSpeed);
  analogWrite(RMotorPWM, RMotorSpeed);
}
void set_forward(){
  digitalWrite(LMotorB, HIGH);
  digitalWrite(RMotorB, HIGH);
  digitalWrite(LMotorA, LOW);
  digitalWrite(RMotorA, LOW);
}

void stop(){
  digitalWrite(LMotorB, LOW);
  digitalWrite(RMotorB, LOW);
  digitalWrite(LMotorA, LOW);
  digitalWrite(RMotorA, LOW);
}

void printIR_value(){
  Serial.println(analogRead(IR1));    
}