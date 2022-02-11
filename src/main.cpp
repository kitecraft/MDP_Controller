#include <Arduino.h>
#include "AxisController/AxisController.h"
#include "StarsideSSD1306/StarsideSSD1306.h"
#include <Encoder.h>
#include <Bounce2.h>

void XAxisButtonInterrupt();
void YAxisButtonInterrupt();
void HandleButtonPress();
void HandleYAxisEncoder();
void HandleXAxisEncoder();
bool HandleEncoder(Encoder &thisEncoder, long &currentValue, AxisController &axisController, DISPLAY_MODES currentMode, int &returnValue);


StarsideSSD1306 oledDisplay;


AxisController xAxis;
AxisController yAxis;

enum ENCODER_BTN {
  ENCODER_BTN_X = 0,
  ENCODER_BTN_Y,
  ENCODER_BTN_NONE
};

#define BUTTON_DEBOUNCE_TIME 250

volatile bool encoderButtonPressed = false;
volatile ENCODER_BTN encoderButtonActivated = ENCODER_BTN_NONE;

long xAxisButton_LastPressed = 0;
long yAxisButton_LastPressed = 0;

Encoder yAxisEncoder(23, 22);
long yAxisEncoder_CurrentValue = 0;

Encoder xAxisEncoder(6, 7);
long xAxisEncoder_CurrentValue = 0;

int yAxisButtonPin = 0;
int xAxisButtonPin = 5;


struct MDP_Options {
  DISPLAY_STEP_OPTIONS displayStepOption = DSO_254m;
  STEP_OPTIONS stepOption = SO_254m;
  MDP_Options(DISPLAY_STEP_OPTIONS newDSO, STEP_OPTIONS newSO) {
    displayStepOption = newDSO;
    stepOption = newSO;
  };
};

MDP_Options allMDPOptions[9] = {MDP_Options(DSO_SINGLEs, SO_SINGLEs),
                                MDP_Options(DSO_TENs, SO_TENs),
                                MDP_Options(DSO_25s, SO_25s),
                                MDP_Options(DSO_058m, SO_058m),
                                MDP_Options(DSO_127m, SO_127m),
                                MDP_Options(DSO_17m, SO_17m),
                                MDP_Options(DSO_254m, SO_254m),
                                MDP_Options(DSO_450m, SO_450m),
                                MDP_Options(DSO_508m, SO_508m) };

int currentXAxisOption = 5;
int currentYAxisOption = 5;


DISPLAY_MODES xAxisMode = AM_MOVE;
DISPLAY_MODES yAxisMode = AM_MOVE;

void setup() {
  oledDisplay.InitDisplay();

  yAxisEncoder.write(0);
  xAxisEncoder.write(0);

  pinMode(xAxisButtonPin, INPUT_PULLUP);
  pinMode(yAxisButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(yAxisButtonPin), YAxisButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(xAxisButtonPin), XAxisButtonInterrupt, FALLING);

  xAxis.Init(17, 16, 14, 15);
  yAxis.Init(21, 20, 19, 18);


  xAxis.SetStepSize(allMDPOptions[currentXAxisOption].stepOption);
  yAxis.SetStepSize(allMDPOptions[currentYAxisOption].stepOption);

  oledDisplay.UpdateHorizSteps(allMDPOptions[currentXAxisOption].displayStepOption);
  oledDisplay.UpdateVertSteps(allMDPOptions[currentXAxisOption].displayStepOption);

}

void loop() {
  HandleXAxisEncoder();
  HandleYAxisEncoder();

  HandleButtonPress();

  xAxis.ProcessMovement();
  yAxis.ProcessMovement();
}

void HandleYAxisEncoder()
{
  int ret = 0;
  bool direction = HandleEncoder(yAxisEncoder, yAxisEncoder_CurrentValue, yAxis, yAxisMode, ret);

  if(ret == 1)
  {
    if(direction)
    {
      currentYAxisOption++;
      if(currentYAxisOption > 7)
      {
        currentYAxisOption = 0;
      }
    }
    else
    {
      currentYAxisOption--;
      if(currentYAxisOption < 0)
      {
        currentYAxisOption = 7;
      }
    }

    yAxis.SetStepSize(allMDPOptions[currentYAxisOption].stepOption);
    oledDisplay.UpdateVertSteps(allMDPOptions[currentYAxisOption].displayStepOption);
  }
}

void HandleXAxisEncoder()
{
  int ret = 0;
  bool direction = HandleEncoder(xAxisEncoder, xAxisEncoder_CurrentValue, xAxis, xAxisMode, ret);

  if(ret == 1)
  {
    if(direction)
    {
      currentXAxisOption++;
      if(currentXAxisOption > 7)
      {
        currentXAxisOption = 0;
      }
    }
    else
    {
      currentXAxisOption--;
      if(currentXAxisOption < 0)
      {
        currentXAxisOption = 7;
      }
    }

    xAxis.SetStepSize(allMDPOptions[currentXAxisOption].stepOption);
    oledDisplay.UpdateHorizSteps(allMDPOptions[currentXAxisOption].displayStepOption);
  }
}

bool HandleEncoder(Encoder &thisEncoder, long &currentValue, AxisController &axisController, DISPLAY_MODES currentMode, int &returnValue)
{
  returnValue = -1;
  long newValue = thisEncoder.read();

  if((newValue != currentValue) && newValue % 2 == 0)
  {
    bool direction = (newValue > currentValue) ? true : false;
    currentValue = newValue;

    if(currentMode == DISPLAY_MODES::AM_MOVE)
    {
      direction == true ? axisController.StepPostive() : axisController.StepNegative();
      returnValue = 0;
    }
    else
    {
      returnValue = 1;
    }

    return direction;
  }

  return false;
}

void HandleButtonPress()
{
  if(encoderButtonPressed)
  {
    if(encoderButtonActivated == ENCODER_BTN_X)
    {
      if(millis() - xAxisButton_LastPressed > BUTTON_DEBOUNCE_TIME)
      {
        xAxisMode = (xAxisMode == AM_MENU) ? AM_MOVE : AM_MENU;
        oledDisplay.ChangeHorizAxisMode(xAxisMode);
        oledDisplay.UpdateHorizSteps(allMDPOptions[currentXAxisOption].displayStepOption);

        xAxisButton_LastPressed = millis();
        encoderButtonPressed = false;
      }
    }

    if(encoderButtonActivated == ENCODER_BTN_Y)
    {
      if(millis() - yAxisButton_LastPressed > BUTTON_DEBOUNCE_TIME)
      {
        yAxisMode = (yAxisMode == AM_MENU) ? AM_MOVE : AM_MENU;
        oledDisplay.ChangeVertAxisMode(yAxisMode);
        oledDisplay.UpdateVertSteps(allMDPOptions[currentYAxisOption].displayStepOption);

        yAxisButton_LastPressed = millis();
        encoderButtonPressed = false;
      }
    }
  }

}

void XAxisButtonInterrupt()
{
  encoderButtonPressed = true;
  encoderButtonActivated = ENCODER_BTN_X;

}

void YAxisButtonInterrupt()
{
  encoderButtonPressed = true;
  encoderButtonActivated = ENCODER_BTN_Y;
}