from py3dengine import math as M

RIGHT = M.Vector3(1, 0, 0)
UP = M.Vector3(0, 1, 0)
FORWARD = M.Vector3(0, 0, 1)

print('Rotate 0,0,1 90 degrees around X axis = 0,-1,0')
v1 = FORWARD
q1 = M.Quaternion.FromAxisAndDegrees(RIGHT, 90)
m1 = M.Matrix4x4.RotationX(90)
m2 = M.Matrix4x4.RotationAxis(RIGHT, 90)

print('v1 is {}, q1 is quaternion, m1 is RotationX, m2 is RotationAxis'.format(str(v1)))
print('v1 * q1:', v1 * q1)
print('v1 * m1:', v1 * m1)
print('v1 * m2:', v1 * m2)

print('Rotate 1,0,0 90 degrees around Y axis = 0,0,-1')
v1 = RIGHT
q1 = M.Quaternion.FromAxisAndDegrees(UP, 90)
m1 = M.Matrix4x4.RotationY(90)
m2 = M.Matrix4x4.RotationAxis(UP, 90)

print('v1 is {}, q1 is quaternion, m1 is RotationX, m2 is RotationAxis'.format(str(v1)))
print('v1 * q1:', v1 * q1)
print('v1 * m1:', v1 * m1)
print('v1 * m2:', v1 * m2)

print('Rotate 0,1,0 90 degrees around Z axis = -1,0,0')
v1 = UP
q1 = M.Quaternion.FromAxisAndDegrees(FORWARD, 90)
m1 = M.Matrix4x4.RotationZ(90)
m2 = M.Matrix4x4.RotationAxis(FORWARD, 90)

print('v1 is {}, q1 is quaternion, m1 is RotationX, m2 is RotationAxis'.format(str(v1)))
print('v1 * q1:', v1 * q1)
print('v1 * m1:', v1 * m1)
print('v1 * m2:', v1 * m2)
