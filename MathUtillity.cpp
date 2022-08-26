#include "MathUtility.h"

Vector3 Transform(const Vector3& v, const Matrix4& m)
{
    Vector3 resultVec;

    resultVec.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
    resultVec.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
    resultVec.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);

    return resultVec;
}