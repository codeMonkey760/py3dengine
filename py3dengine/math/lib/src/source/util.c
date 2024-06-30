#include <math.h>

#include "util.h"

float clampRadians(float radianValue) {
    if (radianValue >= 0.0f) {
        while (radianValue > M_TWO_PI) {
            radianValue -= M_TWO_PI;
        }
    } else {
        while (radianValue < 0.0f) {
            radianValue += M_TWO_PI;
        }
    }

    return radianValue;
}

float clampValue(float value, float max_value) {
    while (value >= max_value) {
        value -= max_value;
    }

    return value;
}
