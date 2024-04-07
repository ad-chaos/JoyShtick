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

  static int8_t px = 0;
  static int8_t py = 0;
  tud_hid_gamepad_report(/* id      = */ 0,
                         /* x       = */ px,
                         /* y       = */ py,
                         /* z       = */ 0,
                         /* rz      = */ 0,
                         /* rx      = */ 0,
                         /* ry      = */ 0,
                         /* dpad    = */ 0,
                         /* buttons = */ 0);

  px = get_next_smooth(px, read_x(), cal.x);
  py = get_next_smooth(py, read_y(), cal.y);
}
