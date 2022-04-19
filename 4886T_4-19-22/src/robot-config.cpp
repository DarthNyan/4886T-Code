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
signature Vision__SIG_1 = signature (1, -3947, -2419, -3183, 6019, 14481, 10250, 1.5, 0);
signature Vision__SIG_2 = signature (2, 8285, 13297, 10791, -1899, -1375, -1637, 1.9, 0);
signature Vision__SIG_3 = signature (3, 467, 3065, 1766, -4123, -3651, -3886, 3, 0);
vision Vision = vision (PORT1, 50, Vision__SIG_1, Vision__SIG_2, Vision__SIG_3);
/*vex-vision-config:end*/
/*vex-vision-config:begin*/
signature FrontVision__SIG_1 = signature (1, -2907, -1691, -2298, 5189, 8539, 6864, 3, 0);
signature FrontVision__SIG_2 = signature (2, 5859, 10601, 8230, -1285, -559, -922, 3, 0);
signature FrontVision__SIG_3 = signature (3, -1, 1203, 600, -4283, -3161, -3722, 3, 0);
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