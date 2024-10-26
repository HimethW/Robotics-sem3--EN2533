#define sensorOut 7
#define S2 6
#define S3 5
#define S0 4
#define S1 2

#define red 11
#define green 10
#define blue 9

#define cal 30

int redPW = 0;
int greenPW = 0;
int bluePW = 0;

int redMin = 400;
int redMax = 10;
int greenMin = 400;
int greenMax = 10;
int blueMin = 400;
int blueMax = 10;

int redVal , greenVal , blueVal;

void setup() 
{
  pinMode(S2 , OUTPUT);
  pinMode(S3 , OUTPUT);
  pinMode(S0 , OUTPUT);
  pinMode(S1 , OUTPUT);
  pinMode(sensorOut , INPUT);

  digitalWrite(S0,HIGH);         //scaling to 20%
	digitalWrite(S1,LOW);      

  Serial.begin(9600);

 
}

void loop() 
{
  redPW = calcRed();
  delay(200);
  greenPW = calcGreen();
  delay(200);
  bluePW = calcBlue();
  delay(200);

  redVal = map(redPW , redMin , redMax , 0 , 255);
  greenVal = map(greenPW , greenMin , greenMax , 0 , 255);
  blueVal = map(bluePW , blueMin , blueMax , 0 , 255);


  if(redVal > greenVal && redVal > blueVal)
  {
    redVal = redVal + cal;
    greenVal = greenVal - cal;
    blueVal = blueVal - cal;
    if(redVal > 255)
    {
      redVal = 255;
    }
  }
  else if(greenVal > redVal && greenVal > blueVal)
  {
    redVal = redVal - cal;
    greenVal = greenVal + cal;
    blueVal = blueVal - cal;
    if(greenVal > 255)
    {
      greenVal = 255;
    }
  }
  else if(blueVal > greenVal && blueVal > redVal)
  {
    redVal = redVal - cal;
    greenVal = greenVal - cal;
    blueVal = blueVal + cal;
    if(blueVal > 255)
    {
      blueVal = 255;
    }
  } 

  Serial.print("red = ");
  Serial.print(redPW);
  Serial.print(" - green = ");
  Serial.print(greenPW);
  Serial.print(" - blue = ");
  Serial.print(bluePW);

  Serial.print(" - red val = ");
  Serial.print(redVal);
  Serial.print(" - green val = ");
  Serial.print(greenVal);
  Serial.print(" - blue val = ");
  Serial.println(blueVal);

  analogWrite(red , redVal);
  analogWrite(green , greenVal);
  analogWrite(blue , blueVal);

}



int calcRed()
{
  digitalWrite(S2,LOW);
	digitalWrite(S3,LOW);
  delay(100);

  return pulseIn(sensorOut , LOW);
}

int calcGreen()
{
  digitalWrite(S2,HIGH);
	digitalWrite(S3,HIGH);
  delay(100);

  return pulseIn(sensorOut , LOW);
}

int calcBlue()
{
  digitalWrite(S2,LOW);
	digitalWrite(S3,HIGH);
  delay(100);

  return pulseIn(sensorOut , LOW);
}
