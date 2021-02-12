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
#include "pico/stdlib.h"
#include <stdio.h>

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

void lcd_init()
{

    GPIO_INIT_W_DIR(RS, GPIO_OUT);
    GPIO_INIT_W_DIR(EN, GPIO_OUT);
    GPIO_INIT_W_DIR(D4, GPIO_OUT);
    GPIO_INIT_W_DIR(D5, GPIO_OUT);
    GPIO_INIT_W_DIR(D6, GPIO_OUT);
    GPIO_INIT_W_DIR(D7, GPIO_OUT);

    lcd_write8(0x32, 0);
    lcd_write8(0x32, 0);

    int displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    int displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_2LINE | LCD_5x8DOTS;
    int displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    lcd_write8(LCD_DISPLAYCONTROL | displaycontrol, 0);
    lcd_write8(LCD_FUNCTIONSET | displayfunction, 0);
    lcd_write8(LCD_ENTRYMODESET | displaymode, 0);

    lcd_clear();

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

void lcd_write_display_ctrl(int ctrl, bool _true)
{
    int displaycontrol;
    if (_true)
    {
        displaycontrol |= ctrl;
    }
    else
    {
        displaycontrol &= ~ctrl;
    }
    lcd_write8(LCD_DISPLAYCONTROL | displaycontrol, 0);
}

void lcd_enable_display(bool enable)
{
    lcd_write_display_ctrl(LCD_DISPLAYON, enable);
}

void lcd_show_cursor(bool show)
{
    lcd_write_display_ctrl(LCD_CURSORON, show);
}

void lcd_blink(bool blink)
{
    lcd_write_display_ctrl(LCD_BLINKON, blink);
}

void lcd_move_left()
{
    lcd_write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, 0);
}

void lcd_move_right()
{
    lcd_write8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, 0);
}

void lcd_set_left_to_right()
{
    int displaymode;
    displaymode |= LCD_ENTRYLEFT;
    lcd_write8(LCD_ENTRYMODESET | displaymode, 0);
}
void lcd_set_right_to_left()
{
    int displaymode;
    displaymode &= ~LCD_ENTRYLEFT;
    lcd_write8(LCD_ENTRYMODESET | displaymode, 0);
}

void lcd_autoscroll(bool autoscroll)
{
    lcd_write_display_ctrl(LCD_ENTRYSHIFTINCREMENT, autoscroll);
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