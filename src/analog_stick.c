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
  return (analog_sticks_t){
      .x = read_x(),
      .y = read_y(),
      .rx = read_rx(),
      .ry = read_ry(),
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

hid_gamepad_report_t INLINE get_next_stick_report(const hid_gamepad_report_t current,
                                            const analog_sticks_t input,
                                            const callib_t callib) {
#define next_smooth(which)                                                     \
  get_next_smooth_single(current.which, input.which, callib.which)
  return (hid_gamepad_report_t){
      .x = next_smooth(x),
      .y = next_smooth(y),
  };
#undef next_smooth
}

callib_t callibrate_zero() {
  float zx = 0., zy = 0.;
  float zrx = 0., zry = 0.;

  for (uint8_t i = 0; i < 64; i++) {
    zy = (i * zy + read_y()) / (i + 1);
    zx = (i * zx + read_x()) / (i + 1);
    zry = (i * zy + read_ry()) / (i + 1);
    zrx = (i * zx + read_rx()) / (i + 1);
  }

  float hrx = MAX(zx, (1 << 12) - zx);
  float hry = MAX(zy, (1 << 12) - zy);
  float hrrx = MAX(zrx, (1 << 12) - zrx);
  float hrry = MAX(zry, (1 << 12) - zry);

  return (callib_t){.x = {.zero = zx, .hrange = hrx},
                    .y = {.zero = zy, .hrange = hry},
                    .rx = {.zero = zrx, .hrange = hrrx},
                    .ry = {.zero = zry, .hrange = hrry}};
}
