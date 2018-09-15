# summerbot-motionbase
control library for the summerbot's motion base

This library is meant to control the servos in the base of the summerbot.

the folowing fonctionnality are coded but none of them have been tested:

   moveForward(short d) -> move the robot in straight line
  
   rotate(double rad) -> rotate the robot by the given angle in radian
   
   interruptMovement() -> interrupt the movement by stopping the rotation of the steppers and call updateCoords()
   
   updateCoords() -> recalculate the coords depending on the current movement and his state
   
   goToCoords(short finalxPos, short finalyPos, double finalAngle) -> move the robot in the given place and the given angle
