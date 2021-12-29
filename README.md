# 4886T ROBOT CODE
#### Video Demo:
#### Description:

This is the latest version of the code for my Vex VRC robot, 4886T. The motor configurations and constants used are specific to my robot, but they could be updated to fit any robot of a similar layout. The program was written on Vexcode V5 Pro, which functions as an IDE and compiles code for the robot brain. My robot currently consists of four drive motors (which power the wheels), two on each side, two motors powering a four-bar lift, one motor powering a "mobile goal" (mogo) grabber, and one motor powering a "ring" intake.

Inside the project folder is the 4886T_12-16-21 folder and the V5StandardDefines header file. This header file contains useful constants, such as conversion factors, and shortened names for Vex-made variables to improve readability. It is designed to not be specific to any robot, so it lives outside of the 4886T_12-16-21 folder so it can be accessed by any robot code.

Inside the 4886T_12-16-21 are a number of files made by Vex to help compile the code for the robot brain. The two files of interest here are include/4886TDefine.h and src/main.cpp. 4886TDefine.h contains all the constants and function prototypes that are specific to this robot. 

main.cpp doesn't begin with void main(), as main() is not interesting. All main() does is call pre_auton() and then set up the autonomous() and usercontrol() functions before waiting in a while(1) loop to avoid exiting. In a match, the robot gets plugged into a switch that can turn on autonomous or usercontrol, so this sets the robot up to run each when the switch allows it. All the interesting code is in pre_auton(), autonomous(), and usercontrol().

pre_auton() configures the gyro sensor and selects the autonomous routine to run when autonomous() is called. It uses a switch(autonSelect) to choose which message to display, and then cycles through the autonomous routines each time the left side of the brain's touchscreen is pressed. Pressing the right side of the screen exits pre_auton(), locking autonSelect.

autonomous() starts by clearing encoders, and then enters a switch(autonSelect), which mirrors the switch used in pre_auton(). Each case is an autonomous routine. I spent a lot of time on AutonDrive and AutonTurn functions because they allow me to abstract away moving around the field. They take arguments in inches, in/sec, in/sec^2, and, for AutonTurn, radians. I chose these units because Vex's field specifications are generally given in inches and radians tend to be more useful than degrees.

Lastly, usercontrol() allows my handheld controller, which is similar to that of a PlayStation, to give feedback to the robot. I use the two joysticks to control the drive motors and the buttons to set PID targets for other motors. I also use a "Shift key" which allows me to have Shift+Button do something  different from Button, which allows me to run more different commands with only twelve buttons.
