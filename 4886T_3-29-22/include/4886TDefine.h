
#define RightAxis Controller1.Axis2.position(percent)
#define LeftAxis Controller1.Axis3.position(percent)
#define AXIS_TO_RPM 6
#define BUTTON_TO_TORQUE 100
#define BUTTON_TO_SPEED 200
#define BUTTON_TO_TURBO 600
#define TORQUE_ROTATION_PER_TICK 10
#define SPEED_ROTATION_PER_TICK 20
#define DEADBAND 50
#define ROLLER_SPEED 600
#define ENC_TO_WHEEL ((float)3/7)


#define RearMogoButton (Controller1.ButtonDown.pressing())
#define FrontMogoButton (Controller1.ButtonLeft.pressing())
#define FourBarUpButton (Controller1.ButtonR1.pressing())
#define FourBarDownButton (Controller1.ButtonR2.pressing())
#define RollerInButton (Controller1.ButtonL2.pressing())
#define RollerOutButton (Controller1.ButtonL1.pressing())

#define FOUR_UP 600
#define FOUR_LOW 400
#define FOUR_HOVER 70
#define FOUR_GROUND 0


#define AWPlineAuton 0
#define PlatformAuton 1
#define NeutralAuton 2
#define FullAWP 3
#define CenterRingsAuton 4
#define DebugAuton 5
#define SkillsAuton 6
#define LeastAuton AWPlineAuton
#define GreatestAuton SkillsAuton

#define WHEEL_TO_WHEEL_DIST 12
#define TicksPerSecond (Msec_TO_sec / tickSpeed)
#define DRIVE_KP 30
#define STEERING_KP 3
#define DRIVE_LAG_TOLERANCE 3
#define STEERING_KD_LIGHT (0)
#define STEERING_KD_HEAVY (-5)
#define D_UPDATE_TIME 3
#define PARK_KP 20
#define PARK_KD 500
#define tiles 24
#define inches 1
#define VISION_CENTER 150


#define DRIVE_FAST 50
#define DRIVE_MID 25
#define DRIVE_SLOW 10
#define DRIVE_MID_ACCEL 50
#define DRIVE_LOW_ACCEL 30
#define DRIVE_HIGH_ACCEL 80
#define DRIVE_RACE_ACCEL 150

#define GyroMarginOfRollOver 300

extern int AutonSelect;
extern int AutonSide;
extern int gyroOffset;

#define BLUE_MOGO Vision__SIG_1
#define RED_MOGO Vision__SIG_2
#define YELLOW_MOGO Vision__SIG_3
#define VISION_GOAL_PRIMED 161
#define BLUE_SIDE 1
#define NEUTRAL_SIDE 0
#define RED_SIDE -1

void Drive(int lspd, int rspd);
void FourbarPID();
void Rollers(int spd);
void Fourbar(int spd);
void AutonDrive(int dist, int vel, int accel, bool doDecelerate = true, bool doLagBehind = false);
void AutonTurn(float radians, int outerRadius, int vel, int accel, bool reversed = false);
int GyroAbsPos();
void GraphDebuggerVars(int Red, int Blue = 0, int Green = 0, int Purple = 0, int Orange = 0, int Yellow = 0);
float limitFloatToMax(float variable, float lim);
void TestRefreshRate();
void StopDrive();
void Park();
float avg(int entries[], int len);
float StopAndRetDist();
void OnRearMogo();
void OffRearMogo();
void ToggleRearMogo();
void OnFrontMogo();
void OffFrontMogo();
void ToggleFrontMogo();
void RaceForward(int dist);
void FindMogo(int side);
void SeeMogo(int side);
void AutonGui();
void drawMogo(int xcent, int ycent, int side);
void drawPlatform(int side);
void drawHomeFieldLines();
void drawRingle(int xcent, int ycent);
void drawRingleStar (int xcent, int ycent);
void drawRingles(void);
//#define arcadeDrive


