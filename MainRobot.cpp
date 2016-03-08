#include "WPILib.h"
#include "breakerbots.h"

#define DRIVE_ENABLED
//#define ELBOW_ENABLED
#define SHOOTER_ENABLED

class Robot: public IterativeRobot
{
private:
	Joystick xbox; // only joystick
	PrintStream ps;
	Timer _timer;

#ifdef DRIVE_ENABLED
	DriveCalibrator driveCalibrator;
	CANTalon frontLeft , backLeft , frontRight, backRight;
	RobotDrive bruinDrive; // robot drive system
#endif

#ifdef ELBOW_ENABLED
	CANTalon elbow;
	AnalogInput ballDetector;
#endif

#ifdef SHOOTER_ENABLED
	AnalogInput proxSensor;
	Servo servoIndexer;
	Servo servoKicker;
	Servo servoTilter;

	CANTalon spinLeft;
	CANTalon spinRight;

	Indexer ballIntakeCalibrator;
#endif

public:
	Robot()
		:
		xbox(0),
		ps(),
		_timer()


#ifdef DRIVE_ENABLED
		,driveCalibrator(xbox),
			frontLeft(SRXMotors::frontLeft),
			backLeft(SRXMotors::backLeft),
			frontRight(SRXMotors::frontRight),
			backRight(SRXMotors::backRight),
		bruinDrive(frontLeft, backLeft, frontRight, backRight)
#endif

#ifdef ELBOW_ENABLED
		,elbow(SRXMotors::roller)
#endif

#ifdef SHOOTER_ENABLED
		,proxSensor(3),
		servoIndexer(RobotPorts::indexer),
		servoKicker(RobotPorts::kicker),
		servoTilter(RobotPorts::tilter),

		spinLeft(SRXMotors::spinnerLeft),
		spinRight(SRXMotors::spinnerRight),

		ballIntakeCalibrator(xbox,proxSensor,
			servoIndexer,servoKicker, servoTilter,
			spinLeft,spinRight,ps)
#endif

	{
#ifdef DRIVE_ENABLED
	bruinDrive.SetSafetyEnabled(false);
	driveCalibrator.setPrecisionEffort(0.5);
#endif

#ifdef ELBOW_ENABLED
	elbow.SetSafetyEnabled(false);
#endif

#ifdef SHOOTER_ENABLED
	spinLeft.SetSafetyEnabled(false);
	spinRight.SetSafetyEnabled(false);

	ballIntakeCalibrator.setTimerDelay(1.5);
	ballIntakeCalibrator.setSpinIN(.2);
	ballIntakeCalibrator.setSpinOUT(-.8);
#endif

	}
private:

	void RobotInit()
	{


	}

	void AutonomousInit()
	{
		_timer.Reset();
		_timer.Start();

#ifdef SHOOTER_ENABLED
		ballIntakeCalibrator.update(true);
#endif



	}

	void AutonomousPeriodic()
	{

#ifdef DRIVE_ENABLED
		if (_timer.Get() < 2){
			bruinDrive.ArcadeDrive(0.4,0,false);
		} else {
			bruinDrive.TankDrive(0.0,0.0,false);
		}
#endif


	}

	void TeleopInit()
	{
		_timer.Stop();
		ps.resetStream();
	}

	void TeleopPeriodic()
	{
		ps.clearStream();

#ifdef DRIVE_ENABLED
		Utilities::drive(driveCalibrator,bruinDrive);
		ps.print(driveCalibrator.modeToString(driveCalibrator.currentMode()));

		ps.print(Utilities::concat("RawX: ",xbox.GetRawAxis(XBox::leftX)));
		ps.print(Utilities::concat("RawY: ",xbox.GetRawAxis(XBox::leftY)));

		ps.print(Utilities::concat("NewX: ",driveCalibrator.X()));
		ps.print(Utilities::concat("NewY: ",driveCalibrator.Y()));

		ps.print(Utilities::concat("TurboX: ",driveCalibrator.turboEngagedX()));
		ps.print(Utilities::concat("TurboY: ",driveCalibrator.turboEngagedY()));
#endif
#ifdef ELBOW_ENABLED
		if (xbox.GetRawButton(XBox::b)){
			elbow.Set(.5);
		} else if (xbox.GetRawButton(XBox::a)){
			elbow.Set(-.5);
		} else {
			elbow.Set(0);
		}
#endif
#ifdef SHOOTER_ENABLED
		ballIntakeCalibrator.setTilterDOWN(SmartDashboard::GetNumber("DB/Slider 2",0));
		ballIntakeCalibrator.setTilterLEVEL(SmartDashboard::GetNumber("DB/Slider 3",0));
		ballIntakeCalibrator.update(true);//bool = debug
#endif

	}

	void TestPeriodic()
	{


	}
};

START_ROBOT_CLASS(Robot)
