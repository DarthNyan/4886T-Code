#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor DriveRF = motor(PORT12, ratio6_1, false);
motor DriveLF = motor(PORT19, ratio6_1, true);
motor MoGo = motor(PORT14, ratio36_1, false);
controller Controller1 = controller(primary);
motor RollerIntake = motor(PORT9, ratio6_1, true);
motor DriveLB = motor(PORT20, ratio6_1, true);
motor DriveRB = motor(PORT11, ratio6_1, false);
motor FourBar = motor(PORT10, ratio36_1, false);
inertial Gyro = inertial(PORT3);
limit LimitMogo = limit(Brain.ThreeWirePort.H);
motor FourBarB = motor(PORT1, ratio36_1, true);

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