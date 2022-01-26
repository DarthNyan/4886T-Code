#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor DriveRF = motor(PORT12, ratio6_1, false);
motor DriveLF = motor(PORT19, ratio6_1, true);
motor MoGo = motor(PORT7, ratio36_1, false);
controller Controller1 = controller(primary);
motor RollerIntake = motor(PORT9, ratio6_1, true);
motor DriveLB = motor(PORT20, ratio6_1, true);
motor DriveRB = motor(PORT11, ratio6_1, false);
motor FourBar = motor(PORT10, ratio36_1, false);
inertial Gyro = inertial(PORT3);
limit LimitMogo = limit(Brain.ThreeWirePort.H);
motor FourBarB = motor(PORT1, ratio36_1, true);
/*vex-vision-config:begin*/
signature Vision__SIG_1 = signature (1, 293, 2575, 1434, -4107, -3399, -3752, 2.5, 0);
signature Vision__SIG_2 = signature (2, 8139, 10133, 9136, -1063, 1, -530, 2.5, 0);
signature Vision__SIG_3 = signature (3, -3333, -2499, -2916, 11851, 14595, 13222, 5, 0);
vision Vision = vision (PORT13, 50, Vision__SIG_1, Vision__SIG_2, Vision__SIG_3);
/*vex-vision-config:end*/

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