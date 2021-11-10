
#define RightAxis Controller1.Axis2.position(percent)
#define LeftAxis Controller1.Axis3.position(percent)
#define AXIS_TO_RPM 2
#define BUTTON_TO_TORQUE 100
#define BUTTON_TO_SPEED 200
#define BUTTON_TO_TURBO 600
#define TORQUE_ROTATION_PER_TICK 10
#define SPEED_ROTATION_PER_TICK 20
#define DEADBAND 15
#define ROLLER_SPEED 500


#define Shift (-(float)(Controller1.ButtonR2.pressing() - 0.5) * 2)
#define MoGoButton (Controller1.ButtonR1.pressing())
#define ChainBarButton (Controller1.ButtonX.pressing())
#define FourBarButton (Controller1.ButtonL1.pressing())
#define RollerButton (Controller1.ButtonL2.pressing())


#define Ground 0
#define AlliancePost 1
#define NeutralPost 2
#define HighPost 3
#define ChainBarPos 0
#define FourBarPos 1
#define MOGO_GROUND 0
#define MOGO_HOLD 570
#define MOGO_MID 300
#define FOUR_UP 360
#define FOUR_LOW 280
#define FOUR_GROUND 0


#define LeftAuton (-1)
#define RightAuton 1
#define AWPlineAuton 0
#define PlatformAuton 1
#define NeutralAuton 2
#define FullAWP 3
#define LeastAuton AWPlineAuton
#define GreatestAuton FullAWP

#define WheelRadius 16
#define TicksPerSecond (Msec_TO_sec / tickSpeed)
#define DRIVE_KP 30
#define STEERING_KP 3

#define DRIVE_FAST 40
#define DRIVE_MID 20
#define DRIVE_SLOW 10
#define DRIVE_MID_ACCEL 40
#define DRIVE_LOW_ACCEL 20
#define DRIVE_HIGH_ACCEL 80

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