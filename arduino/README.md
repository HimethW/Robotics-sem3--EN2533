## `Wheel` Class

#### Constructor
`Wheel wheelName(byte controlPinA, byte controlPinB, byte speedPin)`

A motor controlled through an H-bridge requires two inputs (from digital pins) to determine the direction in which to rotate,
and one input to set the speed to rotate at (from a PWM pin).

#### Methods
1. `forward(int speed)`

    Writes `HIGH` to `controlPinA`, `LOW` to `controlPinB`, and `speed` to `speedPin`.

2. `backward(int speed)`

    Writes `LOW` to `controlPinA`, `HIGH` to `controlPinB`, and `speed` to `speedPin`.

3. `stop()`

    Writes `0` to `speedPin`.

4. `brake()`

    Writes `LOW` to both `controlPinA` and `controlPinB`.

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
   
## `LineFollower` Class

## `Gripper` Class

## `Robot` Class
The `Robot` class defines a single entity representing an entire mobile robot. A `Robot` has 
1. `Wheel`s, that it uses to move,
2. a `Gripper`, which it can use to grab objects,

#### Constructor
`Robot robotName()`

The constructor takes no arguments.

#### Methods
1. `attachGripper(Gripper gripper)`

    A `Gripper` object must be given.

2. `attachWheels(Wheel rearLeft, Wheel forwardLeft, Wheel forwardRight, Wheel rearRight)`

    Four `Wheel` objects, defining the four wheels of the robot must be given.

3. `drive(int speed)`

    Makes the robot move at the specified speed, overriding any signals coming from the controller.
    A positive value for `speed` causes the robot to drive forward, and a negative value causes it to drive
    backward, in each case at a speed proportional to the absolute value of `speed`. If `speed` is `0`, the
    robot soft-brakes.

4. `brake()`

    Hard-brakes the robot.

5. `turn(int radius, int angle)`
   
    Gives appropriate speeds to the wheels such that the robot turns through an arc with the given radius,
    until it turns through the specified angle.
