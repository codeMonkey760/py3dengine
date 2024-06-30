#ifndef QUATERNION_H_
#define QUATERNION_H_

#define QUATERNION_SIZE 4
#include "vector.h"

extern void QuaternionIdentity(float out[QUATERNION_SIZE]);
extern void QuaternionCopy(float out[QUATERNION_SIZE], const float q[QUATERNION_SIZE]);
extern void QuaternionToCStr(char *out, const float q[QUATERNION_SIZE], ssize_t size);
extern void QuaternionVec3Rotation (float out[VEC_3_SIZE], const float v[VEC_3_SIZE], const float q[QUATERNION_SIZE]);
extern void QuaternionFromAxisAngle (float out[QUATERNION_SIZE], float x, float y, float z, float a);
extern void QuaternionNormalize(float out[QUATERNION_SIZE], const float q[QUATERNION_SIZE]);
extern void QuaternionMult(float out[QUATERNION_SIZE], const float q1[QUATERNION_SIZE], const float q2[QUATERNION_SIZE]);

#endif
