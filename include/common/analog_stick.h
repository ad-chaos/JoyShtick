#pragma once

#include <stdint.h>

#define INLINE __attribute__((always_inline)) inline
#define ALPHA(span) (2. / ((span) + 1.))
#define CLAMP(value, hrange) MIN(MAX(value, -hrange), hrange)
#define SMOOTHEN(prev, input, span)                                            \
  ((1. - ALPHA(span)) * prev + ALPHA(span) * input)

#define YPIN 0
#define XPIN 1

typedef struct {
  float zero;
  float hrange;
} coffset_t;

typedef struct {
  coffset_t x;
  coffset_t y;
} callib_t;

callib_t callibrate_zero(void);
int8_t get_next_smooth(float current, float input, coffset_t callib);
float read_x(void);
float read_y(void);
