//Gear Ratios

#define LowGearJoystickRatio 1
#define MidGearJoystickRatio 2
#define HighGearJoystickRatio 6
#define LowGearMaxVel 100
#define MidGearMaxVel 200
#define HighGearMaxVel 600

#define _3_TO_1_GearRatio 3
#define _5_TO_1_GearRatio 5
#define _7_TO_1_GearRatio 7
#define _1_TO_3_GearRatio (1/3)
#define _1_TO_5_GearRatio (1/5)
#define _1_TO_7_GearRatio (1/7)

//Wheels

#define pi 3.141592653
#define LargeOmni_Diameter 4
#define MedOmni_Diameter 3.25
#define SmallOmni_Diameter 2.75
#define LargeWheel_Diameter 5
#define MedWheel_Diameter 4
#define Traction_WheelDiameter 3.25
#define SmallWheel_Diameter 2.75

#define LargeOmni_Circumfrence 13.1
#define MedOmni_Circumfrence 10.2
#define SmallOmni_Circumfrence 8.6
#define LargeWheel_Circumfrence 15.6
#define MedWheel_Circumfrence 13.2
#define TractionWheel_Circumfrence 10.2
#define SmallWheel_Circumfrence 8.6

#define LargeOmni_InPerSec_TO_RPM (60/13.1)
#define MedOmni_InPerSec_TO_RPM (60/10.2)
#define SmallOmni_InPerSec_TO_RPM (60/8.6)
#define LargeWheel_InPerSec_TO_RPM (60/15.6)
#define MedWheel_InPerSec_TO_RPM (60/13.2)
#define TractionWheel_InPerSec_TO_RPM (60/10.2)
#define SmallWheel_InPerSec_TO_RPM (60/8.6)

//UnitConversion

#define DegreesPerTurn 360
#define DegreesPerHalfTurn 180
#define DegreesPerQuarterTurn 90

#define in_TO_cm 2.54
#define m_TO_in 39.37
#define rad_TO_deg 57.296

//Ticks
//Use these to make a while loop repeat once per tick, where ticks are a certain fraction of a second

#define TenthSecondTick 100
#define TwentiethSecondTick 50
#define FiftiethSecondTick 20 
#define Msec_TO_sec 1000
#define VexTimer vex::timer::system()

#define GyroCalibrateTime (2*Msec_TO_sec)

//Controller Buttons
//For use with a controller named "Controller1"

#define LStickUpDown Controller1.Axis3.position(percent)
#define RStickUpDown Controller1.Axis2.position(percent)
#define LStickLeftRight Controller1.Axis4.position(percent)
#define RStickLeftRight Controller1.Axis1.position(percent)

#define XButton Controller1.ButtonX.pressing()
#define YButton Controller1.ButtonY.pressing()
#define BButton Controller1.ButtonB.pressing()
#define AButton Controller1.ButtonA.pressing()

#define UpButton Controller1.ButtonUp.pressing()
#define DownButton Controller1.ButtonDown.pressing()
#define LeftButton Controller1.ButtonLeft.pressing()
#define RightButton Controller1.ButtonRight.pressing()

#define L1Button Controller1.ButtonL1.pressing()
#define R1Button Controller1.ButtonR1.pressing()
#define L2Button Controller1.ButtonL2.pressing()
#define R2Button Controller1.ButtonR2.pressing()

//Brain

#define BrainScreenWidth 480
#define BrainScreenHeight 272

//Vision sensor

#define VisionScreenHeight 212
#define VisionScreenWidth 316