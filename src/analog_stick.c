#include "analog_stick.h"
#include <hardware/adc.h>

float INLINE read_x() {
  adc_select_input(XPIN);
  return adc_read();
}

float INLINE read_y() {
  adc_select_input(YPIN);
  return adc_read();
}

int8_t INLINE get_next_smooth(float current, float input, coffset_t callib) {
  return SMOOTHEN(
      /*prev=*/current,
      /*input=*/CLAMP(input - callib.zero, callib.hrange) * 127. / callib.hrange,
      /*span=*/4.);
}

callib_t callibrate_zero() {
  float zx = 0., zy = 0.;

  for (uint8_t i = 0; i < 64; i++) {
    zy = (i * zy + read_y()) / (i + 1);
    zx = (i * zx + read_x()) / (i + 1);
  }

  float hrx = MAX(zx, (1 << 12) - zx);
  float hry = MAX(zy, (1 << 12) - zy);

  return (callib_t){.x = {.zero = zx, .hrange = hrx},
                    .y = {.zero = zy, .hrange = hry}};
}
