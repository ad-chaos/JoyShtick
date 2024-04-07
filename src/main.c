#include <bsp/board.h>
#include <hardware/adc.h>
#include <tusb.h>

#include "analog_stick.h"

static callib_t cal;
void hid_task(void);

int main(void) {
  board_init();
  tusb_init();

  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);

  cal = callibrate_zero();

  while (1) {
    tud_task();
    hid_task();
  }
}

void hid_task(void) {
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if (board_millis() - start_ms < interval_ms)
    return; // not enough time
  start_ms += interval_ms;

  if (!tud_hid_ready())
    return;

  static hid_gamepad_report_t greport = {};
  tud_hid_report(0, &greport, sizeof(greport));
  greport = get_next_stick_report(greport, read_all_axis(), cal);
}
