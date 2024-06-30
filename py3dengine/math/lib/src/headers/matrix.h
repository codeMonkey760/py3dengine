#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include "quaternion.h"

#define MAT_3_SIZE 9
#define MAT_4_SIZE 16

extern void Mat3Identity(float out[MAT_3_SIZE]);

extern void Mat4Identity(float out[MAT_4_SIZE]);
extern void Mat4Copy(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]);
extern void Mat4Fill(float out[MAT_4_SIZE], float value);
extern void Mat4Transpose(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]);
extern void Mat4ToCStr(char *out, const float m[MAT_4_SIZE], ssize_t size);

extern void Mat4TranslationF(float out[MAT_4_SIZE], float x, float y, float z);
extern void Mat4TranslationFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE]);
extern void Mat4RotationX(float out[MAT_4_SIZE], float theta);
extern void Mat4RotationY(float out[MAT_4_SIZE], float theta);
extern void Mat4RotationZ(float out[MAT_4_SIZE], float theta);
extern void Mat4RotationAxisF(float out[MAT_4_SIZE], float x, float y, float z, float theta);
extern void Mat4RotationAxisFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE], float theta);
extern void Mat4RotationQuaternionF(float out[MAT_4_SIZE], float x, float y, float z, float w);
extern void Mat4RotationQuaternionFA(float out[MAT_4_SIZE], const float q[QUATERNION_SIZE]);
extern void Mat4ScalingF(float out[MAT_4_SIZE], float x, float y, float z);
extern void Mat4ScalingFA(float out[MAT_4_SIZE], const float v[VEC_3_SIZE]);

extern void Mat4Mult(float out[MAT_4_SIZE], const float m1[MAT_4_SIZE], const float m2[MAT_4_SIZE]);
extern int Mat4Inverse(float out[MAT_4_SIZE], const float m[MAT_4_SIZE]);
extern void Mat4Vec4Mult(float out[VEC_4_SIZE], const float m[MAT_4_SIZE], const float v[VEC_4_SIZE]);

extern void Mat4LookAtLH(float out[MAT_4_SIZE], const float camPosW[VEC_3_SIZE], const float camTargetW[VEC_3_SIZE], const float camUpW[VEC_3_SIZE]);

#endif
