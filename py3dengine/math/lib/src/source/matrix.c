#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"

void Mat3Identity(float out[MAT_3_SIZE]) {
    if (out == NULL) return;

    for (int i = 0; i < MAT_3_SIZE; ++i) {
        out[i] = (i % 4 == 0) ? 1.0f : 0.0f;
    }
}

void Mat4Identity(float out[MAT_4_SIZE]) {
    if (out == NULL) return;

    for (int i = 0; i < MAT_4_SIZE; ++i) {
        out[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void Mat4Copy(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]) {
    if (out == NULL || m == NULL) return;

    for (int i = 0; i < MAT_4_SIZE; ++i) {
        out[i] = m[i];
    }
}

void Mat4Fill(float out[MAT_4_SIZE], float value) {
    if (out == NULL) return;

    for (int i = 0; i < MAT_4_SIZE; ++i) {
        out[i] = value;
    }
}

void Mat4Transpose(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]) {
    if (out == NULL || m == NULL) return;

    float temp[MAT_4_SIZE] = {0.0f};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[i * 4 + j] = m[j * 4 + i];
        }
    }

    Mat4Copy(out, temp);
}

void Mat4ToCStr(char *out, const float m[MAT_4_SIZE], ssize_t size) {
    if (out == NULL || m == NULL || size <= 1) return;

    memset(out, 0, sizeof(char) * size);

    snprintf(
        out,
        size-1,
        "Matrix4x4(\n"
        "    %.2f, %.2f, %.2f, %.2f\n"
        "    %.2f, %.2f, %.2f, %.2f\n"
        "    %.2f, %.2f, %.2f, %.2f\n"
        "    %.2f, %.2f, %.2f, %.2f\n"
        ")\n",
        m[ 0], m[ 1], m[ 2], m[ 3],
        m[ 4], m[ 5], m[ 6], m[ 7],
        m[ 8], m[ 9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    );
}

void Mat4TranslationF(float out[MAT_4_SIZE], float x, float y, float z) {
    float v[VEC_3_SIZE] = {x, y, z};

    Mat4TranslationFA(out, v);
}

void Mat4TranslationFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE]) {
    if (out == NULL || v == NULL) return;

    Mat4Identity(out);
    out[12] = v[0];
    out[13] = v[1];
    out[14] = v[2];
}

void Mat4RotationX(float out[MAT_4_SIZE], float theta) {
    if (out == NULL) return;

    theta = DEG_TO_RAD(theta);

    float st = sinf(theta);
    float ct = cosf(theta);

    Mat4Identity(out);
    out[5] = ct;
    out[6] = st;
    out[9] = -st;
    out[10] = ct;
}

void Mat4RotationY(float out[MAT_4_SIZE], float theta) {
    if (out == NULL) return;

    theta = DEG_TO_RAD(theta);

    float ct = cosf(theta);
    float st = sinf(theta);

    Mat4Identity(out);
    out[0] = ct;
    out[2] = -st;
    out[8] = st;
    out[10] = ct;
}

void Mat4RotationZ(float out[MAT_4_SIZE], float theta) {
    if (out == NULL) return;

    theta = DEG_TO_RAD(theta);

    float ct = cosf(theta);
    float st = sinf(theta);

    Mat4Identity(out);
    out[0] = ct;
    out[1] = st;
    out[4] = -st;
    out[5] = ct;
}

void Mat4RotationAxisF(float out[MAT_4_SIZE], float x, float y, float z, float theta) {
    float v[VEC_3_SIZE] = {x, y, z};

    Mat4RotationAxisFA(out, v, theta);
}

void Mat4RotationAxisFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE], float theta) {
    if (out == NULL || v == NULL) return;

    theta = DEG_TO_RAD(theta);

    float x = v[0];
    float y = v[1];
    float z = v[2];
    float len = sqrtf(x * x + y * y + z * z);
    x /= len;
    y /= len;
    z /= len;

    float c = cosf(theta);
    float s = sinf(theta);
    float nc = 1 - c;
    float xy = x * y;
    float yz = y * z;
    float zx = z * x;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    Mat4Identity(out);
    out[0] = x * x * nc + c;
    out[1] = xy * nc + zs;
    out[2] = zx * nc - ys;

    out[4] = xy * nc - zs;
    out[5] = y * y * nc + c;
    out[6] = yz * nc + xs;

    out[8] = zx * nc + ys;
    out[9] = yz * nc - xs;
    out[10] = z * z * nc + c;
}

void Mat4RotationQuaternionF(float out[MAT_4_SIZE], float x, float y, float z, float w) {
    float q[4] = {x, y, z, w};

    Mat4RotationQuaternionFA(out, q);
}

void Mat4RotationQuaternionFA(float out[MAT_4_SIZE], const float q[QUATERNION_SIZE]) {
    if (out == NULL || q == NULL) return;

    float x = q[0];
    float y = q[1];
    float z = q[2];
    float w = q[3];

    float n = (x * x) + (y * y) + (z * z) + (w * w);
    float s = (n == 0.0f) ? 0.0f : 2.0f / n;
    float wx = s * w * x;
    float wy = s * w * y;
    float wz = s * w * z;
    float xx = s * x * x;
    float xy = s * x * y;
    float xz = s * x * z;
    float yy = s * y * y;
    float yz = s * y * z;
    float zz = s * z * z;

    Mat4Identity(out);

    out[0] = 1.0f - (yy + zz);
    out[4] = xy - wz;
    out[8] = xz + wy;
    out[1] = xy + wz;
    out[5] = 1.0f - (xx + zz);
    out[9] = yz - wx;
    out[2] = xz - wy;
    out[6] = yz + wx;
    out[10] = 1.0f - (xx + yy);
}

void Mat4ScalingF(float out[MAT_4_SIZE], float x, float y, float z) {
    if (out == NULL) return;

    Mat4Identity(out);
    out[0] = x;
    out[5] = y;
    out[10] = z;
}

void Mat4ScalingFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE]) {
    if (out == NULL || v == NULL) return;

    Mat4Identity(out);
    out[0] = v[0];
    out[5] = v[1];
    out[10] = v[2];
}

// non gsl version
void Mat4Mult(float out[MAT_4_SIZE], const float m1[MAT_4_SIZE], const float m2[MAT_4_SIZE]) {
    float temp[MAT_4_SIZE] = {0.0f};

    if (out == NULL || m1 == NULL || m2 == NULL) return;

    temp[0] = (m1[0] * m2[0]) + (m1[1] * m2[4]) + (m1[2] * m2[8]) + (m1[3] * m2[12]);
    temp[1] = (m1[0] * m2[1]) + (m1[1] * m2[5]) + (m1[2] * m2[9]) + (m1[3] * m2[13]);
    temp[2] = (m1[0] * m2[2]) + (m1[1] * m2[6]) + (m1[2] * m2[10]) + (m1[3] * m2[14]);
    temp[3] = (m1[0] * m2[3]) + (m1[1] * m2[7]) + (m1[2] * m2[11]) + (m1[3] * m2[15]);

    temp[4] = (m1[4] * m2[0]) + (m1[5] * m2[4]) + (m1[6] * m2[8]) + (m1[7] * m2[12]);
    temp[5] = (m1[4] * m2[1]) + (m1[5] * m2[5]) + (m1[6] * m2[9]) + (m1[7] * m2[13]);
    temp[6] = (m1[4] * m2[2]) + (m1[5] * m2[6]) + (m1[6] * m2[10]) + (m1[7] * m2[14]);
    temp[7] = (m1[4] * m2[3]) + (m1[5] * m2[7]) + (m1[6] * m2[11]) + (m1[7] * m2[15]);

    temp[8] = (m1[8] * m2[0]) + (m1[9] * m2[4]) + (m1[10] * m2[8]) + (m1[11] * m2[12]);
    temp[9] = (m1[8] * m2[1]) + (m1[9] * m2[5]) + (m1[10] * m2[9]) + (m1[11] * m2[13]);
    temp[10] = (m1[8] * m2[2]) + (m1[9] * m2[6]) + (m1[10] * m2[10]) + (m1[11] * m2[14]);
    temp[11] = (m1[8] * m2[3]) + (m1[9] * m2[7]) + (m1[10] * m2[11]) + (m1[11] * m2[15]);

    temp[12] = (m1[12] * m2[0]) + (m1[13] * m2[4]) + (m1[14] * m2[8]) + (m1[15] * m2[12]);
    temp[13] = (m1[12] * m2[1]) + (m1[13] * m2[5]) + (m1[14] * m2[9]) + (m1[15] * m2[13]);
    temp[14] = (m1[12] * m2[2]) + (m1[13] * m2[6]) + (m1[14] * m2[10]) + (m1[15] * m2[14]);
    temp[15] = (m1[12] * m2[3]) + (m1[13] * m2[7]) + (m1[14] * m2[11]) + (m1[15] * m2[15]);

    Mat4Copy(out, temp);
}

// lifted this from:
//http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
// seems to be a copy of bool gluInvertMatrix(const double m[16], double invOut[16])
int Mat4Inverse(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]) {
    float inv[MAT_4_SIZE], det;
    Mat4Fill(inv, 0.0f);

    inv[0] = m[5] * m[10] * m[15] -
             m[5] * m[11] * m[14] -
             m[9] * m[6] * m[15] +
             m[9] * m[7] * m[14] +
             m[13] * m[6] * m[11] -
             m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
             m[4] * m[11] * m[14] +
             m[8] * m[6] * m[15] -
             m[8] * m[7] * m[14] -
             m[12] * m[6] * m[11] +
             m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
             m[4] * m[11] * m[13] -
             m[8] * m[5] * m[15] +
             m[8] * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
              m[4] * m[10] * m[13] +
              m[8] * m[5] * m[14] -
              m[8] * m[6] * m[13] -
              m[12] * m[5] * m[10] +
              m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
             m[1] * m[11] * m[14] +
             m[9] * m[2] * m[15] -
             m[9] * m[3] * m[14] -
             m[13] * m[2] * m[11] +
             m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
             m[0] * m[11] * m[14] -
             m[8] * m[2] * m[15] +
             m[8] * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
             m[0] * m[11] * m[13] +
             m[8] * m[1] * m[15] -
             m[8] * m[3] * m[13] -
             m[12] * m[1] * m[11] +
             m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
              m[0] * m[10] * m[13] -
              m[8] * m[1] * m[14] +
              m[8] * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
             m[1] * m[7] * m[14] -
             m[5] * m[2] * m[15] +
             m[5] * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
             m[0] * m[7] * m[14] +
             m[4] * m[2] * m[15] -
             m[4] * m[3] * m[14] -
             m[12] * m[2] * m[7] +
             m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
              m[0] * m[7] * m[13] -
              m[4] * m[1] * m[15] +
              m[4] * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
              m[0] * m[6] * m[13] +
              m[4] * m[1] * m[14] -
              m[4] * m[2] * m[13] -
              m[12] * m[1] * m[6] +
              m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
             m[1] * m[7] * m[10] +
             m[5] * m[2] * m[11] -
             m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7] +
             m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
              m[0] * m[7] * m[9] +
              m[4] * m[1] * m[11] -
              m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] +
              m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0) return 0;

    det = 1.0f / det;

    for (int i = 0; i < MAT_4_SIZE; i++) {
        out[i] = inv[i] * det;
    }

    return 1;
}

void Mat4Vec4Mult(float out[VEC_4_SIZE], const float m[MAT_4_SIZE], const float v[VEC_4_SIZE]) {
    if (out == NULL || m == NULL || v == NULL) return;

    float temp[4] = {0.0f};
    temp[0] = (m[0] * v[0]) + (m[4] * v[1]) + (m[8] * v[2]) + (m[12] * v[3]);
    temp[1] = (m[1] * v[0]) + (m[5] * v[1]) + (m[9] * v[2]) + (m[13] * v[3]);
    temp[2] = (m[2] * v[0]) + (m[6] * v[1]) + (m[10] * v[2]) + (m[14] * v[3]);
    temp[3] = (m[3] * v[0]) + (m[7] * v[1]) + (m[11] * v[2]) + (m[15] * v[3]);

    out[0] = temp[0];
    out[1] = temp[1];
    out[2] = temp[2];
    out[3] = temp[3];
}

void Mat4LookAtLH(
    float out[MAT_4_SIZE],
    const float camPosW[VEC_3_SIZE],
    const float camTargetW[VEC_3_SIZE],
    const float camUpW[VEC_3_SIZE]
) {
    if (
        out == NULL ||
        camPosW == NULL ||
        camTargetW == NULL ||
        camUpW == NULL
    ) return;

    float look[3] = {0.0f};
    float up[3] = {0.0f};
    float right[3] = {0.0f};
    float negCamPosW[3] = {0.0f};

    Mat4Identity(out);

    Vec3Subtract(look, camTargetW, camPosW);
    Vec3Normalize(look, look);

    Vec3Cross(right, camUpW, look);
    Vec3Normalize(right, right);

    Vec3Cross(up, look, right);
    Vec3Normalize(up, up);

    Vec3Scalar(negCamPosW, camPosW, -1.0f);

    out[0] = right[0];
    out[1] = up[0];
    out[2] = look[0];
    out[3] = 0.0f;

    out[4] = right[1];
    out[5] = up[1];
    out[6] = look[1];
    out[7] = 0.0f;

    out[8] = right[2];
    out[9] = up[2];
    out[10] = look[2];
    out[11] = 0.0f;

    Vec3Dot(&out[12], negCamPosW, right);
    Vec3Dot(&out[13], negCamPosW, up);
    Vec3Dot(&out[14], negCamPosW, look);
    out[15] = 1.0f;
}
