#pragma once
#include <Arduino.h>
#include <TinyStepper_28BYJ_48.h>

#define STEPS_PER_REVOUTION 2048
#define STEPPER_DEFAULT_SPEED 200
#define STEPPER_DEFAULT_ACCEL 200

// Spur Gears:  12 tooth
// tooth distance = 6.2mm
// step distance:  0.036328125 mm
// 2.54mm = 69.918 steps  (70)

enum STEP_OPTIONS {
    SO_SINGLEs = 1,
    SO_TENs = 10,
    SO_25s = 25,
    SO_127m = 35,
    SO_17m = 47,
    SO_254m = 70,
    SO_450m = 124,
    SO_508m = 140
};

enum AXIS_DIRECTION {
  AXIS_DIRECTION_POSTIVE = 0,
  AXIS_DIRECTION_NEGATIVE
};

class AxisController {
private:
    TinyStepper_28BYJ_48 stepperMotor;
    long stepDirectionRectifier = -1;
    STEP_OPTIONS currentStepSize = SO_254m;

    long RectifySteps(){ return ((long)currentStepSize * stepDirectionRectifier); };

public:
    AxisController();
    void Init(int inMotorPin1, int inMotorPin2, int inMotorPin3, int inMotorPin4);

    void ReversePolarity(){ stepDirectionRectifier = 1; };
    void NormalPolority(){ stepDirectionRectifier = -1; };
    void SetStepSize(STEP_OPTIONS newStepSize){ currentStepSize = newStepSize; };

    void StepPostive(){ stepperMotor.setupRelativeMoveInSteps(RectifySteps()); };
    void StepNegative(){ stepperMotor.setupRelativeMoveInSteps(RectifySteps() * -1); };

    bool ProcessMovement();
    bool InMotion(){ return !stepperMotor.motionComplete(); };
};