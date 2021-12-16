using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor DriveRF;
extern motor DriveLF;
extern motor MoGo;
extern controller Controller1;
extern motor RollerIntake;
extern motor DriveLB;
extern motor DriveRB;
extern motor FourBar;
extern inertial Gyro;
extern limit LimitMogo;
extern motor FourBarB;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );