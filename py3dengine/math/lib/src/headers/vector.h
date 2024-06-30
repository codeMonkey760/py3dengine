#ifndef VECTOR_H_
#define VECTOR_H_

#define VEC_2_SIZE 2
#define VEC_3_SIZE 3
#define VEC_4_SIZE 4

/**
 * Math operations for vectors with 2 and 3 dimensions.
 */

/** Vector 2 */
extern void Vec2Identity(float out[VEC_2_SIZE]);
extern void Vec2Copy(float out[VEC_2_SIZE], const float v[VEC_2_SIZE]);
extern void Vec2Fill(float out[VEC_2_SIZE], float value);

extern void Vec2Add(float out[VEC_2_SIZE], const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]);
extern void Vec2Subtract(float out[VEC_2_SIZE], const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]);
extern void Vec2Scalar(float out[VEC_2_SIZE], const float v[VEC_2_SIZE], float scalar);
extern void Vec2Dot(float *out, const float v1[VEC_2_SIZE], const float v2[VEC_2_SIZE]);

extern void Vec2Normalize(float out[VEC_2_SIZE], const float v[VEC_2_SIZE]);

/** Vector 3 */
extern void Vec3Identity(float out[VEC_3_SIZE]);
extern void Vec3Copy(float out[VEC_3_SIZE], const float v[VEC_3_SIZE]);
extern void Vec3Fill(float out[VEC_3_SIZE], float value);
extern void Vec3ToCStr(char *out, const float v[VEC_3_SIZE], ssize_t size);

extern void Vec3Add(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]);
extern void Vec3Subtract(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]);
extern void Vec3Scalar(float out[VEC_3_SIZE], const float v[VEC_3_SIZE], float scalar);
extern void Vec3Dot(float *out, const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]);
extern void Vec3Cross(float out[VEC_3_SIZE], const float v1[VEC_3_SIZE], const float v2[VEC_3_SIZE]);
extern void Vec3Divide(float out[VEC_3_SIZE], const float v[VEC_3_SIZE], float scalar);

extern void Vec3Length(float *out, const float v[VEC_3_SIZE]);
extern void Vec3Normalize(float out[VEC_3_SIZE], const float v[VEC_3_SIZE]);

/** Vector4 */
extern void Vec4Identity(float out[VEC_4_SIZE]);
extern void Vec4Copy(float out[VEC_4_SIZE], const float v[VEC_4_SIZE]);
extern void Vec4Fill(float out[VEC_4_SIZE], float value);
extern void Vec4MakeWFirst(float out[VEC_4_SIZE], float v[VEC_4_SIZE]);
extern void Vec4MakeWLast(float out[VEC_4_SIZE], float v[VEC_4_SIZE]);

#endif