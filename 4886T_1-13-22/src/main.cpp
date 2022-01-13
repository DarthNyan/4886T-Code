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
// DriveRF              motor         12              
// DriveLF              motor         19              
// MoGo                 motor         7               
// Controller1          controller                    
// RollerIntake         motor         9               
// DriveLB              motor         20              
// DriveRB              motor         11              
// FourBar              motor         10              
// Gyro                 inertial      3               
// LimitMogo            limit         H               
// FourBarB             motor         1               
// Vision               vision        13              
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

int AutonSelect = PlatformAuton;
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Gyro.calibrate(); //calibrate gyro sensor
  while(true)
  {
    //touchscreen autonomous selection
    Brain.Screen.clearScreen();
    Brain.Screen.drawLine(200, 0, 200, 200); //draw a line through the middle of the screen
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Auton: ");
    //Display which autonomous program will be run
    switch(AutonSelect)
    {
      case AWPlineAuton:
        Brain.Screen.print("AWP Line");
        break;
      case PlatformAuton:
        Brain.Screen.print("Platform");
        break;
      case NeutralAuton:
        Brain.Screen.print("Neutral Goal");
        break;
      case FullAWP:
        Brain.Screen.print("Full AWP");
        break;
      case AWP_Platform:
        Brain.Screen.print("AWP-Platform");
        break;
      case DebugAuton:
        Brain.Screen.print("Debug");
        break;
      case SkillsAuton:
        Brain.Screen.print("Skills");
        break;
      default:
        Brain.Screen.print("Error");
    }
    Brain.Screen.setCursor(1, 40);
    Brain.Screen.print("Enter");

    if(Brain.Screen.pressing()) //upon touching the screen
    {
      if(Brain.Screen.xPosition() < 200) //if the touch is on the left side
      {
        //cycle through autonomus programs
        if(AutonSelect  >= GreatestAuton) AutonSelect = LeastAuton;
        else AutonSelect ++;

        while(Brain.Screen.pressing()){}
      }
      //if on the right side, exit. AutonSelect is now locked in.
      else break;
    }
    wait(20, msec);
  }
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
int mogoTarg = 0, fourTarg = 0, gyroHeading;
void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  gyroHeading = 0; //initialize encoders
  FourBar.setPosition(0, degrees);
  MoGo.setPosition(0, degrees);
  mogoTarg = 0;
  fourTarg = 0;
  //AutonSelect determines which routine to run. Each case is a routine.
  switch(AutonSelect)
  {
    case AWPlineAuton:
      AutonDrive(0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);  //drive to mogo
      MoGo.setPosition(MOGO_JUNK, degrees); 
      AutonMogo(MOGO_HOLD, 0);  //pick up mogo 
      AutonTurn(-pi, 11*inches, DRIVE_MID, DRIVE_MID_ACCEL);  //turn to face line of rings
      Rollers(ROLLER_SPEED);  //roll intake and grab line of rings
      AutonDrive(-2*tiles, DRIVE_SLOW, DRIVE_LOW_ACCEL); 
      AutonDrive(2*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);  //drive back into home zone
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2); //set down and back up
      AutonDrive(-0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      break;
    case NeutralAuton:
      MoGo.setPosition(-170, degrees);
      AutonDrive(2*tiles - 6*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL);  //drive to low neutral goal
      MoGo.setPosition(MOGO_JUNK, degrees);
      AutonMogo(MOGO_HOLD, 0);  //pick up neutral goal & drive back
      AutonDrive(-1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(-pi/2, WHEEL_TO_WHEEL_DIST/2, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonDrive(8*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec); //place mogo down & back up
      AutonDrive(-4*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST/2, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face alliance goal.
      AutonDrive(20*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //go to alliance goal and pick up
      AutonMogo(MOGO_HOLD, Msec_TO_sec * 1.2);
      Rollers(ROLLER_SPEED); //roll intake to score preloads
      AutonDrive(-1*tiles, DRIVE_MID, DRIVE_LOW_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2); //set down and back up
      AutonDrive(-0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      Rollers(0); //stop rollers
      Mogo(0);
      break;
    case PlatformAuton:
      MoGo.setPosition(MOGO_JUNK, degrees); //grab mogo, score 1 ring in base
      AutonMogo(MOGO_HOLD, Msec_TO_sec * 2);
      Rollers(ROLLER_SPEED);
      AutonDrive(-6*inches, DRIVE_SLOW, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec);
      AutonDrive(-0.5*tiles, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonTurn(-atan((float)45/15), WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonDrive(50*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      AutonDrive(-1.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec);
      break;
    case FullAWP:
      AutonMogo(MOGO_MID, Msec_TO_sec/2); // score 1 ring in base of mogo
      mogoTarg = MOGO_GROUND; //set mogo back down
      AutonDrive(-20*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //bump against wall
      AutonTurn(-pi/2, WHEEL_TO_WHEEL_DIST + 6*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face other mogo
      AutonTurn(0, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonDrive(4.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //go to other mogo & pick it up
      MoGo.setPosition(MOGO_JUNK, degrees);
      AutonMogo(MOGO_HOLD, 1.5*Msec_TO_sec);
      AutonTurn(0, WHEEL_TO_WHEEL_DIST/2, DRIVE_FAST, DRIVE_MID_ACCEL, true); //correct steering incase collision with mogo knocked it off 
      Rollers(ROLLER_SPEED); //roll intake to score preloads
      AutonDrive(-1*tiles, DRIVE_MID, DRIVE_LOW_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2); //set down and back up
      AutonDrive(-0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      Rollers(0); //stop rollers
      Mogo(0);
      break;
    case AWP_Platform:
      MoGo.setPosition(MOGO_JUNK, degrees);
      mogoTarg = MOGO_MID; //close claw on mogo and set ring down
      AutonDrive(4*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //drive over platform carrying mogo
      AutonMogo(MOGO_GROUND, Msec_TO_sec); //set mogo down
      AutonDrive(-(1*tiles - 10*inches), DRIVE_FAST, DRIVE_MID_ACCEL); //bump agains platform
      AutonTurn(-pi/5, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face other mogo
      AutonDrive(1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to mogo
      MoGo.setPosition(MOGO_JUNK, degrees); //pick up mogo
      AutonMogo(MOGO_HOLD, Msec_TO_sec*1.5);
      Rollers(ROLLER_SPEED); //spin rollers to score rings
      AutonDrive(-18*inches, DRIVE_MID, DRIVE_LOW_ACCEL);//back up
      AutonMogo(MOGO_GROUND, Msec_TO_sec);//set mogo down
      Rollers(0);//stop rollers
      AutonDrive(-6*inches, DRIVE_MID, DRIVE_LOW_ACCEL);//back up
      break;
    case SkillsAuton:
      MoGo.setPosition(MOGO_JUNK, degrees); //grab red goal
      mogoTarg = MOGO_HOLD;
      AutonDrive(6*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi/2 + atan(0.25), WHEEL_TO_WHEEL_DIST + 6*inches, DRIVE_MID, DRIVE_MID_ACCEL, true); //turn to face rear towards neutral goal
      Rollers(ROLLER_SPEED); //score rings
      AutonDrive(-3*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //push neutral goal & move towards red platform
      //AutonDrive(12*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST - 4*inches, DRIVE_SLOW, DRIVE_MID_ACCEL); //face rollers toward stars of rings
      AutonDrive(-1.7*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //drive over stars and push them back, score a few
      Rollers(0);
      fourTarg = FOUR_UP; //raise goal & turn to face red platform
      mogoTarg = MOGO_MID;
      AutonTurn(7*pi/5, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonDrive(0.9*tiles + 6*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to red platform & score goal
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonDrive(-12*inches, DRIVE_MID, DRIVE_MID_ACCEL); //back up
      fourTarg = FOUR_GROUND; //lower lift
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST + 3*inches, DRIVE_MID, DRIVE_MID_ACCEL); //face neutral goal, pick it up
      AutonDrive(12*inches, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      AutonDrive(-1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //bring neutral goal back
      fourTarg = FOUR_UP; //raise it
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL, true); //face platform
      AutonDrive(12*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //score goal
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonDrive(-12*inches, DRIVE_MID, DRIVE_MID_ACCEL); //back up
      fourTarg = FOUR_GROUND; //lower lift
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL); //face blue goal
      AutonDrive(2.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to blue goal and grab it
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2);
      AutonDrive(-12*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL); //turn to face line of rings
      Rollers(ROLLER_SPEED); //roll intake
      AutonDrive(-2*tiles - 16*inches, DRIVE_MID, DRIVE_MID_ACCEL); //drive over line of rings
      Rollers(-ROLLER_SPEED); //spin rollers out to clear potential jams
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST/2, DRIVE_MID, DRIVE_MID_ACCEL); //drive over star of rings
      Rollers(ROLLER_SPEED);
      AutonDrive(-2.1*tiles, DRIVE_MID, DRIVE_MID_ACCEL);
      mogoTarg = MOGO_MID; //raise lift
      fourTarg = FOUR_UP;
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST/2, DRIVE_SLOW, DRIVE_MID_ACCEL); //score goal
      AutonDrive(1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //make sure pointing straight
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonDrive(-2.5*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //push tall goal into red zone
      fourTarg = FOUR_GROUND; //lower lift, spin rollers out
      Rollers(-ROLLER_SPEED);
      AutonDrive(2.6*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //drive back to platform
      AutonTurn(0, WHEEL_TO_WHEEL_DIST - 2*inches, DRIVE_MID, DRIVE_MID_ACCEL, true); //face & go to red goal
      AutonDrive(2.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2); //grab red goal
      Rollers(ROLLER_SPEED);
      AutonTurn(pi/3, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //point rear towards 3rd neutral
      AutonDrive(-2*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //push neutral goal into red zone
      AutonTurn(-pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to put red gaol in red zone
      AutonDrive(1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      break;
    case DebugAuton:
      while(true)
      {
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print(GyroAbsPos());
        wait(FiftiethSecondTick, msec);
      }
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
  bool driveReversed = false;
  bool buttonToggle = false;
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
      if(!buttonToggle)
      {
          if(driveReversed)
          {
            driveReversed = false;
          }
          else driveReversed = true;
      }
      buttonToggle = true; 
    }
    else 
    {
      //set buttonToggle to false to prepare for next press of button
      buttonToggle = false;
    }
    //controller has two joysticks, one on the left and one on the right. 
    //the right joystick controls the right side of the drive and the left controls the left
    if (driveReversed)
    {
      //inverted drive, swap joysticks and invert them
      //This will now drive as though forward is backward
      lpow = -RightAxis * AXIS_TO_RPM;
      rpow = -LeftAxis * AXIS_TO_RPM;
    }
    else
    {
      //forward drive.
      lpow = LeftAxis * AXIS_TO_RPM;
      rpow = RightAxis * AXIS_TO_RPM;
    }

    //deadband, if moving too slow, don't bother moving at all
    if(lpow < DEADBAND && lpow > -DEADBAND) lpow = 0;
    if(rpow < DEADBAND && rpow > -DEADBAND) rpow = 0;
    Drive(lpow, rpow); //spin the motors

    //Fourbar PID
    fourTarg += (FourBarButton * TORQUE_ROTATION_PER_TICK * Shift); //move the fourbar up and down manually
    if(XButton) fourTarg = FOUR_UP; //Move the fourbar to preset heights by pressing controller buttons
    else if(YButton) fourTarg = FOUR_LOW;
    else if(BButton) fourTarg = FOUR_GROUND;
    FourbarPID(); //spin the fourbar to target

    //Mogo PID, same as fourbar control.
    mogoTarg += MoGoButton * TORQUE_ROTATION_PER_TICK * Shift;
    if(DownButton) mogoTarg = MOGO_GROUND;
    else if(LeftButton) 
    {
      //limit switch will indicate whether MOGO_HOLD has been reached. 
      //set position to a junk value that allows it to spin until it hits the limit switch
      MoGo.setPosition(MOGO_JUNK, degrees);
      mogoTarg = MOGO_HOLD;
    }
    else if(RightButton) mogoTarg = MOGO_MID;
    MogoPID(); //spin the fourbar to target
    
    Rollers(RollerButton * ROLLER_SPEED * Shift); //basic roller control

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
  DriveLB.setStopping(coast);
  DriveRF.setStopping(coast);
  DriveRB.setStopping(coast);

  DriveLF.spin(forward, lspd, rpm);
  DriveLB.spin(forward, lspd, rpm);
  DriveRF.spin(forward, rspd, rpm);
  DriveRB.spin(forward, rspd, rpm);
}
void Mogo(int spd)
{
  MoGo.spin(forward, spd, rpm);
}
void Rollers(int spd)
{
  RollerIntake.spin(forward, spd, rpm);
}
void Fourbar(int spd)
{
  FourBar.spin(forward, spd, rpm);
  FourBarB.spin(forward, spd, rpm);
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
void AutonDrive(int dist, int maxVel, int accel)
{
  //initialize variables
  float lEnc, rEnc;
  float lStart = DriveLB.position(turns), rStart = DriveRB.position(turns);
  float vel = 0, pos = 0;
  bool decel = false;
  int tickSpeed = FiftiethSecondTick, currentTick = VexTimer;
  if(dist > 0)
  {
    while(vel >= 0) //driving forward
    {
      //get dist encoders have traveled since the function call in inches
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL; 
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      
      //if we've at the distance to start stopping, 
      if(dist - pos <= StopDistance(vel, accel)) 
      {
        vel -= (float)accel / TicksPerSecond; //decrement velocity
      }
      else if (vel < maxVel) //otherwise, if we're not at our maximum velocity
      {
        vel += (float)accel / TicksPerSecond; //increment velocity
      }
      //ensure velocity does not go over maxVel. 
      //It is possible to increment vel past maxVel, so this ensures vel never exceeds its maximum.
      vel = limitFloatToMax(vel, maxVel);

      //increment position by velocity
      pos += vel / TicksPerSecond; 

      //Spin the motors
      Drive(vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //drive at vel in/s, converting in/s to rpm
              + DRIVE_KP * (pos - lEnc) //P loop corrects for errors in position
              - STEERING_KP * (GyroAbsPos() - gyroHeading), //P loop for correcting angle, should angle be off desired course
            vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL //right half is same as left half
              + DRIVE_KP * (pos - rEnc)
              + STEERING_KP * (GyroAbsPos() - gyroHeading)
           );
      
      //debugging, graphs the selected variables on the brain screen
      GraphDebuggerVars(StopDistance(vel, accel), dist - pos, dist, pos, lEnc); 

      MogoPID(); //hold mogo and fourbar at their PID targets
      FourbarPID();

      currentTick += tickSpeed; //wait until a fiftieth of a second has passed since the last iteration through the loop.
      while(VexTimer < currentTick) {} //this ensures constant loop time
    }
  }
  else if(dist < 0) //driving backward. Same as driving forward, but a couple of variables are reversed
  {
    while(vel <= 0)
    {
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      //sign of StopDistance and comparison are now reversed. dist - pos should now be negative
      if(dist - pos >= -StopDistance(vel, accel) || decel == true) 
      {
        vel += (float)accel / TicksPerSecond; //sign of vel is reversed,
        decel = true;
      }
      else if (vel > -maxVel)
      {
        vel -= (float)accel / TicksPerSecond;
      }
      //rest of the code is the same as drive forward. Negative vel now causes pos to move negative
      vel = limitFloatToMax(vel, maxVel);
      pos += vel / TicksPerSecond;

      Drive(DRIVE_KP * (pos - lEnc)
              + vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL
              - STEERING_KP * (GyroAbsPos() - gyroHeading),
            DRIVE_KP * (pos - rEnc)
              + vel * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL
              + STEERING_KP * (GyroAbsPos() - gyroHeading)
           );
      
      GraphDebuggerVars(StopDistance(vel, accel), -(dist - pos), -dist, -pos, -lEnc);

      MogoPID();
      FourbarPID();

      currentTick += tickSpeed;
      while(VexTimer < currentTick) {}
    }
  }
  Drive(0, 0); //stop the drive at the end to prevent drifting
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
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence * ENC_TO_WHEEL; //get dist encoders have traveled in inches
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence * ENC_TO_WHEEL;
      //angle left is equal to the target angle - angle traveled in radians
      angleLeft = radians - (GyroAbsPos()) / rad_TO_deg; 

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


      MogoPID(); //hold mogo and fourbar
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

        
        MogoPID(); //hold mogo and fourbar
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
int GyroRollOvers = 0, PreviousGyro = 0, CurrentGyro;
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
  return (CurrentGyro + GyroRollOvers * DegreesPerTurn);

}

//Simple Mobile Goal (MoGo) P loop.
void MogoPID()
{
  //If the limit switch is pressed, the mogo is at MOGO_HOLD position. 
  //MOGO_HOLD is defined as up against the limit switch 
  if(LimitMogo.pressing()) MoGo.setPosition(MOGO_HOLD, degrees);
  
  Mogo(mogoTarg - MoGo.position(degrees)); //simple P loop
}

//Simple P loop for the fourbar
void FourbarPID()
{
  Fourbar(fourTarg - FourBar.position(degrees)); 
}

//Spins the mogo lift to a target during autonomus while doing nothing else for the duration
void AutonMogo(int pos, int duration)
{
  mogoTarg = pos; //set mogoTarg
  for(int i = 0; i < duration / FiftiethSecondTick; i ++)
      {
        MogoPID(); //while the duration hasn't been reached, call mogoPID and wait
        wait(FiftiethSecondTick, msec);
      }
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
  DriveLF.stop(hold);
  DriveRB.stop(hold);
  DriveRF.stop(hold);
}
int oldPitch = 0, robotPitch; 
void Park()
{
  robotPitch = Gyro.orientation(roll, degrees); //gyro is mounted sideways so its roll is robot's pitch
  Drive(robotPitch*PARK_KP + (robotPitch - oldPitch)*PARK_KD, robotPitch*PARK_KP + (robotPitch - oldPitch)*PARK_KD);
  oldPitch = robotPitch;
}