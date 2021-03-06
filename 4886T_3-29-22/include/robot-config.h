using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor DriveRF;
extern motor DriveLF;
extern controller Controller1;
extern motor RollerIntake;
extern motor DriveLB;
extern motor DriveRB;
extern inertial Gyro;
extern motor FourBar;
extern motor DriveLM;
extern motor DriveRM;
extern digital_out RearMogo;
extern digital_out FrontMogo;
extern signature Vision__SIG_1;
extern signature Vision__SIG_2;
extern signature Vision__SIG_3;
extern signature Vision__SIG_4;
extern signature Vision__SIG_5;
extern signature Vision__SIG_6;
extern signature Vision__SIG_7;
extern vision Vision;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );