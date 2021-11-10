/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// DriveRF              motor         10              
// DriveLF              motor         1               
// MoGo                 motor         18              
// Controller1          controller                    
// RollerIntake         motor         7               
// DriveLB              motor         11              
// DriveRB              motor         20              
// FourBar              motor         19              
// Gyro                 inertial      3               
// LimitMogo            limit         H               
// FourBarB             motor         12              
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

int AutonSide = RightAuton;
int AutonSelect = AWPlineAuton;
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Gyro.calibrate();
  while(true)
  {
    Brain.Screen.clearScreen();
    Brain.Screen.drawLine(150, 0, 150, 200);
    Brain.Screen.drawLine(300, 0, 300, 200);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Side: ");
    switch(AutonSide)
    {
      case RightAuton:
        Brain.Screen.print("Right");
        break;
      case LeftAuton:
        Brain.Screen.print("Left");
        break;
      default:
        Brain.Screen.print("Error");
    }
    Brain.Screen.setCursor(1, 17);
    Brain.Screen.print("Auton: ");
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
      default:
        Brain.Screen.print("Error");
    }
    Brain.Screen.setCursor(1, 40);
    Brain.Screen.print("Enter");

    if(Brain.Screen.pressing())
    {
      if(Brain.Screen.xPosition() < 150)
      {
        AutonSide *= -1;
        while(Brain.Screen.pressing()){}
      }
      else if(Brain.Screen.xPosition() < 300)
      {
        if(AutonSelect  >= GreatestAuton) AutonSelect = LeastAuton;
        else AutonSelect ++;

        while(Brain.Screen.pressing()){}
      }
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
  gyroHeading = 0; 
  FourBar.setPosition(0, degrees);
  MoGo.setPosition(0, degrees);
  mogoTarg = 0;
  fourTarg = 0;
  switch(AutonSelect)
  {
    case AWPlineAuton:
      
      AutonDrive(12, DRIVE_FAST, DRIVE_MID_ACCEL);  //drive to mogo
      MoGo.setPosition(-100, degrees); 
      mogoTarg = MOGO_HOLD;  //pick up mogo & drive back 
      AutonTurn(-pi, 13, DRIVE_FAST, DRIVE_MID_ACCEL);  //turn and bring fourbar up
      Rollers(ROLLER_SPEED);  //roll intake and grab line of rings
      AutonDrive(-30, DRIVE_SLOW, DRIVE_LOW_ACCEL); 
      AutonDrive(40, DRIVE_FAST, DRIVE_MID_ACCEL);  //drive back into home zone
      AutonMogo(MOGO_GROUND, Msec_TO_sec / 2); //set down and back up
      AutonDrive(-12, DRIVE_FAST, DRIVE_MID_ACCEL);
      break;
    case NeutralAuton:
      AutonDrive(48, DRIVE_FAST, DRIVE_HIGH_ACCEL);  //drive to low neutral goal
      MoGo.setPosition(-100, degrees);
      AutonMogo(MOGO_HOLD, Msec_TO_sec / 3);  //pick up neutral goal & drive back
      AutonDrive(-24, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonTurn(-pi/2, 8, DRIVE_FAST, DRIVE_MID_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec); //place mogo down
      AutonTurn(pi/2, 8, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face alliance goal.
      AutonDrive(8, DRIVE_FAST, DRIVE_MID_ACCEL); //go to alliance goal and pick up
      AutonMogo(MOGO_HOLD, Msec_TO_sec);
      Rollers(ROLLER_SPEED); //roll intake to score preloads
      AutonDrive(-24, DRIVE_MID, DRIVE_LOW_ACCEL);
      AutonMogo(MOGO_GROUND, Msec_TO_sec / 2); //set down and back up
      AutonDrive(-12, DRIVE_FAST, DRIVE_MID_ACCEL);
      Rollers(0);
      break;
    case PlatformAuton:
    case FullAWP:
      MoGo.setPosition(-100, degrees);
      AutonMogo(MOGO_HOLD, Msec_TO_sec * 1.5); //raise mogo, get absolute position, score 1 ring in base
      mogoTarg = MOGO_GROUND; //set mogo back down
      AutonDrive(-24, DRIVE_FAST, DRIVE_LOW_ACCEL); //bump against wall
      if(AutonSelect == FullAWP)
      {
        AutonTurn(-pi/2, 20, DRIVE_FAST, DRIVE_MID_ACCEL);
        AutonTurn(0, 16, DRIVE_FAST, DRIVE_MID_ACCEL); //turn to face other mogo
        AutonDrive(114, DRIVE_FAST, DRIVE_MID_ACCEL); //go to other mogo & pick it up
        AutonMogo(MOGO_HOLD, Msec_TO_sec);
        AutonTurn(0, 8, DRIVE_FAST, DRIVE_MID_ACCEL, true); //correct steering incase collision with mogo knocked it off 
        Rollers(ROLLER_SPEED); //roll intake to score preloads
        AutonDrive(-24, DRIVE_MID, DRIVE_LOW_ACCEL);
        AutonMogo(MOGO_GROUND, Msec_TO_sec / 2); //set down and back up
        AutonDrive(-12, DRIVE_FAST, DRIVE_MID_ACCEL);
        Rollers(0);
        break;
      }
      AutonTurn(-pi/2, 32, DRIVE_FAST, DRIVE_MID_ACCEL);
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
      buttonToggle = false;
    }

    if (driveReversed)
    {
      //Drive(-RightAxis * AXIS_TO_RPM, -LeftAxis * AXIS_TO_RPM);  //inverted drive
      lpow = -RightAxis * AXIS_TO_RPM;
      rpow = -LeftAxis * AXIS_TO_RPM;
    }
    else
    {
      //Drive(LeftAxis * AXIS_TO_RPM, RightAxis * AXIS_TO_RPM); //basic drive command
      lpow = LeftAxis * AXIS_TO_RPM;
      rpow = RightAxis * AXIS_TO_RPM;
    }

    if(lpow < DEADBAND && lpow > -DEADBAND) lpow = 0;  //deadband
    if(rpow < DEADBAND && rpow > -DEADBAND) rpow = 0;
    Drive(lpow, rpow);

    
    fourTarg += (FourBarButton * TORQUE_ROTATION_PER_TICK * Shift); //fourbar PID
    if(XButton)
    {
      fourTarg = FOUR_UP;
    }
    else if(YButton)
    {
      fourTarg = FOUR_LOW;
    }
    else if(BButton)
    {
      fourTarg = FOUR_GROUND;
    }
    FourbarPID();

    mogoTarg += MoGoButton * TORQUE_ROTATION_PER_TICK * Shift; //mogo PID
    if(DownButton) 
    {
      mogoTarg = MOGO_GROUND;
    }
    else if(LeftButton) 
    {
      MoGo.setPosition(-100, degrees);
      mogoTarg = MOGO_HOLD;
    }
    else if(RightButton) mogoTarg = MOGO_MID;
    MogoPID();
    
    Rollers(RollerButton * ROLLER_SPEED * Shift); //basic rollers

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


void Drive(int lspd, int rspd)
{
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
  FourBar.spin(forward, spd - (FourBar.position(degrees) - FourBarB.position(degrees)), rpm);
  FourBarB.spin(forward, spd + (FourBar.position(degrees) - FourBarB.position(degrees)), rpm);
}
float StopDistance(float vel, float accel, float targetVel = 0)
{
  return -(targetVel*targetVel - vel*vel)/(2*accel); 
  //derived from physics equation V^2 = (V0)^2 +  2a(X-X0)
  //set V to targetVel and solve for X-X0 
}
void AutonDrive(int dist, int maxVel, int accel)
{
  float lEnc, rEnc;
  float lStart = DriveLB.position(turns), rStart = DriveRB.position(turns);
  float vel = 0, pos = 0;
  bool decel = false;
  int tickSpeed = FiftiethSecondTick, currentTick = VexTimer, gyroUpdate;
  if(dist > 0)
  {
    while(vel >= 0) //driving forward
    {
      gyroUpdate = GyroAbsPos(); //call GyroAbsPos to ensure gyro val is accurate in future

      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence; //get dist encoders have traveled in inches
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence;

      if(dist - pos <= StopDistance(vel, accel) || decel == true) //if we've at the distance to start stopping or started our deceleration routine
      {
        vel -= (float)accel / TicksPerSecond; //decrement velocity and indicate that we've started our deceleration routine
        decel = true;
      }
      else if (vel < maxVel) //otherwise, if we're not at our maximum velocity
      {
        vel += (float)accel / TicksPerSecond; //increment velocity, velocity in in/sec
      }
      else vel = maxVel; //otherwise ensure that we stay at max velocity

      pos += vel / TicksPerSecond; //increment position by velocity

      //drive, converting velocity from in/sec to rpm and adding PID control to stay at the position target
      Drive(DRIVE_KP * (pos - lEnc)
       + vel * MedWheel_InPerSec_TO_RPM 
       - STEERING_KP * (GyroAbsPos() - gyroHeading), 
       DRIVE_KP * (pos - lEnc) 
       + vel * MedWheel_InPerSec_TO_RPM 
       + STEERING_KP * (GyroAbsPos() - gyroHeading));
      
      GraphDebuggerVars(StopDistance(vel, accel), dist - pos, dist, pos, lEnc); //debugging

      MogoPID(); //hold mogo and fourbar
      FourbarPID();

      currentTick += tickSpeed; //wait until a fiftieth of a second has passed since the last iteration through the loop.
      while(VexTimer < currentTick) {} //this ensures constant loop time
    }
  }
  else if(dist < 0) //driving backward. Copy-paste of drive forward, but reversed
  {
    while(vel <= 0)
    {
      gyroUpdate = GyroAbsPos();
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence;
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence;

      if(dist - pos >= -StopDistance(vel, accel) || decel == true)
      {
        vel += (float)accel / TicksPerSecond;
        decel = true;
      }
      else if (vel > -maxVel)
      {
        vel -= (float)accel / TicksPerSecond;
      }
      else vel = -maxVel;

      pos += vel / TicksPerSecond;

      Drive(DRIVE_KP * (pos - lEnc)
       + vel * MedWheel_InPerSec_TO_RPM
       - STEERING_KP * (GyroAbsPos() - gyroHeading),
       DRIVE_KP * (pos - lEnc)
       + vel * MedWheel_InPerSec_TO_RPM 
       + STEERING_KP * (GyroAbsPos() - gyroHeading));
      
      GraphDebuggerVars(StopDistance(vel, accel), -(dist - pos), -dist, -pos, -lEnc);

      MogoPID(); //hold mogo and fourbar
      FourbarPID();

      

      currentTick += tickSpeed;
      while(VexTimer < currentTick) {}
    }
  }
  Drive(0, 0);
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print(MoGo.position(degrees));
}
void AutonTurn(float radians, int outerRadius, int vel, int accel, bool reversed)
{
  float GyroStart = GyroAbsPos() / rad_TO_deg, angleLeft;
  float innerRadius = outerRadius - WheelRadius, radiusRatio = innerRadius / outerRadius;
  float lEnc, rEnc, lStart = DriveLB.position(turns), rStart = DriveRB.position(turns);
  float outerVel = 0, outerPos = 0;
  int tickSpeed = FiftiethSecondTick, currentTick = VexTimer;

  if (radians > GyroStart) //turning right
    while (outerVel >= 0)
    {
      lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence; //get dist encoders have traveled in inches
      rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence;
      angleLeft = radians - (GyroAbsPos()) / rad_TO_deg; //angle left is equal to the target angle - angle traveled
      if(angleLeft * outerRadius <= StopDistance(outerVel, accel)) //if the distance left to travel is <= distance to stop, decelerate
      {
        outerVel -= (float)accel / TicksPerSecond;
      }
      else if (outerVel < vel) //else, if the outervel < max vel, accelerate
      {
        outerVel += (float)accel / TicksPerSecond; 
      }
      else outerVel = vel; //else, ensure outervel stays at vel

      outerPos += outerVel / TicksPerSecond; //increment position target by velocity

      //drive at the inner and outer wheel speeds, using PID to ensure wheels stay with their position target
      if(reversed)
        Drive(-DRIVE_KP * (outerPos * radiusRatio + lEnc) - outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM,
              -DRIVE_KP * (outerPos + rEnc) - outerVel * MedWheel_InPerSec_TO_RPM);
      else
        Drive(DRIVE_KP * (outerPos - lEnc) + outerVel * MedWheel_InPerSec_TO_RPM, 
              DRIVE_KP * (outerPos * radiusRatio - rEnc) + outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM);


      MogoPID(); //hold mogo and fourbar
      FourbarPID();

      currentTick += tickSpeed; //wait until a fiftieth of a second has passed.
      while(VexTimer < currentTick) {}
    }
    else if(radians < GyroStart) //turning left
      while (outerVel >= 0) //same as right turn, but in inverted
      {
        lEnc = (DriveLB.position(turns) - lStart) * MedWheel_Circumfrence; //get dist encoders have traveled in inches
        rEnc = (DriveRB.position(turns) - rStart) * MedWheel_Circumfrence;
        angleLeft =  -(radians - (GyroAbsPos()) / rad_TO_deg); //angle left is equal to the target angle - angle traveled
        if(angleLeft * outerRadius <= StopDistance(outerVel, accel)) //if the distance left to travel is <= distance to stop, decelerate
        {
          outerVel -= (float)accel / TicksPerSecond;
        }
        else if (outerVel < vel) //else, if the outervel < max vel, accelerate
        {
          outerVel += (float)accel / TicksPerSecond; 
        }
        else outerVel = vel; //else, ensure outervel stays at vel

        outerPos += outerVel / TicksPerSecond; //increment position target by velocity

        //drive at the inner and outer wheel speeds, using PID to ensure wheels stay with their position target
        if(reversed)
          Drive(-DRIVE_KP * (outerPos + lEnc) - outerVel * MedWheel_InPerSec_TO_RPM,
                -DRIVE_KP * (outerPos * radiusRatio + rEnc) + outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM);
        else
          Drive(DRIVE_KP * (outerPos * radiusRatio - lEnc) + outerVel * radiusRatio * MedWheel_InPerSec_TO_RPM, 
              DRIVE_KP * (outerPos - rEnc) + outerVel * MedWheel_InPerSec_TO_RPM);

        GraphDebuggerVars(- radians * rad_TO_deg, - angleLeft * rad_TO_deg);

        
        MogoPID(); //hold mogo and fourbar
        FourbarPID();
    
        currentTick += tickSpeed; //wait until a fiftieth of a second has passed.
        while(VexTimer < currentTick) {}
      }
  Drive(0, 0);
  gyroHeading = radians * rad_TO_deg;
}

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
  xcursor ++;
}

int GyroRollOvers = 0, PreviousGyro = 0, CurrentGyro;
int GyroAbsPos()
{
  CurrentGyro = Gyro.orientation(yaw, degrees);
  if(CurrentGyro - PreviousGyro > GyroMarginOfRollOver) GyroRollOvers --;
  else if(CurrentGyro - PreviousGyro < -GyroMarginOfRollOver) GyroRollOvers ++;
  PreviousGyro = CurrentGyro;

  return (CurrentGyro + GyroRollOvers * DegreesPerTurn);

}
void MogoPID()
{
  if(LimitMogo.pressing()) MoGo.setPosition(MOGO_HOLD, degrees);
  
  Mogo(mogoTarg - MoGo.position(degrees)); //mogo PID
}
void FourbarPID()
{
  Fourbar(fourTarg - FourBar.position(degrees)); 
}
void AutonMogo(int pos, int duration)
{
  mogoTarg = pos;
  for(int i = 0; i < duration / FiftiethSecondTick; i ++)
      {
        MogoPID();
        wait(FiftiethSecondTick, msec);
      }
}