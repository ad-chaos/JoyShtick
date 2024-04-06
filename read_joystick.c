#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

uint16_t max_x = 0, min_x = UINT16_MAX, max_y = 0, min_y = UINT16_MAX, zero;

void print_pins() {
  adc_select_input(1);
  uint16_t result_x = adc_read();
  adc_select_input(0);
  uint16_t result_y = adc_read();
  printf("%d %d\n", result_x, result_y);
}

int main() {
  stdio_init_all();

  adc_init();

  adc_gpio_init(26);
  adc_gpio_init(27);

  while (1) {
    print_pins();
    sleep_ms(10);
  }
}
