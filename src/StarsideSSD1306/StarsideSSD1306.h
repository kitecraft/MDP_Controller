#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Bitter_8.h"
#include "Bitter_9.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_MID (SCREEN_WIDTH/2)
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8

#define HEADER_LABELS_Y 13
#define LABEL_HORIZ_X 10
#define LABEL_VERT_X (SCREEN_WIDTH/2 + 15)
#define HEADER_UNDERLINE_Y 17

enum DISPLAY_STEP_OPTIONS {
    DSO_SINGLEs = 0,
    DSO_TENs,
    DSO_25s,
    DSO_127m,
    DSO_17m,
    DSO_254m
};

enum DISPLAY_MODES {
  AM_MENU = 0,
  AM_MOVE
};

class StarsideSSD1306 {
private:
    Adafruit_SSD1306 display;
    String DisplaySteps[6] = {"1", "10", "25", "1.27", "1.70", "2.54"};
    GFXfont mediumFont = Bitter_VariableFont_wght8pt7b;
    GFXfont largeFont = Bitter_VariableFont_wght9pt7b;
    DISPLAY_MODES horizAxisMode = DISPLAY_MODES::AM_MOVE;
    DISPLAY_MODES vertAxisMode = DISPLAY_MODES::AM_MOVE;

    void UpdateSteps(DISPLAY_STEP_OPTIONS displayOption, int xOffset, bool inverted);
public:
    StarsideSSD1306();
    bool InitDisplay();

    void UpdateHorizSteps(DISPLAY_STEP_OPTIONS displayOption);
    void UpdateVertSteps(DISPLAY_STEP_OPTIONS displayOption);
    void ChangeHorizAxisMode(DISPLAY_MODES newMode);
    void ChangeVertAxisMode(DISPLAY_MODES newMode);
};