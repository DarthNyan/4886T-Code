
#define RightAxis Controller1.Axis2.position(percent)
#define LeftAxis Controller1.Axis3.position(percent)
#define AXIS_TO_RPM 6
#define BUTTON_TO_TORQUE 100
#define BUTTON_TO_SPEED 200
#define BUTTON_TO_TURBO 600
#define TORQUE_ROTATION_PER_TICK 10
#define SPEED_ROTATION_PER_TICK 20
#define DEADBAND 50
#define ROLLER_SPEED 500
#define ENC_TO_WHEEL ((float)3/7)


#define Shift (-(float)(Controller1.ButtonR2.pressing() - 0.5) * 2)
#define MoGoButton (Controller1.ButtonR1.pressing())
#define ChainBarButton (Controller1.ButtonX.pressing())
#define FourBarButton (Controller1.ButtonL1.pressing())
#define RollerButton (Controller1.ButtonL2.pressing())

#define MOGO_JUNK -500
#define MOGO_GROUND 0
#define MOGO_HOLD 750
#define MOGO_MID 450
#define FOUR_UP 380
#define FOUR_LOW 280
#define FOUR_GROUND 0


#define AWPlineAuton 0
#define PlatformAuton 1
#define NeutralAuton 2
#define FullAWP 3
#define AWP_Platform 4
#define DebugAuton 5
#define SkillsAuton 6
#define LeastAuton AWPlineAuton
#define GreatestAuton SkillsAuton

#define WHEEL_TO_WHEEL_DIST 12
#define TicksPerSecond (Msec_TO_sec / tickSpeed)
#define DRIVE_KP 30
#define STEERING_KP 3
#define PARK_KP 20
#define PARK_KD 500
#define tiles 24
#define inches 1

#define VISION_CENTER 150


#define DRIVE_FAST 50
#define DRIVE_MID 25
#define DRIVE_SLOW 10
#define DRIVE_MID_ACCEL 50
#define DRIVE_LOW_ACCEL 20
#define DRIVE_HIGH_ACCEL 100

#define GyroMarginOfRollOver 300

void Drive(int lspd, int rspd);
void Mogo(int spd);
void MogoPID();
void FourbarPID();
void Rollers(int spd);
void Fourbar(int spd);
void AutonDrive(int dist, int vel, int accel);
void AutonTurn(float radians, int outerRadius, int vel, int accel, bool reversed = false);
void AutonMogo(int pos, int duration);
int GyroAbsPos();
void GraphDebuggerVars(int Red, int Blue = 0, int Green = 0, int Purple = 0, int Orange = 0, int Yellow = 0);
float limitFloatToMax(float variable, float lim);
void TestRefreshRate();
void StopDrive();
void Park();

//#define arcadeDrive