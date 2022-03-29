/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Tate Whiteberg                                            */
/*    Created:      Sep. 22 2021                                              */
/*    Last Updated: Nov. 28 2021                                              */
/*    Description:  Main robot code                                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// DriveRF              motor         11              
// DriveLF              motor         14              
// Controller1          controller                    
// RollerIntake         motor         9               
// DriveLB              motor         15              
// DriveRB              motor         12              
// Gyro                 inertial      3               
// FourBar              motor         10              
// DriveLM              motor         17              
// DriveRM              motor         16              
// RearMogo             digital_out   H               
// FrontMogo            digital_out   G               
// Vision               vision        1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "4886TDefine.h"
#include "../../V5StandardDefines.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

int AutonSelect = AWPlineAuton, AutonSide = BLUE_SIDE;
bool rearMogoOn = false, frontMogoOn = false;
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  OffRearMogo();
  OnFrontMogo();
  Gyro.calibrate(); //calibrate gyro sensor
  AutonGui();
  Brain.Screen.clearScreen();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
int fourTarg = 0, gyroHeading;
float steeringKD = STEERING_KD_LIGHT, stoppedDist = 0;
void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  gyroHeading = 0; //initialize encoders
  gyroOffset += GyroAbsPos();
  FourBar.setPosition(0, degrees);
  fourTarg = FOUR_GROUND;
  //AutonSelect determines which routine to run. Each case is a routine.
  switch(AutonSelect)
  {
    case AWPlineAuton:
      OnRearMogo(); //open claws
      OffFrontMogo();
      RaceForward(42*inches); //goal rush
      OnFrontMogo();
      stoppedDist = StopAndRetDist();
      fourTarg = FOUR_HOVER;
      AutonDrive(-36*inches- stoppedDist, DRIVE_FAST, DRIVE_HIGH_ACCEL); //bring goal back
      AutonTurn(-pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL); //face alliance goal
      FindMogo(AutonSide); //find alliance goal
      OffRearMogo();
      fourTarg = FOUR_UP; //raise lift
      AutonTurn(0, WHEEL_TO_WHEEL_DIST + 1*inches, DRIVE_FAST, DRIVE_LOW_ACCEL); //face line of rings
      Rollers(ROLLER_SPEED); //spin rollers
      wait(200, msec);
      AutonDrive(30*inches, DRIVE_MID, DRIVE_MID_ACCEL); //score rings
      AutonDrive(-42*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //bring goal back
      Rollers(0);
      OnRearMogo(); //set goal down
      Fourbar(0);
      wait(1000, msec);
      fourTarg = FOUR_GROUND;
      AutonDrive(3*inches, DRIVE_SLOW, DRIVE_MID_ACCEL);
      Fourbar(0);
      break;
    case NeutralAuton:
      OnRearMogo(); //open claws
      OffFrontMogo();
      RaceForward(42*inches); //goal rush
      OnFrontMogo();
      StopAndRetDist();
      fourTarg = FOUR_HOVER;
      AutonTurn(3*pi/4, 20*inches, DRIVE_FAST, DRIVE_MID_ACCEL, true); //turn towards
      FindMogo(NEUTRAL_SIDE);
      OffRearMogo();
      //AutonTurn(3*pi/4, WHEEL_TO_WHEEL_DIST/2, DRIVE_MID, DRIVE_LOW_ACCEL);
      gyroHeading = rad_TO_deg * 3*pi/4;
      AutonDrive(48, DRIVE_FAST, DRIVE_MID_ACCEL);
      break;
    case DebugAuton:
      OnRearMogo();
      wait(2, sec);
      FindMogo(0);
      OffRearMogo();
      break;
    default:
      break;
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  bool driveSlow = false;
  bool buttonAToggle = false, buttonLeftToggle = false, buttonDownToggle = false;
  int lpow, rpow;
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    if(AButton) //button A toggles drive reverse
    {
      //checking buttonToggle and setting it to true later ensures the code is executed exactly once per press of button A
      if(!buttonAToggle)
      {
          if(driveSlow)
          {
            driveSlow = false;
          }
          else driveSlow = true;
      }
      buttonAToggle = true; 
    }
    else 
    {
      //set buttonToggle to false to prepare for next press of button
      buttonAToggle = false;
    }
    //forward drive.
    lpow = LeftAxis * AXIS_TO_RPM;
    rpow = RightAxis * AXIS_TO_RPM;

    if (driveSlow)
    {
      lpow /= 2;
      rpow /= 2;
    }

    //deadband, if moving too slow, don't bother moving at all
    if(lpow < DEADBAND && lpow > -DEADBAND) lpow = 0;
    if(rpow < DEADBAND && rpow > -DEADBAND) rpow = 0;

    if (driveSlow && lpow == 0 && rpow == 0)
    {
      StopDrive();
    }
    else
    {
      Drive(lpow, rpow); //spin the motors
    }
    //Fourbar PID
    fourTarg += ((FourBarUpButton - FourBarDownButton) * TORQUE_ROTATION_PER_TICK); //move the fourbar up and down manually
    if(XButton) fourTarg = FOUR_UP; //Move the fourbar to preset heights by pressing controller buttons
    else if(YButton) fourTarg = FOUR_LOW;
    else if(BButton) fourTarg = FOUR_GROUND;
    FourbarPID(); //spin the fourbar to target
    
    Rollers((RollerInButton - RollerOutButton) * ROLLER_SPEED); //basic roller control

    if(RearMogoButton) //button Down toggles rear mogo
    {
      //checking buttonToggle and setting it to true later ensures the code is executed exactly once per press of button A
      if(!buttonDownToggle)
      {
          ToggleRearMogo();
      }
      buttonDownToggle = true; 
    }
    else 
    {
      //set buttonToggle to false to prepare for next press of button
      buttonDownToggle = false;
    }
    if(FrontMogoButton) //button Left toggles front mogo
    {
      //checking buttonToggle and setting it to true later ensures the code is executed exactly once per press of button A
      if(!buttonLeftToggle)
      {
          ToggleFrontMogo();
      }
      buttonLeftToggle = true; 
    }
    else 
    {
      //set buttonToggle to false to prepare for next press of button
      buttonLeftToggle = false;
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

//the following functions just spin the motors at a certain rpm.
void Drive(int lspd, int rspd)
{
  DriveLF.setStopping(coast);
  DriveLM.setStopping(coast);
  DriveLB.setStopping(coast);
  DriveRF.setStopping(coast);
  DriveRM.setStopping(coast);
  DriveRB.setStopping(coast);

  DriveLF.spin(forward, lspd, rpm);
  DriveLM.spin(forward, lspd, rpm);
  DriveLB.spin(forward, lspd, rpm);
  DriveRF.spin(forward, rspd, rpm);
  DriveRM.spin(forward, rspd, rpm);
  DriveRB.spin(forward, rspd, rpm);
}
void Rollers(int spd)
{
  RollerIntake.spin(forward, spd, rpm);
}
void Fourbar(int spd)
{
  FourBar.spin(forward, spd, rpm);
}

//Returns the distance (in inches) required to decelerate to a certain speed (default 0)
//given current velocity (in/s) and acceleration (in/s^2)
float StopDistance(float currentVel, float accel, float targetVel = 0)
{
  return -(targetVel*targetVel - currentVel*currentVel)/(2*accel); 
  //derived from physics equation V^2 = (V0)^2 +  2a(X-X0)
  //set V to targetVel and solve for X-X0,
  // = (targetVel^2 - currentVel^2)/2*accel
}

//Drive straight function. Arguments are in inches, in/s, and in/s^2
void AutonDrive(int dist, int maxVel, int accel, bool doDecelerate, bool doLagBehind)
{
  //initialize variables
  float lEnc, rEnc;
  float lStart = DriveLB.position(turns), rStart = DriveRB.position(turns);
  float vel = (DriveLB.velocity(rpm) + DriveRB.velocity(rpm)) * 0.5 * ENC_TO_WHEEL / MedWheel_InPerSec_TO_RPM, pos = 0;
  int tickSpeed = FiftiethSecondTick, currentTick = VexTimer, numTicks = 0;
  int currentSteeringError = GyroAbsPos() - gyroHeading, previousSteeringError = currentSteeringError;
  bool decelerating = false;
  if(dist > 0)
  {
    while(vel >= 0 || decelerating == false) //driving forward
    {
      //get dist encoders have traveled since the function call in inches
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL; 
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      
      //if we've at the distance to start stopping, 
      if(dist - pos <= StopDistance(vel, accel) && doDecelerate) 
      {
        vel -= (float)accel / TicksPerSecond; //decrement velocity
        decelerating = true;
      }
      else if (vel < maxVel) //otherwise, if we're not at our maximum velocity
      {
        vel += (float)accel / TicksPerSecond; //increment velocity
      }
      //ensure velocity does not go over maxVel. 
      //It is possible to increment vel past maxVel, so this ensures vel never exceeds its maximum.
      vel = limitFloatToMax(vel, maxVel);

      //increment position by velocity
      //but don't if we're too far behind the target
      if ((lEnc + rEnc)/2 > pos - DRIVE_LAG_TOLERANCE || doLagBehind) pos += vel / TicksPerSecond; 
      
      if((lEnc + rEnc)/2 >= dist) break; //if average of encoders say target is reached, break;

      if(numTicks % D_UPDATE_TIME == 0) //every D_UPDATE_TIME ticks:
      {
        previousSteeringError = currentSteeringError; //update the D term
        currentSteeringError = GyroAbsPos() - gyroHeading;
      }
      //Spin the motors
      Drive(vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //drive at vel in/s, converting in/s to rpm
              + DRIVE_KP * (pos - lEnc) //P loop corrects for errors in position
              - STEERING_KP * currentSteeringError //PD loop for correcting angle, should angle be off desired course
              - steeringKD * (previousSteeringError - currentSteeringError),
            vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //right half is same as left half
              + DRIVE_KP * (pos - rEnc)
              + STEERING_KP * currentSteeringError
              + steeringKD * (previousSteeringError - currentSteeringError)
           );
      //debugging, graphs the selected variables on the brain screen
      GraphDebuggerVars(StopDistance(vel, accel), dist - pos, dist, pos, lEnc); 

      //hold fourbar at PID target
      FourbarPID();

      currentTick += tickSpeed; //wait until a fiftieth of a second has passed since the last iteration through the loop.
      while(VexTimer < currentTick) {} //this ensures constant loop time
      numTicks ++;
    }
  }
  else if(dist < 0) //driving backward. Same as driving forward, but a couple of variables are reversed
  {
    while(vel <= 0 || decelerating == false)
    {
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      //sign of StopDistance and comparison are now reversed. dist - pos should now be negative
      if(dist - pos >= -StopDistance(vel, accel) && doDecelerate) 
      {
        vel += (float)accel / TicksPerSecond; //sign of vel is reversed,
        decelerating = true;
      }
      else if (vel > -maxVel)
      {
        vel -= (float)accel / TicksPerSecond;
      }
      //rest of the code is the same as drive forward. Negative vel now causes pos to move negative
      vel = limitFloatToMax(vel, maxVel);

      if ((lEnc + rEnc)/2 < pos + DRIVE_LAG_TOLERANCE || doLagBehind) pos += vel / TicksPerSecond;

      if((lEnc + rEnc)/2 <= dist) break; //if average of encoders say target is reached, break;

      if(numTicks % D_UPDATE_TIME == 0)
      {
        previousSteeringError = currentSteeringError;
        currentSteeringError = GyroAbsPos() - gyroHeading;
      }
      //Spin the motors
     Drive(vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //drive at vel in/s, converting in/s to rpm
              + DRIVE_KP * (pos - lEnc) //P loop corrects for errors in position
              - STEERING_KP * currentSteeringError //PD loop for correcting angle, should angle be off desired course
              - steeringKD * (previousSteeringError - currentSteeringError),
            vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //right half is same as left half
              + DRIVE_KP * (pos - rEnc)
              + STEERING_KP * currentSteeringError
              + steeringKD * (previousSteeringError - currentSteeringError)
           );

      GraphDebuggerVars(5*currentSteeringError + 100, 5*previousSteeringError + 100, 5*steeringKD * (previousSteeringError - currentSteeringError) + 100);


      FourbarPID();

      currentTick += tickSpeed;
      while(VexTimer < currentTick) {}
      numTicks ++;
    }
  }
  if (doDecelerate) StopDrive(); //stop the drive at the end to prevent drifting
}

//Function for autonomus turning. Arguments are in radians, inches, in/s, and in/s^2.
//set reversed to true if you want to turn by driving backwards. Otherwise, robot will turn driving forwards
void AutonTurn(float radians, int outerRadius, int maxVel, int accel, bool reversed)
{
  //initialize variables
  float GyroStart = GyroAbsPos() / rad_TO_deg, angleLeft;
  //inner radius is directly proportional to outer radius,
  //inner wheel's position and velocity relative to outer wheel should follow the same proportion
  //that proportion is radiusRatio
  float innerRadius = outerRadius - WHEEL_TO_WHEEL_DIST, radiusRatio = innerRadius / outerRadius;
  float lEnc, rEnc, lStart = DriveLB.position(turns), rStart = DriveRB.position(turns);
  float outerVel = 0, outerPos = 0;
  int tickSpeed = FiftiethSecondTick, currentTick = VexTimer;

  if (radians > GyroStart) //turning right
    while (outerVel >= 0)
    {
      //get distance encoders have traveled since the function call in inches  
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      //angle left is equal to the target angle - angle traveled in radians
      angleLeft = radians - (GyroAbsPos() / rad_TO_deg); 

      //recall arc length = rΘ
      //if the distance left to travel, as determined by angleLeft * outerRadius,
      //is <= distance required to stop, decelerate
      if(angleLeft * outerRadius <= StopDistance(outerVel, accel)) 
      {
        outerVel -= (float)accel / TicksPerSecond;
        
      }
      else if (outerVel < maxVel) //else, if outervel < maxVel, accelerate
      {
        outerVel += (float)accel / TicksPerSecond; 
      }
      outerVel = limitFloatToMax(outerVel, maxVel);

      outerPos += outerVel / TicksPerSecond; //increment position target by velocity

      //turning uses input from two different sensors. If gyro says position is reached, break
      if(radians <= GyroAbsPos() / rad_TO_deg) break; 

      //Spin the motors
      if(reversed) //if turning backwards
        Drive(- outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //left wheel is inner wheel, spin reversed at innerVel = outerVel * radiusRatio
                - DRIVE_KP * (outerPos * radiusRatio + lEnc), //P loop to correct position
              - outerVel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //right wheel is outer wheel, spin reversed at outerVel
                - DRIVE_KP * (outerPos + rEnc) //P loop to correct position
             );
      else //if turning forwards
        Drive(outerVel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //left wheel is outer wheel,
                + DRIVE_KP * (outerPos - lEnc), //P loop to correct position
              outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //right wheel is inner wheel
                + DRIVE_KP * (outerPos * radiusRatio - rEnc) //P loop to correct position
             );

      //hold fourbar
      FourbarPID();

      GraphDebuggerVars(angleLeft * rad_TO_deg, angleLeft * outerRadius * 10, StopDistance(outerVel, accel) * 10);
      
      currentTick += tickSpeed; //wait until a fiftieth of a second has passed.
      while(VexTimer < currentTick) {}
    }
    else if(radians < GyroStart) //turning left
      while (outerVel >= 0) //very similar to right turn
      {
        //read encoders
        lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL; //get dist encoders have traveled in inches
        rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
        //angle left is equal to the target angle - angle traveled in radians, but negative this time
        angleLeft =  -(radians - (GyroAbsPos()) / rad_TO_deg); 
        //if the distance left to travel is <= distance to stop, decelerate
        if(angleLeft * outerRadius <= StopDistance(outerVel, accel)) 
        {
          outerVel -= (float)accel / TicksPerSecond;
        }
        else if (outerVel < maxVel) //else, if the outervel < max vel, accelerate
        {
          outerVel += (float)accel / TicksPerSecond; 
        }
        outerVel = limitFloatToMax(outerVel, maxVel);

        outerPos += outerVel / TicksPerSecond; //increment position target by velocity

        //turning uses input from two different sensors. If gyro says position is reached, break
        if(radians >= GyroAbsPos() / rad_TO_deg) break; 
        
        //Spin the motors, same as turning right but inner and outer are swapped
        if(reversed)
          Drive(- outerVel * MedWheel_InPerSec_TO_RPM * ENC_TO_WHEEL
                  - DRIVE_KP * (outerPos + lEnc),
                - outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM * ENC_TO_WHEEL
                  - DRIVE_KP * (outerPos * radiusRatio + rEnc)
               );
        else
          Drive(outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM * ENC_TO_WHEEL
                  + DRIVE_KP * (outerPos * radiusRatio - lEnc), 
                outerVel * MedWheel_InPerSec_TO_RPM * ENC_TO_WHEEL
                  + DRIVE_KP * (outerPos - rEnc) 
               );

        
        FourbarPID();
    
        currentTick += tickSpeed; //wait until a fiftieth of a second has passed.
        while(VexTimer < currentTick) {}
      }
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(GyroAbsPos());
  StopDrive(); //stop drive motors to prevent drift
  //set heading to desired turn angle (in degrees). This is useful for correcting steering in AutonDrive
  gyroHeading = radians * rad_TO_deg;
}

//graph variables on the brain screen. Call in a while loop.
//each iteration draws 1 pixel per color, up to 6 colors. No numeric values graphed
//good for comparing several different values as they change over time
int xcursor = 0;
void GraphDebuggerVars(int Red, int Blue, int Green, int Purple, int Orange, int Yellow)
{
  Brain.Screen.setPenColor(red);
  Brain.Screen.drawPixel(xcursor, Red);
  Brain.Screen.setPenColor(blue);
  Brain.Screen.drawPixel(xcursor, Blue);
  Brain.Screen.setPenColor(green);
  Brain.Screen.drawPixel(xcursor, Green);
  Brain.Screen.setPenColor(purple);
  Brain.Screen.drawPixel(xcursor, Purple);
  Brain.Screen.setPenColor(orange);
  Brain.Screen.drawPixel(xcursor, Orange);
  Brain.Screen.setPenColor(yellow);
  Brain.Screen.drawPixel(xcursor, Yellow);
  xcursor ++; //auto increment x cursor.
}

//Gyro.orientation(yaw, degrees) goes from 180 to -179 and vice versa.
//this returns the gyro's absolute yaw in degrees. Use instead of Gyro.orientation
int GyroRollOvers = 0, PreviousGyro = 0, CurrentGyro, gyroOffset = 0;
int GyroAbsPos()
{
  CurrentGyro = Gyro.orientation(yaw, degrees);
  //if the difference between the current gyro position and the previous > 300, gyro has rolled from -179 to 180
  //decrement gyroRollOvers because gyro is turning in negative direction
  if(CurrentGyro - PreviousGyro > GyroMarginOfRollOver) GyroRollOvers --;
  //same line of thinking for rolling from 180 to -179
  else if(CurrentGyro - PreviousGyro < -GyroMarginOfRollOver) GyroRollOvers ++;
  PreviousGyro = CurrentGyro;

  //returns the absolute yaw of the gyro in degrees.
  return (CurrentGyro + GyroRollOvers * DegreesPerTurn) - gyroOffset;

}

//Simple P loop for the fourbar
void FourbarPID()
{
  Fourbar(fourTarg - FourBar.position(degrees)); 
}

// limits a float to a maximum value.
float limitFloatToMax(float variable, float lim)
{
  if (lim < 0) lim *= -1; //if lim is negative, switch it to positive
  if (variable > lim) return lim; //if var is greater than positive limit, return limit
  else if (variable < -lim) return -lim; //if var is less than negative limit, return negative limit
  else return variable; //otherwise, var is within bounds. return it.
}
void TestRefreshRate()
{
  for(int tick = 0; tick < BrainScreenWidth; tick ++){
    Brain.Screen.setPenColor(red);
    Brain.Screen.drawPixel(tick, GyroAbsPos());
    Brain.Screen.setPenColor(blue);
    Brain.Screen.drawPixel(tick, (tick % 10 == 0) * 100);
    Drive(600, -600);
    wait(1, msec);
  }
  Drive(0, 0);
}
void StopDrive()
{
  DriveLB.stop(hold);
  DriveLM.stop(hold);
  DriveLF.stop(hold);
  DriveRB.stop(hold);
  DriveRM.stop(hold);
  DriveRF.stop(hold);
}
int oldPitch = 0, robotPitch; 
void Park()
{
  robotPitch = Gyro.orientation(roll, degrees); //gyro is mounted sideways so its roll is robot's pitch
  Drive(robotPitch*PARK_KP + (robotPitch - oldPitch)*PARK_KD, robotPitch*PARK_KP + (robotPitch - oldPitch)*PARK_KD);
  oldPitch = robotPitch;
}
float avg(int entries[], int len)
{
  int sum = 0;
  for(int i = 0; i < len; i ++)
  {
    sum += entries[i];
  }
  return (float)sum/len;
}
float StopAndRetDist()
{ 
  float lstart = DriveLB.position(turns), rstart = DriveRB.position(turns);
  while(DriveLB.velocity(rpm) + DriveRB.velocity(rpm) > 0)
  {
      Drive(-1, -1);
      FourbarPID();
  }
  return 0.5*((DriveLB.position(turns) - lstart) + (DriveRB.position(turns) - rstart)) * ENC_TO_WHEEL * MedWheel_Circumfrence;
}
void OnRearMogo()
{
  rearMogoOn = true;
  RearMogo.set(true);
}
void OffRearMogo()
{
  rearMogoOn = false;
  RearMogo.set(false);
}
void ToggleRearMogo()
{
  if(rearMogoOn)
  {
    OffRearMogo();
  }
  else
  {
    OnRearMogo();
  }
}
void OnFrontMogo()
{
  frontMogoOn = true;
  FrontMogo.set(true);
}
void OffFrontMogo()
{
  frontMogoOn = false;
  FrontMogo.set(false);
}
void ToggleFrontMogo()
{
  if(frontMogoOn)
  {
    OffFrontMogo();
  }
  else
  {
    OnFrontMogo();
  }
}
void RaceForward(int dist)
{
  float lStart = DriveLB.position(turns);
  float rStart = DriveRB.position(turns);
  float lEnc = 0, rEnc = 0;
  int currentSteeringError;
  while (0.5 * (lEnc + rEnc) < dist)
  {
    lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
    rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
    currentSteeringError = GyroAbsPos() - gyroHeading;

    Drive(BUTTON_TO_TURBO
            - STEERING_KP * currentSteeringError, 
          BUTTON_TO_TURBO
            + STEERING_KP * currentSteeringError

    );
  }
}
void FindMogo(int side)
{

  do
  {
    if(side == BLUE_SIDE) Vision.takeSnapshot(BLUE_MOGO);
    else if(side == RED_SIDE) Vision.takeSnapshot(RED_MOGO);
    else if(side == NEUTRAL_SIDE) Vision.takeSnapshot(YELLOW_MOGO);
    else break;

    Drive(Vision.objects[0].centerY - VISION_GOAL_PRIMED - DRIVE_MID*MedWheel_InPerSec_TO_RPM/ENC_TO_WHEEL
            - (VisionScreenWidth/2 - Vision.objects[0].centerX), 
          Vision.objects[0].centerY - VISION_GOAL_PRIMED - DRIVE_MID*MedWheel_InPerSec_TO_RPM/ENC_TO_WHEEL
            + VisionScreenWidth/2 - Vision.objects[0].centerX);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(Vision.objects[0].centerX);
    Brain.Screen.print(", ");
    Brain.Screen.print(Vision.objects[0].centerY);

    GyroAbsPos();
    wait(20, msec);
  }
  while (Vision.objects[0].centerY < VISION_GOAL_PRIMED);
  gyroHeading = GyroAbsPos();
  AutonDrive(-3*inches, DRIVE_MID, DRIVE_MID_ACCEL);
}
void SeeMogo(int side)
{
  while(true)
  {
    if(side == BLUE_SIDE) Vision.takeSnapshot(BLUE_MOGO);
    else if(side == RED_SIDE) Vision.takeSnapshot(RED_MOGO);
    else if(side == NEUTRAL_SIDE) Vision.takeSnapshot(YELLOW_MOGO);
    else break;

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(Vision.objects[0].centerX);
    Brain.Screen.print(", ");
    Brain.Screen.print(Vision.objects[0].centerY);

    GyroAbsPos();
    wait(20, msec);
  }
}
void AutonGui()
{
  while(true)
  {
    Brain.Screen.setFillColor(transparent);
    Brain.Screen.clearScreen();
    Brain.Screen.setPenWidth(2);
    Brain.Screen.setPenColor(green);
    Brain.Screen.drawRectangle(120,0,360,240);  // a line for the field outline
    Brain.Screen.drawLine(0, 100, 120, 100);
    drawHomeFieldLines();
    drawRingles();
    drawMogo(300, 60, 0);
    drawMogo(210, 60, 0);
    drawMogo(390, 60, 0);
    drawPlatform(AutonSide);
    drawMogo(450, 150, AutonSide);
    drawMogo(235, 210, AutonSide);
    Brain.Screen.setPenColor(white);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Change Auton");
    //Display which autonomous program will be run
    Brain.Screen.setPenColor(green);
    switch(AutonSelect)
    {
      case AWPlineAuton:
        Brain.Screen.drawLine(390, 200, 390, 60);
        Brain.Screen.drawLine(390, 60, 400, 150);
        Brain.Screen.drawLine(400, 150, 450, 150);
        Brain.Screen.drawLine(450, 150, 420, 60);
        Brain.Screen.drawLine(420, 60, 420, 180);
        break;
      case PlatformAuton:
        Brain.Screen.drawLine(200, 200, 210, 60);
        Brain.Screen.drawLine(210, 60, 230, 200);
        Brain.Screen.drawLine(230, 200, 150, 200);
        break;
      case NeutralAuton:
        Brain.Screen.drawLine(390, 200, 390, 60);
        Brain.Screen.drawLine(390, 60, 340, 80);
        Brain.Screen.drawLine(340, 80, 300, 60);
        Brain.Screen.drawLine(300, 60, 420, 180);
        break;
      case FullAWP:
        Brain.Screen.drawLine(200, 200, 200, 150);
        Brain.Screen.drawLine(200, 150, 450, 150);
        Brain.Screen.drawLine(450, 150, 420, 60);
        Brain.Screen.drawLine(420, 60, 420, 180);
        break;
      case CenterRingsAuton:
        Brain.Screen.drawLine(390, 200, 300, 60);
        Brain.Screen.drawLine(300, 60, 450, 150);
        Brain.Screen.drawLine(450, 150, 420, 60);
        Brain.Screen.drawLine(420, 60, 420, 180);
        break;
      case DebugAuton:
        Brain.Screen.setCursor(5, 29);
        Brain.Screen.print("Debug");
        break;
      case SkillsAuton:
      Brain.Screen.setCursor(5, 29);
        Brain.Screen.print("Skills");
        break;
      default:
      Brain.Screen.setCursor(5, 29);
        Brain.Screen.print("Error");
    }
    Brain.Screen.setPenColor(white);
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print("Change Side");

    if(Brain.Screen.pressing()) //upon touching the screen
    {
      if(Brain.Screen.xPosition() < 120) //if the touch is on the left side
      {
        if (Brain.Screen.yPosition() < 100)
        {
          //cycle through autonomus programs
          if(AutonSelect  >= GreatestAuton) AutonSelect = LeastAuton;
          else AutonSelect ++;
        }
        else
        {
          AutonSide *= -1;
        }
        while(Brain.Screen.pressing()){}
      }
      //if on the right side, exit. AutonSelect is now locked in.
      else return;
    }
    wait(20, msec);
  }
}

void drawMogo(int xcent, int ycent, int side)
{
  if (side == RED_SIDE)
  {
    Brain.Screen.setFillColor(red);
    Brain.Screen.setPenColor(red);
  }
  else if (side == BLUE_SIDE)
  {
    Brain.Screen.setFillColor(blue);
    Brain.Screen.setPenColor(blue);
  }
  else
  {
    Brain.Screen.setFillColor(yellow);
    Brain.Screen.setPenColor(yellow);
  }
  Brain.Screen.setPenWidth(2);
  Brain.Screen.drawCircle(xcent, ycent, 15);
}

void drawPlatform(int side)
{
  if (side == RED_SIDE) Brain.Screen.setPenColor(red);
  else Brain.Screen.setPenColor(blue);
  Brain.Screen.setPenWidth(6);
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.drawRectangle(250,180,100,50);
  Brain.Screen.setPenWidth(2);
}

void drawHomeFieldLines()
{
  Brain.Screen.setPenColor(white);
  Brain.Screen.setPenWidth(2);
  Brain.Screen.drawLine(120,120,480,120);
  Brain.Screen.drawLine(420,120,480,180);
}
void drawRingle(int xcent, int ycent)   
{ 
  Brain.Screen.setFillColor(transparent);      
  Brain.Screen.setPenColor(purple);  
  Brain.Screen.setPenWidth(2);      
  Brain.Screen.drawCircle(xcent, ycent, 3);
}
void drawRingleStar (int xcent, int ycent)
{
  drawRingle(xcent-7, ycent);
  drawRingle(xcent+7, ycent);
  drawRingle(xcent, ycent+7);
  drawRingle(xcent, ycent-7);
}
void drawRingles(void)
{  /* the Left L of ringles */
  drawRingle(130,60);
  drawRingle(147,60);
  drawRingle(163,60);
  drawRingle(180,60);
  drawRingle(180,45);
  drawRingle(180,30);
  drawRingle(180,15);
  drawRingle(180,0);
  /* six ringles in the middle */
  drawRingle(240,60);
  drawRingle(255,60);
  drawRingle(270,60);
  drawRingle(330,60);
  drawRingle(345,60);
  drawRingle(360,60);
  /* the right :L of ringles */
  drawRingle(470,60);
  drawRingle(453,60);
  drawRingle(437,60);
  drawRingle(420,60);
  drawRingle(420,75);
  drawRingle(420,90);
  drawRingle(420,105);
  drawRingle(420,120); 
  drawRingleStar(240,120);
  drawRingleStar(300,120);
}
