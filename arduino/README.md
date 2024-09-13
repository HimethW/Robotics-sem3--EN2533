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

## `LineFollower` Class

## `Gripper` Class

## `Robot` Class
The `Robot` class defines a single entity representing an entire mobile robot. A `Robot` has 
1. a 'controller', according to whose control signals it moves, which in this case will be a `LineFollower` object,
2. a `Gripper` object, which it can use to grab objects,
3. `Wheel`s, that it uses to move.

#### Constructor
`Robot robotName()`

The constructor accepts no arguments.

#### Methods
1. `attachController(LineFollower lineFollower)`

    A `LineFollower` object must be passed in.

2. `attachGripper(Gripper gripper)`

    A `Gripper` object must be given.

3. `attachWheels(Wheel rearLeft, Wheel forwardLeft, Wheel forwardRight, Wheel rearRight)`

    Four `Wheel` objects, defining the four wheels of the robot must be given.

4. `drive(int speed)`

    Makes the robot move at the specified speed, overriding any signals coming from the controller.
    A positive value for `speed` causes the robot to drive forward, and a negative value causes it to drive
    backward, in each case at a speed proportional to the absolute value of `speed`. If `speed` is `0`, the
    robot soft-brakes.

6. `brake()`

    Hard-brakes the robot.

7. `turn(int radius, int angle)`
   
    Gives appropriate speeds to the wheels such that the robot turns through an arc with the given radius, until
    it turns through the specified angle.
