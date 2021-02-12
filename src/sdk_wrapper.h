
#include "pico/stdlib.h"

#define GPIO_GET(gpio) gpio_get(gpio)
#define GPIO_PUT(gpio, value) gpio_put(gpio, value)
#define GPIO_PUT(gpio, value) gpio_put(gpio, value)
#define SLEEP_US(us) sleep_us(us)
#define SLEEP_MS(us) sleep_ms(us)
#define _GPIO_OUT GPIO_OUT
#define _GPIO_IN GPIO_IN

#define GPIO_INIT_W_DIR(gpio, dir) \
    gpio_init(gpio);               \
    gpio_set_dir(gpio, dir);
