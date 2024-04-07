#include <hardware/adc.h>
#include <pico/stdlib.h>
#include <stdio.h>

#include "analog_stick.h"

static callib_t cal;

int main() {
  stdio_init_all();

  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);

  cal = callibrate_zero();
  // because cal.##which is an invalid preprocessing token :(
#define next_smooth(which) get_next_smooth(p##which, read_##which(), c##which)
  coffset_t cx = cal.x;
  coffset_t cy = cal.y;

  int8_t px = 0, py = 0;
  while (true) {
    px = next_smooth(x);
    py = next_smooth(y);
    printf("%d %d\n", px, py);
    sleep_ms(20);
  }
}
