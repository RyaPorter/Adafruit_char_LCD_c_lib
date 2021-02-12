# CharLCD C Library
The standard Adafruit CharLCD library (Adafruit_Python_CHarLCD) translated into C for the Rpi Pico. The original (deprecated) python library can be found [here](https://github.com/adafruit/Adafruit_Python_CharLCD/blob/master/Adafruit_CharLCD/Adafruit_CharLCD.py).

Bit's of the library have been omitted as I had no need for them, namely RGB and backlight control.

I wired the pins `RS`, `EN`, `D4`, `D5`, `D6`, `D7` to GPIO pins 0, 1, 2, 3, 4, 5 respectively. These can be changed in the header definitions.
I was using the RPi Pico when developing this code, therefore the presence of the `pico_sdk` is assumed. The macros `GPIO_INIT_W_DIR`, `GPIO_PUT` and `SLEEP_US` can be changed to use this code with another board/sdk. Remember to remove the reference to `pico/stdlib.h` in `char_lcd.c` as well.

## Using the library
Nothing special really, should be pretty straight forward. Copy the files into your project. Change what you need to change, if you've for instance used different GPIO pins or are deving on a different board.
Call `lcd_init` to initialise the lcd display ready for sending text. Then call `lcd_writeln` to write text, `lcd_clear` to clear the lcd.
