# Adafruit_char_LCD_c_lib
The standard Adafruit CharLCD library translated into c for the Rpi Pico. The original (deprecated) python library can be found [here](https://github.com/adafruit/Adafruit_Python_CharLCD/blob/master/Adafruit_CharLCD/Adafruit_CharLCD.py).

Bit's of the library have been omitted as I had no need for them, namely RGB and backlight control.

I wired the pins `RS`, `EN`, `D4`, `D5`, `D6`, `D7` to GPIO pins 0, 1, 2, 3, 4, 5 respectively. These can be changed in the header definitions.
I was using the RPi Pico when developing this code, therefore the presence of the `pico_sdk` is assumed. The macros `GPIO_INIT_W_DIR` and `GPIO_PUT` can be changed to use this code with another board/sdk. Remember to remove the reference to `pico/stdlib.h` in `char_lcd.c` as well.

## Library use
Nothing special. Copy the files into your project. Change what you need to change.
