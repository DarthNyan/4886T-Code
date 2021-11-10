#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor DriveRF = motor(PORT10, ratio18_1, true);
motor DriveLF = motor(PORT1, ratio18_1, false);
motor MoGo = motor(PORT18, ratio36_1, true);
controller Controller1 = controller(primary);
motor RollerIntake = motor(PORT7, ratio6_1, true);
motor DriveLB = motor(PORT11, ratio18_1, false);
motor DriveRB = motor(PORT20, ratio18_1, true);
motor FourBar = motor(PORT19, ratio36_1, true);
inertial Gyro = inertial(PORT3);
limit LimitMogo = limit(Brain.ThreeWirePort.H);
motor FourBarB = motor(PORT12, ratio36_1, false);

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