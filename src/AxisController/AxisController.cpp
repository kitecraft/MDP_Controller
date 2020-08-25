#include "AxisController.h"

AxisController::AxisController()
{

}

void AxisController::Init(int inMotorPin1, int inMotorPin2, int inMotorPin3, int inMotorPin4)
{
    stepperMotor.connectToPins(inMotorPin1, inMotorPin2, inMotorPin3, inMotorPin4);
    stepperMotor.setSpeedInStepsPerSecond(STEPPER_DEFAULT_SPEED);
    stepperMotor.setAccelerationInStepsPerSecondPerSecond(STEPPER_DEFAULT_ACCEL);
}

bool AxisController::ProcessMovement()
{
    if(!stepperMotor.motionComplete())
    {
        return stepperMotor.processMovement();
    }
    return true;
}