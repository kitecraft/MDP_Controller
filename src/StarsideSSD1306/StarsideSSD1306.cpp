#include "StarsideSSD1306.h"

StarsideSSD1306::StarsideSSD1306()
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
}

bool StarsideSSD1306::InitDisplay()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {
        return false;
    }
    display.setTextColor(WHITE);
	display.setFont(&mediumFont);

    display.clearDisplay();

    display.drawFastVLine(SCREEN_WIDTH/2, 0, SCREEN_HEIGHT, WHITE);
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
    display.fillRect(3, 30, SCREEN_WIDTH/2 - 6, 34, BLACK);
    display.setFont(&largeFont);

    uint16_t x1, y1;
    uint16_t w1, h1;
    display.getTextBounds(DisplaySteps[displayOption].c_str(), 0, 0, &x1, &y1, &w1, &h1);

    display.setCursor(32 - (w1/2), 42);

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
        message = "mm";
        break;
    }

    display.getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w1, &h1);

    display.setCursor(32 - (w1/2), 57);
    display.print(message);
    display.display();
}

void StarsideSSD1306::UpdateVertSteps(DISPLAY_STEP_OPTIONS displayOption)
{
    display.fillRect(SCREEN_WIDTH/2 + 3, 30, SCREEN_WIDTH/2 - 6, 34, BLACK);
    display.setFont(&largeFont);

    uint16_t x1, y1;
    uint16_t w1, h1;
    display.getTextBounds(DisplaySteps[displayOption].c_str(), 0, 0, &x1, &y1, &w1, &h1);

    display.setCursor(32 - (w1/2) + (SCREEN_WIDTH/2), 42);
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
        message = "mm";
        break;
    }

    display.getTextBounds(message.c_str(), 0, 0, &x1, &y1, &w1, &h1);
    display.setCursor(32 - (w1/2) + (SCREEN_WIDTH/2), 57);

    display.print(message);

    display.display();
}