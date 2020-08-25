#include <Arduino.h>
#include "AxisController/AxisController.h"
#include "StarsideSSD1306/StarsideSSD1306.h"
#include <Encoder.h>

void XAxisButtonInterrupt();
void YAxisButtonInterrupt();
void HandleYAxisEncoder();
void HandleXAxisEncoder();
void HandleEncoder(Encoder &thisEncoder, long &currentValue, AxisController &axisController);

StarsideSSD1306 oledDisplay;

enum ENCODER_BTN {
  ENCODER_BTN_X = 0,
  ENCODER_BTN_Y,
  ENCODER_BTN_NONE
};

#define BUTTON_DEBOUNCE_TIME 250

volatile bool encoderButtonPressed = false;
volatile ENCODER_BTN encoderButtonActivated = ENCODER_BTN_NONE;

long yAxisButton_LastPressed = 0;
long xAxisButton_LastPressed = 0;

Encoder yAxisEncoder(6, 7);
long yAxisEncoder_CurrentValue = 0;

Encoder xAxisEncoder(3, 4);
long xAxisEncoder_CurrentValue = 0;

int xAxisButtonPin = 2;
int yAxisButtonPin = 5;

AxisController YAxis;
AxisController XAxis;

void setup() {
  Serial.begin(115200);
  oledDisplay.InitDisplay();
  pinMode(yAxisButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(yAxisButtonPin), YAxisButtonInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(xAxisButtonPin), XAxisButtonInterrupt, FALLING);

  YAxis.Init(23, 22, 21, 20);
  XAxis.Init(19, 18, 17, 16);

  oledDisplay.UpdateHorizSteps(DSO_254m);
  oledDisplay.UpdateVertSteps(DSO_254m);
  yAxisEncoder.write(0);
}

void loop() {
  HandleXAxisEncoder();
  HandleYAxisEncoder();


  if(encoderButtonPressed)
  {
    if(encoderButtonActivated == ENCODER_BTN_Y)
    {
      if(millis() - yAxisButton_LastPressed > BUTTON_DEBOUNCE_TIME)
      {
        Serial.println("YAxis Button pressed");
        yAxisButton_LastPressed = millis();
        encoderButtonPressed = false;
      }
    }
    if(encoderButtonActivated == ENCODER_BTN_X)
    {
      if(millis() - xAxisButton_LastPressed > BUTTON_DEBOUNCE_TIME)
      {
        Serial.println("XAxis Button pressed");
        xAxisButton_LastPressed = millis();
        encoderButtonPressed = false;
      }
    }
  }

  XAxis.ProcessMovement();
  YAxis.ProcessMovement();
}

void HandleYAxisEncoder()
{
  HandleEncoder(yAxisEncoder, yAxisEncoder_CurrentValue, YAxis);
}

void HandleXAxisEncoder()
{
  HandleEncoder(xAxisEncoder, xAxisEncoder_CurrentValue, XAxis);
}

void HandleEncoder(Encoder &thisEncoder, long &currentValue, AxisController &axisController)
{
  long newValue = thisEncoder.read();

  if((newValue != currentValue) && newValue % 2 == 0)
  {
    (newValue > currentValue) ? axisController.StepPostive() : axisController.StepNegative();
    currentValue = newValue;
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