#pragma once

#include "class/hid/hid.h"
#include <stdint.h>

#define INLINE __attribute__((always_inline)) inline
#define ALPHA(span) (2. / ((span) + 1.))
#define CLAMP(value, hrange) MIN(MAX(value, -hrange), hrange)
#define SMOOTHEN(prev, input, span)                                            \
  ((1. - ALPHA(span)) * prev + ALPHA(span) * input)

#define YPIN 0
#define XPIN 1
#define JPINS 0xf

typedef struct {
  float zero;
  float hrange;
} coffset_t;

typedef struct {
  coffset_t x;
  coffset_t y;
} callib_t;

typedef struct {
  uint16_t x;
  uint16_t y;
} analog_sticks_t;

callib_t callibrate_zero(void);
int8_t get_next_smooth_single(const float, const float, const coffset_t);
hid_gamepad_report_t get_next_stick_report(const hid_gamepad_report_t, const analog_sticks_t,
                                     const callib_t);
uint16_t read_x(void);
uint16_t read_y(void);
analog_sticks_t read_all_axis(void);
