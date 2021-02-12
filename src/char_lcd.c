// Copyright (c) 2020 RyaPorter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// The logic and functions in this file were translated from the original python Adafruit_Python_CharLCD libary. The original license for this libary is below:

// Copyright (c) 2014 Adafruit Industries
// Author: Tony DiCola
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "char_lcd.h"
#include <stdio.h>
#include "sdk_wrapper.h"

void lcd_pulse_enable()
{
    GPIO_PUT(EN, 0);
    SLEEP_US(1);
    GPIO_PUT(EN, 1);
    SLEEP_US(1);
    GPIO_PUT(EN, 0);
    SLEEP_US(1);
}

void lcd_write8(uint8_t value, bool char_mode)
{
    SLEEP_US(1000);
    GPIO_PUT(RS, char_mode);

    GPIO_PUT(D4, ((value >> 4) & 1) > 0);
    GPIO_PUT(D5, ((value >> 5) & 1) > 0);
    GPIO_PUT(D6, ((value >> 6) & 1) > 0);
    GPIO_PUT(D7, ((value >> 7) & 1) > 0);

    lcd_pulse_enable();

    GPIO_PUT(D4, (value & 1) > 0);
    GPIO_PUT(D5, ((value >> 1) & 1) > 0);
    GPIO_PUT(D6, ((value >> 2) & 1) > 0);
    GPIO_PUT(D7, ((value >> 3) & 1) > 0);

    lcd_pulse_enable();
}

void lcd_clear()
{
    lcd_write8(LCD_CLEARDISPLAY, 0);
    SLEEP_US(3000);
}

void lcd_init(struct display_t *display)
{

    GPIO_INIT_W_DIR(RS, _GPIO_OUT);
    GPIO_INIT_W_DIR(EN, _GPIO_OUT);
    GPIO_INIT_W_DIR(D4, _GPIO_OUT);
    GPIO_INIT_W_DIR(D5, _GPIO_OUT);
    GPIO_INIT_W_DIR(D6, _GPIO_OUT);
    GPIO_INIT_W_DIR(D7, _GPIO_OUT);

    lcd_write8(0x32, 0);
    lcd_write8(0x32, 0);

    int displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    int displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_2LINE | LCD_5x8DOTS;
    int displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    lcd_write8(LCD_DISPLAYCONTROL | displaycontrol, 0);
    lcd_write8(LCD_FUNCTIONSET | displayfunction, 0);
    lcd_write8(LCD_ENTRYMODESET | displaymode, 0);

    lcd_clear();

    display->display_ctrl = displaycontrol;
    display->display_mode = displaymode;

    //set the entry mode
}

void lcd_home()
{
    lcd_write8(LCD_RETURNHOME, 0);
    SLEEP_US(3000);
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    if (row > LINES)
    {
        row = LINES - 1;
    }

    int row_offset;

    switch (row)
    {
    case 0:
        row_offset = LCD_ROW_OFFSET1;
        break;
    case 1:
        row_offset = LCD_ROW_OFFSET2;
        break;
    case 2:
        row_offset = LCD_ROW_OFFSET3;
        break;
    case 3:
        row_offset = LCD_ROW_OFFSET4;
        break;
    default:
        row_offset = LCD_ROW_OFFSET1;
        break;
    }

    lcd_write8(LCD_SETDDRAMADDR | (col + row_offset), 0);
}

void lcd_enable_display(bool enable, struct display_t *display)
{
    if (enable)
    {
       display-> display_ctrl |= LCD_DISPLAYON;
    }
    else
    {
        display->display_ctrl &= ~LCD_DISPLAYON;
    }
    lcd_write8(LCD_DISPLAYCONTROL | display->display_ctrl, 0);
}

void lcd_show_cursor(bool show, struct display_t *display)
{
    if (show)
    {
        display->display_ctrl |= LCD_CURSORON;
    }
    else
    {
        display->display_ctrl &= ~LCD_CURSORON;
    }
    lcd_write8(LCD_DISPLAYCONTROL | display->display_ctrl, 0);
}

void lcd_blink(bool blink, struct display_t *display)
{
    if (blink)
    {
        display->display_ctrl |= LCD_BLINKON;
    }
    else
    {
        display->display_ctrl &= ~LCD_BLINKON;
    }
    lcd_write8(LCD_DISPLAYCONTROL | display->display_ctrl, 0);
}

void lcd_move_left()
{
    lcd_write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, 0);
}

void lcd_move_right()
{
    lcd_write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, 0);
}

void lcd_set_left_to_right(struct display_t *display)
{
    display->display_mode |= LCD_ENTRYLEFT;
    lcd_write8(LCD_ENTRYMODESET | display->display_mode, 0);
}
void lcd_set_right_to_left(struct display_t *display)
{
    display->display_mode &= ~LCD_ENTRYLEFT;
    lcd_write8(LCD_ENTRYMODESET | display->display_mode, 0);
}

void lcd_autoscroll(bool autoscroll, struct display_t *display)
{
    if (autoscroll)
    {
        display->display_mode |= LCD_ENTRYSHIFTINCREMENT;
    }
    else
    {
        display->display_mode &= ~LCD_ENTRYSHIFTINCREMENT;
    }
    lcd_write8(LCD_ENTRYMODESET | display->display_mode, 0);
}
void lcd_writeln(char *str)
{
    int line = 0;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '\n')
        {
            line = line + 1;
            lcd_set_cursor(0, line);
        }
        else
        {
            lcd_write8(str[i], 1);
        }
    }
}