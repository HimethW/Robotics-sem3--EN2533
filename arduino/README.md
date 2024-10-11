## `Wheel` Class

#### Constructor
`Wheel wheelName(byte controlPinA, byte controlPinB, byte speedPin)`

A motor controlled through an H-bridge requires two inputs (from digital pins) to determine the direction in which to rotate,
and one input to set the speed to rotate at (from a PWM pin).

#### Methods
1. `setSpeed(float speed)`

    Writes a PWM signal corresponding to `speed` to `speedPin`.

3. `forward()`

    Writes `HIGH` to `controlPinA`, `LOW` to `controlPinB`.

2. `backward()`

    Writes `LOW` to `controlPinA`, `HIGH` to `controlPinB`.

3. `stop()`

    Writes `0` to `speedPin`.

4. `brake()`

    Writes `LOW` to both `controlPinA` and `controlPinB`.

#### Other
`Wheel.h` includes the definition of the type `Wheel_t`, defined as follows.
```
typedef struct {
    byte controlPinA,
    byte controlPinB,
    byte speedPin
} Wheel_t;
```
This can be used to store all the pins required to instantiate a `Wheel` object, and can be passed to a different function that could use these values to do so.

## `Sensor` Class
Sensors can typically be interfaced using libraries. Code that imports such libraries and reads values from the sensor can be implemented in a different file. It is sufficient to implement functions that read values from the sensor, process them as necessary, and return an array of floats or integers. Such functions can then be passed as callbacks to instances of the `Sensor` class.

If libraries that interface the sensor are not available, code to read values from them and process them can be implemented directly within functions defined in the separate file mentioned above.

#### Constructor
`Sensor sensorname(byte numOutputs)`

`numOutputs` - the number of outputs expected to be returned by the sensor.

e.g.
- a single ToF sensor gives one output, a number denoting the distance to the nearest obstacle.
- a single IR sensor gives one output, which may be a `float` denoting the strength of the reflected IR pulse, or a digital `1` or `0`, denoting if a certain color is detected or not.
- an array of 8 IR sensors gives eight outputs, each of which may be interpreted in the same way as above.
- a color sensor may give three analog values as outputs, corresponding to the detected intensities of red, green, and blue.

#### Methods
1. `setReadInt(void *readIntCallBack(*int, byte))`

    Specify the callback function to be called when the sensor is to be read, and integer outputs are expected.

    `*readIntCallBack(int *outputsArray, byte numOutputs)` - a pointer to a function (of return type `void`), that accepts a pointer to an integer array (`outputsArray`), and a `byte` specifying the array length, which reads _integer_ values from the sensor and places them in `outputsArray`

2. `setReadFloat(void *readFloatCallBack(*float, byte))`

    Specify the callback function to be called when the sensor is to be read, and floating point outputs are expected.

    `*readFloatCallBack(float *outputsArray, byte numOutputs)` - a pointer to a function (of return type `void`), that accepts a pointer to an floating point array (`outputsArray`), and a `byte` specifying the array length, which reads _float_ values from the sensor and places them in `outputsArray`.

3. `readInt()`

    Calls the function specified in `readIntCallBack` and places the values read in a private integer array.

4. `readFloat()`

    Calls the function specified in `readFloatCallBack` and places the values read in a private float array.
   
## `PIDController` Class

#### Constructor
`PIDController pidcontrollername(float K_P, float K_I, float K_D)`

Instantiate a `PIDController` object. Keeps track of an error signal provided to it, and generates a PID signal. Accepts `float`s specifying the constants $K_P$, $K_I$, and $K_D$ for the proportional, integral, and derivative terms, to generate the PID signal.

#### Methods
1. `getPID(float error)`

    Returns a `float` corresponding to a PID signal generated based on the provided error, and the integral and derivative of the error signal seen so far, weighed down by the constants specified in the constructor.

## `Gripper` Class

## `Robot` Class
The `Robot` class defines a single entity representing an entire mobile robot. A `Robot` has 
1. `Wheel`s, that it uses to move,
2. a `Gripper`, which it can use to grab objects,

#### Constructor
`Robot robotName()`

The constructor takes no arguments. A `Robot` object has an instance of `PIDController`, to generate PID signals to generate control signals for its wheels.

#### Methods
1. `attachWheels(Wheel_t *leftWheels, byte numLeftWheels, Wheel_t *rightWheels, byte numRightWheels)`

    Two `Wheel_t` arrays, one containing `Wheel_t`s corresponding to all the wheels on the left, and the other, to all the wheels on the right. The number of wheels on each side must also be passed.

2. `initPIDController(float K_P, float K_I, float K_D)`

    Instantiates a `PIDController` object for the robot, based on the constants provided.

3. `setBaseSpeed(float speed)`

    Sets a base speed for all the wheels of the robot.

4. `setRightSpeed(float speed)`

    Changes the speed of only the right wheels to the given value.

5. `setLeftSpeed(float speed)`

    Changes the speed of only the left wheels to the given value.

6. `drive(byte direction)`

    Drives in the indicated direction at the specified base speed, by setting the speed of all wheels to that value. `direction` can be `FORWARD` or `BACKWARD`, which are constant keywords defined within the library.

7. `driveDistance(byte direction, int distance)`

    Drives in the indicated direction at the specified base speed, until the distance specified by `distance` is covered.
    
8. `brake()`

    Hard-brakes the robot.

9. `turn(int radius, int angle)`
   
    Gives appropriate speeds to the wheels such that the robot turns through an arc with the given radius,
    until it turns through the specified angle.

10. `followLine(float error)`

    Generates and writes PID signals to the wheels so as to follow a line. `error` is a float denoting how far off the robot is from the line, and must be calculated externally based on inputs from an IR sensor array.

## `Logger` Class

#### Methods
1. `log`

   This method is defined to arrange the debugging info into a proper format that includes the user message, file name and currently executing function. Built-in arduino macros were used for this.

2. `printTime`

   This is a private method that is used to track the time that has executed since the start of the program and print that information. This can be used to identify errors in high speed sequences.

#### Usage

A seperate function called `LOG_MESSAGE(msg)` is defined for user convenience and by passing a message to that function all the above mentioned information will be printed automatically.

Example: `LOG_MESSAGE("System initialized");`

This will give the following output

`[TIME: 12345 ms] [sample.ino] [LINE: 10] [FUNCTION: setup] System initialized`


   
   
