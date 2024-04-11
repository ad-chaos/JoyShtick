#include "analog_stick.h"
#include <hardware/adc.h>

uint16_t INLINE read_x() {
  adc_select_input(XPIN);
  return adc_read();
}

uint16_t INLINE read_y() {
  adc_select_input(YPIN);
  return adc_read();
}

uint16_t INLINE read_rx() {
  adc_select_input(RXPIN);
  return adc_read();
}

uint16_t INLINE read_ry() {
  adc_select_input(RYPIN);
  return adc_read();
}

analog_sticks_t INLINE read_all_axis() {
  adc_set_round_robin(JPINS);
  return (analog_sticks_t){
      .x = adc_read(),
      .y = adc_read(),
      .rx = adc_read(),
      .ry = adc_read(),
  };
}

int8_t INLINE get_next_smooth_single(const float current, const float input,
                                     const coffset_t callib) {
  return SMOOTHEN(
      /*prev=*/current,
      /*input=*/CLAMP(input - callib.zero, callib.hrange) * 127. /
          callib.hrange,
      /*span=*/4.);
}

hid_gamepad_report_t INLINE
get_next_stick_report(const hid_gamepad_report_t current,
                      const analog_sticks_t input, const callib_t callib) {
#define next_smooth(which)                                                     \
  get_next_smooth_single(current.which, input.which, callib.which)
  return (hid_gamepad_report_t){
      .x = next_smooth(x),
      .y = next_smooth(y),
  };
}

callib_t callibrate_zero() {
  float zx = 0., zy = 0.;
  float zrx = 0., zry = 0.;

  for (uint8_t i = 0; i < 64; i++) {
    analog_sticks_t sticks = read_all_axis();
#define cumulative_average(of) z##of = (i * z##of + sticks.of) / (i + 1)
    cumulative_average(x);
    cumulative_average(y);
    cumulative_average(rx);
    cumulative_average(ry);
  }

#define half_range(of) float hr##of = MAX(z##of, (1 << 12) - z##of);
  half_range(x);
  half_range(y);
  half_range(rx);
  half_range(ry);

  return (callib_t){.x = {.zero = zx, .hrange = hrx},
                    .y = {.zero = zy, .hrange = hry},
                    .rx = {.zero = zrx, .hrange = hrrx},
                    .ry = {.zero = zry, .hrange = hrry}};
}
