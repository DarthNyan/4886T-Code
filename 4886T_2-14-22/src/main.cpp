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
// MoGo                 motor         14              
// Controller1          controller                    
// RollerIntake         motor         9               
// DriveLB              motor         20              
// DriveRB              motor         11              
// FourBar              motor         10              
// Gyro                 inertial      3               
// LimitMogo            limit         H               
// FourBarB             motor         1               
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

int AutonSelect = SkillsAuton, gyroOffset = 0;
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
float steeringKD = STEERING_KD_LIGHT, stoppedDist = 0;
void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  gyroHeading = 0; //initialize encoders
  gyroOffset += GyroAbsPos();
  FourBar.setPosition(0, degrees);
  MoGo.setPosition(0, degrees);
  mogoTarg = MOGO_GROUND;
  fourTarg = FOUR_GROUND;
  //AutonSelect determines which routine to run. Each case is a routine.
  switch(AutonSelect)
  {
    case AWPlineAuton:
      MoGo.setPosition(-150, degrees);
      AutonDrive(2*tiles - 7*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);  //drive to low neutral goal
      mogoTarg = MOGO_MID;
      AutonDrive(3*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);
      stoppedDist = StopAndRetDist(); //pick up neutral goal & drive back
      AutonDrive(-1*tiles - 3*inches - stoppedDist, DRIVE_FAST, DRIVE_HIGH_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonTurn(pi/2 - pi/12, WHEEL_TO_WHEEL_DIST - 3*inches, DRIVE_FAST, DRIVE_LOW_ACCEL, true); //turn to face alliance goal.
      AutonDrive(24*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //go to alliance goal and pick up
      MoGo.setPosition(MOGO_JUNK, degrees);
      AutonMogo(MOGO_HOLD, Msec_TO_sec);
      AutonTurn(pi/6, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //face line of rings
      Rollers(ROLLER_SPEED);
      AutonDrive(-1.2*tiles, DRIVE_SLOW, DRIVE_MID_ACCEL); //drive over line of rings, scoring them
      AutonMogo(MOGO_GROUND, Msec_TO_sec); //set goal down
      AutonDrive(4*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //drive back into zone
      AutonDrive(-4*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      Mogo(0); //stop rollers
      Rollers(0);
      break;
    case NeutralAuton:
      MoGo.setPosition(-150, degrees);
      AutonDrive(2*tiles - 7*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);  //drive to low neutral goal
      mogoTarg = MOGO_MID;
      AutonDrive(3*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);
      stoppedDist = StopAndRetDist(); //pick up neutral goal & drive back
      AutonDrive(-1*tiles - 3*inches - stoppedDist, DRIVE_FAST, DRIVE_HIGH_ACCEL); //pick up neutral goal & drive back
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST/2, DRIVE_MID, DRIVE_LOW_ACCEL);
      mogoTarg = MOGO_GROUND;
      AutonDrive(-1.3*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      //AutonTurn(0, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_LOW_ACCEL);
      //AutonDrive(0.9*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(-pi/4, 0.77*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonDrive(8*inches, DRIVE_FAST, DRIVE_MID_ACCEL, false);
      mogoTarg = MOGO_MID;
      AutonDrive(4*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL);
      //AutonMogo(MOGO_MID, Msec_TO_sec/3);
      AutonDrive(-2.3*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      //AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      break;
    case PlatformAuton:
      MoGo.setPosition(-150, degrees);
      AutonDrive(45*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);  //drive to low neutral goal
      mogoTarg = MOGO_MID;
      AutonDrive(3*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);
      stoppedDist = StopAndRetDist();  //pick up neutral goal & drive back
      AutonDrive(-48*inches - stoppedDist, DRIVE_FAST, DRIVE_HIGH_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST - 3*inches, DRIVE_SLOW, DRIVE_MID_ACCEL, true);
      AutonDrive(24*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_HOLD, Msec_TO_sec*2);
      Rollers(ROLLER_SPEED);
      AutonDrive(-24*inches, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec);
      Rollers(0);
      AutonDrive(3*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonDrive(-3*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      Mogo(0);
      break;
    case FullAWP:
      MoGo.setPosition(-160, degrees);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/4); // score 1 ring in base of mogo
      AutonDrive(-18*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //bump against wall
      AutonTurn(-pi/2, WHEEL_TO_WHEEL_DIST + 6*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face other mogo
      AutonTurn(0, WHEEL_TO_WHEEL_DIST + 5*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonDrive(4.3*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //go to other mogo & pick it up
      MoGo.setPosition(MOGO_JUNK, degrees);
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_MID_ACCEL, true); //face line of rings 
      Rollers(ROLLER_SPEED); //roll intake to score preloads and line
      AutonDrive(-1*tiles, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonDrive(1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec); //set down and back up
      AutonDrive(-0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      Rollers(0); //stop rollers
      Mogo(0);
      break;
    case AWP_Platform:
      MoGo.setPosition(MOGO_JUNK, degrees);
      mogoTarg = MOGO_HOLD;
      AutonDrive(6*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi, 1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL, true);
      Rollers(ROLLER_SPEED);
      wait(200, msec);
      AutonDrive(-1*tiles, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec);
      Rollers(0);
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true);
      AutonDrive(0.8*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(5*pi/3, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonDrive(4*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      AutonDrive(-1.8*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      break;
    case SkillsAuton:
      MoGo.setPosition(MOGO_JUNK, degrees); //grab red goal
      mogoTarg = MOGO_HOLD;
      AutonDrive(6*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi/2 + atan(0.25), WHEEL_TO_WHEEL_DIST + 6*inches, DRIVE_FAST, DRIVE_MID_ACCEL, true); //turn to face rear towards neutral goal
      Rollers(ROLLER_SPEED); //score rings
      AutonDrive(-3.1*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //push neutral goal & move towards red platform
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST - 4*inches, DRIVE_SLOW, DRIVE_LOW_ACCEL); //face rollers toward stars of rings
      AutonDrive(-1.8*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //drive over stars and push them back, score a few
      Rollers(0);
      wait(100, msec);
      fourTarg = FOUR_UP; //raise goal & turn to face red platform
      mogoTarg = MOGO_MID;
      AutonTurn(7*pi/5, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_LOW_ACCEL);
      AutonDrive(0.6*tiles + 6*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to red platform & score goal
      //fourTarg = FOUR_LOW;
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true);
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      //fourTarg = FOUR_UP;
      AutonDrive(-12*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //back up
      fourTarg = FOUR_GROUND; //lower lift
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST + 3*inches, DRIVE_MID, DRIVE_MID_ACCEL); //face neutral goal, pick it up
      wait(100, msec);
      AutonDrive(5*inches, DRIVE_MID, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      AutonDrive(-15*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //bring neutral goal back
      fourTarg = FOUR_UP; //raise it
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_FAST, DRIVE_LOW_ACCEL, true); //face platform
      AutonDrive(12*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //score goal
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonDrive(-12*inches, DRIVE_MID, DRIVE_MID_ACCEL); //back up
      fourTarg = FOUR_GROUND; //lower lift
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL); //face blue goal
      wait(100, msec);
      AutonDrive(2.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to blue goal and grab it
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2);
      //AutonDrive(-12*inches, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(3*pi/2, WHEEL_TO_WHEEL_DIST + 2*inches, DRIVE_FAST, DRIVE_MID_ACCEL, true); //turn to face line of rings
      wait(100, msec);
      MoGo.setPosition(MOGO_JUNK, degrees);
      Rollers(ROLLER_SPEED); //roll intake
      AutonDrive(-2.2*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //drive over line of rings
      //Rollers(-ROLLER_SPEED); //spin rollers out to clear potential jams
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST/2, DRIVE_SLOW, DRIVE_LOW_ACCEL, true); //drive over star of rings
      Rollers(ROLLER_SPEED);
      AutonDrive(-2*tiles, DRIVE_MID, DRIVE_MID_ACCEL);
      wait(100, msec);
      mogoTarg = MOGO_MID; //raise lift
      fourTarg = FOUR_UP;
      Rollers(0);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST/2, DRIVE_MID, DRIVE_LOW_ACCEL); //score goal
      AutonDrive(0.8*tiles, DRIVE_FAST, DRIVE_LOW_ACCEL);
      //AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_SLOW, DRIVE_MID_ACCEL, true); //make sure pointing straight
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2);
      AutonDrive(-2.7*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //push tall goal into red zone
      fourTarg = FOUR_GROUND; //lower lift, spin rollers out
      Rollers(-ROLLER_SPEED);
#ifdef redGoalFirst
      AutonTurn(0, 2.25*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //face red goal
      AutonDrive(12*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to red goal and grab it
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2);
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //turn to face line of rings
      Rollers(ROLLER_SPEED); //roll intake
      AutonDrive(-2*tiles, DRIVE_MID, DRIVE_MID_ACCEL); //drive over line of rings
      mogoTarg = MOGO_MID; //begin setting goal down
      AutonTurn(0, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //turn, putting red goal on line of zone
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2); //set goal down
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true); //face neutral goal
      AutonDrive(2*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //go to neutral goal
      AutonMogo(MOGO_MID, Msec_TO_sec/2); //grab goal
      fourTarg = FOUR_UP; //lift up
      AutonTurn(2*pi/3, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL); //face platform
      AutonDrive(1.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //go to platform
      AutonMogo(MOGO_GROUND, Msec_TO_sec/2); //deposit goal
      AutonDrive(-1*tiles, DRIVE_FAST, DRIVE_LOW_ACCEL); //back up
#else
      AutonTurn(pi, WHEEL_TO_WHEEL_DIST/2, DRIVE_MID, DRIVE_LOW_ACCEL); // turn and back up towards wall
      Rollers(-ROLLER_SPEED);
      AutonDrive(-2*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(pi/2, 18*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //face neutral goal
      AutonDrive(8*inches, DRIVE_FAST, DRIVE_LOW_ACCEL); //pick up neutral goal
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      fourTarg = FOUR_UP; //lift up
      AutonTurn(2*pi/3, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL); //face platform
      AutonDrive(2.2*tiles, DRIVE_FAST, DRIVE_LOW_ACCEL); //drive to platform
      mogoTarg = MOGO_GROUND; //deposit goal
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST, DRIVE_MID, DRIVE_MID_ACCEL, true);
      AutonDrive(-12*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //back up
      fourTarg = FOUR_GROUND;
      AutonTurn(0, WHEEL_TO_WHEEL_DIST + 2*inches, DRIVE_FAST, DRIVE_MID_ACCEL); //face red goal
      wait(100, msec);
      AutonDrive(1.7*tiles, DRIVE_FAST, DRIVE_MID_ACCEL); //drive to red goal
      AutonMogo(MOGO_HOLD, Msec_TO_sec/2); //grab goal
      AutonTurn(pi/2, WHEEL_TO_WHEEL_DIST + 1*inches, DRIVE_MID, DRIVE_MID_ACCEL, true); //turn to face line of rings
      Rollers(ROLLER_SPEED); //roll intake
      AutonDrive(-3*tiles, DRIVE_MID, DRIVE_HIGH_ACCEL); //drive over line of rings
      AutonMogo(MOGO_GROUND, Msec_TO_sec);
      AutonDrive(-0.5*tiles, DRIVE_FAST, DRIVE_MID_ACCEL);
#endif
      break;
    case DebugAuton:
      //MoGo.setPosition(-150, degrees);
      //mogoTarg = MOGO_MID;
      //AutonDrive(2*tiles - 7*inches, DRIVE_FAST, DRIVE_HIGH_ACCEL, false);  //drive to low neutral goal
      AutonMogo(MOGO_MID, Msec_TO_sec/2);
      DriveLB.setPosition(0, turns);
      DriveRB.setPosition(0, turns);
      while((DriveLB.velocity(rpm) + DriveRB.velocity(rpm))/2 <= 50 * MedWheel_InPerSec_TO_RPM / ENC_TO_WHEEL)
      {
        DriveLB.spin(forward, 100, pct);
        DriveRB.spin(forward, 100, pct);
        DriveLF.spin(forward, 100, pct);
        DriveRF.spin(forward, 100, pct);
        MogoPID();

        Controller1.Screen.clearScreen();
        Controller1.Screen.setCursor(1, 1);
        Controller1.Screen.print(DriveLB.position(turns) * ENC_TO_WHEEL * MedWheel_Circumfrence);
      }
      Drive(0, 0);

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
#ifndef arcadeDrive
      //inverted drive, swap joysticks and invert them
      //This will now drive as though forward is backward
      lpow = -RightAxis * AXIS_TO_RPM;
      rpow = -LeftAxis * AXIS_TO_RPM;
#else
      //Arcade drive. One stick controls forward/backward, other controls steering
      //better at going straight than tank drive, clunkier turning (for me).
      //most useful when going for rings.
      lpow = -LStickUpDown * AXIS_TO_RPM + RStickLeftRight * AXIS_TO_RPM;
      rpow = -LStickUpDown * AXIS_TO_RPM - RStickLeftRight * AXIS_TO_RPM;
#endif
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
void AutonDrive(int dist, int maxVel, int accel, bool doDecelerate)
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
      pos += vel / TicksPerSecond; 
      
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

      MogoPID(); //hold mogo and fourbar at their PID targets
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

      pos += vel / TicksPerSecond;

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

      MogoPID();
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

      //recall arc length = r??
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
  return (CurrentGyro + GyroRollOvers * DegreesPerTurn) - gyroOffset;

}

//Simple Mobile Goal (MoGo) P loop.
void MogoPID()
{
  //If the limit switch is pressed, the mogo is at MOGO_HOLD position. 
  //MOGO_HOLD is defined as up against the limit switch 
  if(LimitMogo.pressing()) MoGo.setPosition(MOGO_HOLD, degrees);
  
  Mogo(mogoTarg - MoGo.position(degrees)); //simple P loop

  if(mogoTarg == MOGO_GROUND) steeringKD = STEERING_KD_LIGHT;
  else steeringKD = STEERING_KD_HEAVY;
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
      MogoPID();
      FourbarPID();
  }
  return 0.5*((DriveLB.position(turns) - lstart) + (DriveRB.position(turns) - rstart)) * ENC_TO_WHEEL * MedWheel_Circumfrence;
}