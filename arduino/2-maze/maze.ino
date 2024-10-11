#include <Robot.h>
#include <Wheels.h>
#include <IRSensors.h>
#include <LineFollower.h>

#define ASCENDING 0
#define DESCENDING 1
#define led 5

const Wheel_t rearLeft = {1, 2, 3}; // controlPinA, controlPinB, speedPin
const Wheel_t forwardLeft = {4, 5, 6};
const Wheel_t forwardRight = {7, 8, 9};
const Wheel_t rearRight = {10, 11, 12};

const Wheel_t leftWheels[2] = {rearLeft, forwardLeft};
const Wheel_t rightWheels[2] = {rearRight, forwardRight};

const byte numIRSensors = 6;
const byte IRSensorPins[numIRSensors] = {13, 14, 15, 16, 17, 18}; // PWM pins
const float weightsArray[numIRSensors] = {-3, -2, -1, 1, 2, 3};

Robot nilakna; // LOL
IRSensorArray pravindu(IRSensorPins, numIRSensors); // LOL

LineFollower lineFollower(&nilakna, &pravindu);


//moving and picking functions
void f(){
  driveTillJunction(&nilakna, &lineFollower);
}

void b(){
  nilakna.turn(0, 90);
  nilakna.turn(0, 90);
  driveTillJunction(&nilakna, &lineFollower);
}

void r(){
  nilakna.turn(0, 90);
}

void l(){
  nilakna.turn(0, -90);
}

void pickup(){
  digitalWrite(led,HIGH);
}

void drop(){
  digitalWrite(led,LOW);
}

//these are values that are obtained from task 1
int number = 9;
int opengate = 2;

int result = number % 5;

//initial preparations

void movebox(){
    if(result == 0){
        pickup();
        f();
        drop();
        b();
        r();
        f();
        l();
        f();
    }
    else{
        f();
        r();
        f();
        l();
    }
}

void findgate(){
    //check if gate 1 is open if so set opengate to 1
    r();
    f();
    f();
    l();
    //check if gate 2 is open if so set opengate to 2
}

void func0(){
    l();
    f();
    f();
    f();
    pickup();
    b();
    if(opengate == 1){
        drop();
        b();
        l();
        f();
        r();
        f();
        r();
        f();
        pickup();
        f();
    }
    else{
        b();
        b();
        drop();
        b();
        l();
        f();
        r();
        f();
        r();
        f();
        pickup();
        f();
    }
}

void func1(){
    if(opengate == 1){
        l();
        f();
        f();
        l();
        f();
        pickup();
        b();
        b();
    }
    else{
        b();
        l();
        f();
        f();
        pickup();
        b();
        b();
        drop();
        b();
        r();
        f();
        l();
        f();
        l();
        f();
        pickup();
        b();
        b();
    }
}
        
void func2(){
    b();
    l();
    f();
    pickup();
    if(opengate == 1){
        f();
        drop();
        b();
        r();
        f();
        l();
        f();
        l();
        f();
        pickup();
        b();
        b();
    }
    else{
        b();
        drop();
        b();
        r();
        f();
        l();
        f();
        l();
        f();
        pickup();
        b();
        b();
    }
}
        
void func3(){
    if(opengate == 1){
        r();
        f();
        r();
        f();
        r();
        f();
        pickup();
        f();
        f();
        drop();
        b();
        r();
        f();
        l();
        f();
        l();
        f() ;
        pickup();
        b();
        b();
    }
    else{
        r();
        r();
        f();
        pickup();
        b();
        b();
    }
}
        
void func4(){
    r();
    r();
    f();
    l();
    f();
    pickup();
    b();
    if (opengate == 1){
        b();
        b();
    }
    drop(); 
    b();
    l();
    f();
    r();
    f();
    r();
    f();
    pickup();
    b();
    b();
}

void setup() {
  pravindu.setWeights(weightsArray);
  nilakna.attachWheels(leftWheels, 2, rightWheels, 2);
  nilakna.setBaseSpeed(1.0);

  lineFollower.setPIDConstants(1, 0, 1);
  pinMode(led, OUTPUT);
}



void loop(){
  



// Code starts here
movebox();
findgate();
switch(result){
  case 0:
    func0();
    break;
  case 1:
    func1();
    break;
  case 2:
    func2();
    break;
  case 3:
    func3();
    break;
  case 4:
    func4();
    break;
}