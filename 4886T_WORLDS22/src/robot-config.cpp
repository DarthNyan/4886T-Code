#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor DriveRF = motor(PORT11, ratio6_1, false);
motor DriveLF = motor(PORT14, ratio6_1, true);
controller Controller1 = controller(primary);
motor RollerIntake = motor(PORT9, ratio6_1, true);
motor DriveLB = motor(PORT15, ratio6_1, true);
motor DriveRB = motor(PORT12, ratio6_1, false);
inertial Gyro = inertial(PORT3);
motor FourBar = motor(PORT10, ratio36_1, false);
motor DriveLM = motor(PORT17, ratio6_1, true);
motor DriveRM = motor(PORT16, ratio6_1, false);
digital_out RearMogo = digital_out(Brain.ThreeWirePort.H);
digital_out FrontMogo = digital_out(Brain.ThreeWirePort.G);
/*vex-vision-config:begin*/
signature Vision__SIG_1 = signature (1, -2197, -1377, -1788, 8951, 12403, 10676, 4.3, 0);
signature Vision__SIG_2 = signature (2, 7145, 10361, 8754, -1099, -641, -870, 1.9, 0);
signature Vision__SIG_3 = signature (3, 1793, 3295, 2544, -3607, -3201, -3404, 3, 0);
vision Vision = vision (PORT1, 50, Vision__SIG_1, Vision__SIG_2, Vision__SIG_3);
/*vex-vision-config:end*/
/*vex-vision-config:begin*/
signature FrontVision__SIG_1 = signature (1, -2215, -1337, -1776, 7943, 11491, 9716, 3, 0);
signature FrontVision__SIG_2 = signature (2, 5859, 10601, 8230, -1285, -559, -922, 3, 0);
signature FrontVision__SIG_3 = signature (3, -47, 1721, 838, -3857, -2553, -3206, 3, 0);
signature FrontVision__SIG_4 = signature (4, 0, 0, 0, 0, 0, 0, 3, 0);
signature FrontVision__SIG_5 = signature (5, 0, 0, 0, 0, 0, 0, 3, 0);
signature FrontVision__SIG_6 = signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
signature FrontVision__SIG_7 = signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision FrontVision = vision (PORT8, 50, FrontVision__SIG_1, FrontVision__SIG_2, FrontVision__SIG_3, FrontVision__SIG_4, FrontVision__SIG_5, FrontVision__SIG_6, FrontVision__SIG_7);
/*vex-vision-config:end*/
digital_out Slapper = digital_out(Brain.ThreeWirePort.F);
digital_out Hook = digital_out(Brain.ThreeWirePort.E);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}