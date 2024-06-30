#include <math.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"

/** Vector 2 */
void Vec2Identity(float out[VEC_2_SIZE]) {
    Vec2Fill(out, 0.0f);
}

void Vec2Copy(float out[VEC_2_SIZE], const float v[VEC_2_SIZE]) {
    if (out == NULL || v == NULL) return;

    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = v[i];
    }
}

void Vec2Fill(float out[VEC_2_SIZE], float value) {
    if (out == NULL) return;

    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = value;
    }
}

void Vec2Add(float out[VEC_2_SIZE], const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = v1[i] + v2[i];
    }
}

void Vec2Subtract(float out[VEC_2_SIZE], const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = v1[i] - v2[i];
    }
}

void Vec2Scalar(float out[VEC_2_SIZE], const float v[VEC_2_SIZE], float scalar) {
    if (v == NULL || out == NULL) return;

    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = v[i] * scalar;
    }
}

void Vec2Dot(float *out, const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    (*out) = (v1[0] * v2[0]) + (v1[1] * v2[1]);
}

void Vec2Normalize(float out[VEC_2_SIZE], const float v[VEC_2_SIZE]) {
    if (out == NULL || v == NULL) return;

    float len = sqrtf((v[0] * v[0]) + (v[1] * v[1]));
    for (int i = 0; i < VEC_2_SIZE; ++i) {
        out[i] = v[i] / len;
    }
}

/** Vector 3 */
void Vec3Identity(float out[VEC_3_SIZE]) {
    Vec3Fill(out, 0.0f);
}

void Vec3Copy(float out[VEC_3_SIZE], const float v[VEC_3_SIZE]) {
    if (out == NULL || v == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = v[i];
    }
}

void Vec3Fill(float out[VEC_3_SIZE], float value) {
    if (out == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = value;
    }
}

void Vec3ToCStr(char *out, const float v[VEC_3_SIZE], ssize_t size) {
    if (out == NULL || v == NULL || size <= 1) return;

    memset(out, 0, sizeof(char) * size);

    snprintf(out, size-1, "Vector3(%.2f, %.2f, %.2f)", v[0], v[1], v[2]);
}

void Vec3Add(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = v1[i] + v2[i];
    }
}

void Vec3Subtract(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = v1[i] - v2[i];
    }
}

void Vec3Scalar(float out[VEC_3_SIZE], const float v[VEC_3_SIZE], float scalar) {
    if (v == NULL || out == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = v[i] * scalar;
    }
}

void Vec3Dot(float *out, const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;

    (*out) = (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

void Vec3Cross(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]) {
    if (out == NULL || v1 == NULL || v2 == NULL) return;
    float temp[VEC_3_SIZE] = {0.0f};

    temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    Vec3Copy(out, temp);
}

void Vec3Divide(float out[VEC_3_SIZE], const float v[VEC_3_SIZE], float scalar) {
    if (v == NULL || out == NULL) return;

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = v[i] / scalar;
    }
}

void Vec3Length(float *out, const float v[VEC_3_SIZE]) {
    if (out == NULL || v == NULL) return;

    (*out) = sqrtf((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
}

void Vec3Normalize(float out[VEC_3_SIZE], const float v[VEC_3_SIZE]) {
    if (out == NULL || v == NULL) return;

    float len = 0.0f;
    Vec3Length(&len, v);
    if (len == 0.0f) return;

    Vec3Divide(out, v, len);
}

/** Vector4 */
void Vec4Identity(float out[VEC_4_SIZE]) {
    Vec3Fill(out, 0.0f);
    out[3] = 1.0f;
}

void Vec4Copy(float out[VEC_4_SIZE], const float v[VEC_4_SIZE]) {
    if (out == NULL || v == NULL) return;

    for (int i = 0; i < VEC_4_SIZE; ++i) {
        out[i] = v[i];
    }
}

void Vec4Fill(float out[VEC_4_SIZE], float value) {
    if (out == NULL) return;

    for (int i = 0; i < VEC_4_SIZE; ++i) {
        out[i] = value;
    }
}

void Vec4MakeWFirst(float out[VEC_4_SIZE], float v[VEC_4_SIZE]) {
    float temp[VEC_4_SIZE];
    Vec4Copy(temp, v);

    out[0] = temp[3];
    out[1] = temp[0];
    out[2] = temp[1];
    out[3] = temp[2];
}

void Vec4MakeWLast(float out[VEC_4_SIZE], float v[VEC_4_SIZE]) {
    float temp[VEC_4_SIZE];
    Vec4Copy(temp, v);

    out[0] = temp[1];
    out[1] = temp[2];
    out[2] = temp[3];
    out[3] = temp[0];
}
