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
signature Vision__SIG_2 = signature (2, 7661, 13215, 10438, -1275, -765, -1020, 1.9, 0);
signature Vision__SIG_3 = signature (3, 905, 3667, 2286, -4395, -4085, -4240, 3, 0);
vision Vision = vision (PORT1, 50, Vision__SIG_1, Vision__SIG_2, Vision__SIG_3);
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