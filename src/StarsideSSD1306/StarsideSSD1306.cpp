#include "StarsideSSD1306.h"

StarsideSSD1306::StarsideSSD1306()
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
}

bool StarsideSSD1306::InitDisplay()
{

    if(!display.begin()) {
        return false;
    }
    //if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    //    return false;
   // }
    display.setTextColor(WHITE);
	display.setFont(&mediumFont);

    display.clearDisplay();

    display.drawFastVLine(SCREEN_MID, 0, SCREEN_HEIGHT, WHITE);
    display.setCursor(LABEL_HORIZ_X, HEADER_LABELS_Y);
    display.print("Horiz");

    display.setCursor(LABEL_VERT_X, HEADER_LABELS_Y);
    display.print("Vert");

    display.drawFastHLine(0, HEADER_UNDERLINE_Y, SCREEN_WIDTH, WHITE);

    display.display();

    return true;
}

void StarsideSSD1306::UpdateHorizSteps(DISPLAY_STEP_OPTIONS displayOption)
{
    bool inverted = (horizAxisMode == AM_MENU) ? true : false;
    UpdateSteps(displayOption, 0, inverted);
}

void StarsideSSD1306::UpdateVertSteps(DISPLAY_STEP_OPTIONS displayOption)
{
    bool inverted = (vertAxisMode == AM_MENU) ? true : false;
    UpdateSteps(displayOption, SCREEN_MID, inverted);
}

void StarsideSSD1306::UpdateSteps(DISPLAY_STEP_OPTIONS displayOption, int xOffset, bool inverted)
{
    if(inverted)
    {
        display.fillRect(xOffset + 4, 20, SCREEN_WIDTH/2 - 8, 44, WHITE);
        display.setTextColor(BLACK);
    }
    else {
        display.fillRect(xOffset + 4, 20, SCREEN_WIDTH/2 - 8, 44, BLACK);
        display.setTextColor(WHITE);
    }

    display.setFont(&largeFont);

    uint16_t x1, y1;
    uint16_t w1, h1;
    display.getTextBounds(DisplaySteps[displayOption].c_str(), 0, 0, &x1, &y1, &w1, &h1);

    display.setCursor(32 - (w1/2) + xOffset, 39);
    display.print(DisplaySteps[displayOption]);


    String message = "";

    switch(displayOption){
    case DSO_SINGLEs:
        message = "step";
        break;
    case DSO_TENs:
    case DSO_25s:
        message = "steps";
        break;
    case DSO_127m:
    case DSO_17m:
    case DSO_254m:
    case DSO_450m:
    case DSO_508m:
        message = "mm";
        break;
    }

    display.getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w1, &h1);
    display.setCursor(32 - (w1/2) + xOffset, 54);

    display.print(message);
    display.display();
}

void StarsideSSD1306::ChangeHorizAxisMode(DISPLAY_MODES newMode)
{
     horizAxisMode = newMode;
}

void StarsideSSD1306::ChangeVertAxisMode(DISPLAY_MODES newMode)
{
    vertAxisMode = newMode;
}