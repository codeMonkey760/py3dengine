#ifndef UTIL_H_
#define UTIL_H_

#include <math.h>

#define M_TWO_PI (2.0f * ((float) M_PI))
#define DEG_TO_RAD(X) ((X) * 0.017453293)
#define RAD_TO_DEG(X) ((X) * 57.295779579)

float clampRadians(float radianValue);
float clampValue(float value, float max_value);

#endif

