#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int counter = 0;

void setup(void)
{
    // change hardware I2C pins to (5,4) (D1,D2)
    Wire.begin(5, 4);
    // init display
    u8g2.begin();
}

void drawLabelTop(U8G2 u8g2, uint8_t day)
{
    u8g2.setFont(u8g2_font_5x8_tf);
    switch (day)
    {
    case 0:
        u8g2.drawStr(50, 8, "Monday");
        break;
    case 1:
        u8g2.drawStr(48, 8, "Tuesday");
        break;
    case 2:
        u8g2.drawStr(43, 8, "Wednesday");
        break;
    case 3:
        u8g2.drawStr(45, 8, "Thursday");
        break;
    case 4:
        u8g2.drawStr(50, 8, "Friday");
        break;
    case 5:
        u8g2.drawStr(46, 8, "Saturday");
        break;
    case 6:
        u8g2.drawStr(50, 8, "Sunday");
        break;
    }

    u8g2.drawRFrame(u8g2.getDisplayWidth() / 4, -20, u8g2.getDisplayWidth() / 2, 32, 4);
}

void drawSignal(U8G2 u8g2, uint8_t strength)
{
    for (uint8_t i = 0; i < strength; i++)
    {
        u8g2.drawVLine(u8g2.getDisplayWidth() - 10 + i * 2, 6 - i, i);
    }
}

void drawMiniBattery(U8G2 u8g2, uint8_t level)
{
    u8g2.drawHLine(u8g2.getDisplayWidth() - 20, 2, 8);
    u8g2.drawPixel(u8g2.getDisplayWidth() - 20 + 8, 3);
    u8g2.drawPixel(u8g2.getDisplayWidth() - 20 + 8, 4);
    u8g2.drawPixel(u8g2.getDisplayWidth() - 20, 3);
    u8g2.drawPixel(u8g2.getDisplayWidth() - 20, 4);
    u8g2.drawHLine(u8g2.getDisplayWidth() - 20, 5, 8);

    u8g2.drawHLine(u8g2.getDisplayWidth() - 19, 3, level % 8);
    u8g2.drawHLine(u8g2.getDisplayWidth() - 19, 4, level % 8);
}

void drawTime(U8G2 u8g2)
{
    u8g2.setFont(u8g2_font_courB24_tf);

    u8g2.drawStr(16, 41, (String("08:") + (counter % 60 < 10 ? "0" : "") + String(counter % 60)).c_str());
}

void drawTemp(U8G2 u8g2, float value)
{
    u8g2.setFont(u8g2_font_5x8_tf);
    u8g2.drawStr(1, u8g2.getDisplayHeight() - 1, (String(value) + String("C")).c_str());

    u8g2.drawRFrame(-4, u8g2.getDisplayHeight() - 11, 64, 24, 4);
}

void drawPres(U8G2 u8g2, float value)
{
    u8g2.setFont(u8g2_font_5x8_tf);
    u8g2.drawStr(u8g2.getDisplayWidth() - (String(value).length() + 2) * 5, u8g2.getDisplayHeight() - 1, (String(value) + "mb").c_str());

    u8g2.drawRFrame(u8g2.getDisplayWidth() / 2 + 4, u8g2.getDisplayHeight() - 11, 64, 24, 4);
}

void draw(U8G2 u8g2)
{
    u8g2.setDrawColor(1);
    drawLabelTop(u8g2, counter % 7);
    drawSignal(u8g2, counter % 7);
    drawMiniBattery(u8g2, counter);
    drawTime(u8g2);
    drawTemp(u8g2, 23.4);
    drawPres(u8g2, 1024.3);
}

void loop(void)
{
    u8g2.clearBuffer(); // clear the internal memory
    draw(u8g2);
    u8g2.sendBuffer(); // transfer internal memory to the display
    delay(500);
    counter++;
}
