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
#define RYPIN 2
#define RXPIN 3
#define JPINS 0xf

typedef struct {
  float zero;
  float hrange;
} coffset_t;

typedef struct {
  coffset_t x;
  coffset_t y;
  coffset_t rx;
  coffset_t ry;
} callib_t;

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t rx;
  uint16_t ry;
} analog_sticks_t;

callib_t callibrate_zero(void);
int8_t get_next_smooth_single(const float, const float, const coffset_t);
hid_gamepad_report_t get_next_stick_report(const hid_gamepad_report_t, const analog_sticks_t,
                                     const callib_t);
uint16_t read_x(void);
uint16_t read_y(void);
uint16_t read_rx(void);
uint16_t read_ry(void);
analog_sticks_t read_all_axis(void);
