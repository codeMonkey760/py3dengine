#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "quaternion.h"

void QuaternionIdentity(float out[QUATERNION_SIZE]) {
    if (out == NULL) return;

    for (int i = 0; i < QUATERNION_SIZE-1; ++i) {
        out[i] = 0.0f;
    }
    out[QUATERNION_SIZE-1] = 1.0f;
}

void QuaternionCopy(float out[QUATERNION_SIZE], const float q[QUATERNION_SIZE]) {
    if (out == NULL || q == NULL) return;

    for (int i = 0; i < QUATERNION_SIZE; ++i) {
        out[i] = q[i];
    }
}

void QuaternionToCStr(char *out, const float q[QUATERNION_SIZE], ssize_t size) {
    if (out == NULL || q == NULL || size <= 1) return;

    memset(out, 0, sizeof(char) * size);

    snprintf(out, size-1, "Quaternion(%.2f, %.2f, %.2f, %.2f)", q[0], q[1], q[2], q[3]);
}

void QuaternionVec3Rotation(float out[VEC_3_SIZE], const float v[VEC_3_SIZE], const float q[QUATERNION_SIZE]) {
    if (out == NULL || v == NULL || q == NULL) return;

    float temp[VEC_3_SIZE] = {0.0f};
    float temp2[VEC_3_SIZE] = {0.0f};
    float temp3[VEC_3_SIZE] = {0.0f};

    float q_dot_v = 0.0f;
    Vec3Dot(&q_dot_v, q, v);
    float q_dot_q = 0.0f;
    Vec3Dot(&q_dot_q, q, q);

    Vec3Scalar(temp, q, (2.0f * q_dot_v));
    Vec3Scalar(temp2, v, ((q[3] * q[3]) - q_dot_q));
    Vec3Cross(temp3, q, v);
    Vec3Scalar(temp3, temp3, 2.0f * q[3]);

    for (int i = 0; i < VEC_3_SIZE; ++i) {
        out[i] = temp[i] + temp2[i] + temp3[i];
    }
}

// found this at:
// http://stackoverflow.com/questions/4436764/rotating-a-quaternion-on-1-axis
void QuaternionFromAxisAngle(float out[QUATERNION_SIZE], float x, float y, float z, float a) {
    if (out == NULL) return;

    a = DEG_TO_RAD(a);

    float fac = sinf(a / 2.0f);

    out[0] = x * fac;
    out[1] = y * fac;
    out[2] = z * fac;

    out[3] = cosf(a / 2.0f);

    QuaternionNormalize(out, out);
}

void QuaternionNormalize(float out[QUATERNION_SIZE], const float q[QUATERNION_SIZE]) {
    if (out == NULL) return;

    float len = sqrtf((out[0] * out[0]) + (out[1] * out[1]) + (out[2] * out[2]) + (out[3] * out[3]));
    out[0] = q[0] / len;
    out[1] = q[1] / len;
    out[2] = q[2] / len;
    out[3] = q[3] / len;
}

/*
 * http://www.cprogramming.com/tutorial/3d/quaternions.html
 Let Q1 and Q2 be two quaternions, which are defined, respectively, as (w1, x1, y1, z1) and (w2, x2, y2, z2).
(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2)
(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
*/
void QuaternionMult(float out[QUATERNION_SIZE], const float q1[QUATERNION_SIZE], const float q2[QUATERNION_SIZE]) {
    if (out == NULL || q1 == NULL || q2 == NULL) return;

    float temp[4] = {0.0f};
    temp[0] = (q1[3] * q2[0]) + (q1[0] * q2[3]) + (q1[1] * q2[2]) - (q1[2] * q2[1]);
    temp[1] = (q1[3] * q2[1]) - (q1[0] * q2[2]) + (q1[1] * q2[3]) + (q1[2] * q2[0]);
    temp[2] = (q1[3] * q2[2]) + (q1[0] * q2[1]) - (q1[1] * q2[0]) + (q1[2] * q2[3]);
    temp[3] = (q1[3] * q2[3]) - (q1[0] * q2[0]) - (q1[1] * q2[1]) - (q1[2] * q2[2]);

    QuaternionCopy(out, temp);
}
