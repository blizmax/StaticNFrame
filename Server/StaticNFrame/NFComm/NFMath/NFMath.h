#pragma once

#include <string>
#include <algorithm>
#include <stdint.h>
#include <vector>
#include <limits>
#include "NFComm/NFCore/NFPlatform.h"

// windows include
#if NF_PLATFORM == NF_PLATFORM_WIN
#else
// linux include
#include <errno.h>
#endif

#include "G3D/g3dmath.h"
#include "G3D/Vector2.h"
#include "G3D/Vector3.h"
#include "G3D/Vector4.h"
#include "G3D/Matrix3.h"
#include "G3D/Quat.h"

typedef G3D::Matrix3 Matrix;
typedef G3D::Quat Quaternion;
typedef G3D::Vector2 Vector2;
typedef G3D::Vector3 Vector3;
typedef G3D::Vector4 Vector4;

#define NF_PI									3.1415926535898
#define NF_2PI									6.2831853071796
#define NF_DegreeToRadian						G3D::toRadians
#define NF_RadianToDegree						G3D::toRadians

#define NFVec2Length(v)							(v)->length()
#define NFVec2LengthSq(v)						(v)->squaredLength()
#define NFVec2Normalize(v, vv)					(v)->normalise()
#define NFVec2Dot(v, vv)						(v)->dot(static_cast<const G3D::Vector2 &>(*(vv)))
#define NFVec3Length(v)							(v)->length()
#define NFVec3LengthSq(v)						(v)->squaredLength()
#define NFVec3Dot(v, vv)						(v)->dot(static_cast<const G3D::Vector3 &>(*(vv)))
#define NFVec3Cross(v)							(v)->cross()
#define NFVec3Normalize(v, vv)					(v)->normalise()
#define NFVec3Lerp(v)							(v)->lerp()
#define NFVec3Transform(v)						D3DXVec3Transform
#define NFVec3TransformCoord(v)					D3DXVec3TransformCoord
#define NFVec3TransformNormal(v)				D3DXVec3TransformNormal
#define NFVec4Transform(v)						D3DXVec4Transform
#define NFVec4Length(v)							(v)->length()
#define NFVec4LengthSq(v)						(v)->squaredLength()
#define NFVec4Normalize(v, vv)					(v)->normalise()
#define NFVec4Lerp(v)							(v)->lerp()
#define NFVec4Dot(v, vv)						(v)->dot(static_cast<const G3D::Vector4 &>(*(vv)))
#define NFMatrixIdentity						Matrix3::identity()
#define NFMatrixInverse(v)						Matrix3::inverse()
#define NFMatrixRotationQuaternion(v)			D3DXMatrixRotationQuaternion
#define NFMatrixTranspose(v)					D3DXMatrixTranspose
#define NFMatrixfDeterminant(v)					D3DXMatrixDeterminant
#define NFMatrixOrthoLH(v)						D3DXMatrixOrthoLH
#define NFMatrixLookAtLH(v)						D3DXMatrixLookAtLH
#define NFMatrixMultiply(v)						D3DXMatrixMultiply
#define NFMatrixPerspectiveFovLH(v)				D3DXMatrixPerspectiveFovLH
#define NFMatrixRotationX(v)					D3DXMatrixRotationX
#define NFMatrixRotationY(v)					D3DXMatrixRotationY
#define NFMatrixRotationZ(v)					D3DXMatrixRotationZ
#define NFMatrixScaling(v)						D3DXMatrixScaling
#define NFMatrixTranslation(v)					D3DXMatrixTranslation
#define NFMatrixRotationYawPitchRoll(v)			D3DXMatrixRotationYawPitchRoll
#define NFQuaternionDot(v)						D3DXQuaternionDot
#define NFQuaternionNormalize(v)				D3DXQuaternionNormalize
#define NFQuaternionRotationMatrix(v)			D3DXQuaternionRotationMatrix
#define NFQuaternionSlerp(v)					D3DXQuaternionSlerp
#define NFQuaternionRotationAxis(v)				D3DXQuaternionRotationAxis
#define NFQuaternionMultiply(v)					D3DXQuaternionMultiply
#define NFQuaternionInverse(v)					D3DXQuaternionInverse
#define NFClamp									G3D::clamp

// 从2个3d向量忽略y计算出2d长度
inline float NFVec3CalcVec2Length(const Vector3& v1, const Vector3& v2)
{
	float x = v1.x - v2.x;
	float z = v1.z - v2.z;
	return sqrt(x * x + z * z);
}

inline float int82angle(int8_t angle, bool half = false)
{
	return float(angle) * float((NF_PI / (half ? 254.f : 128.f)));
}

inline int8_t angle2int8(float v, bool half = false)
{
	int8_t angle = 0;
	if (!half)
	{
		angle = static_cast<int8_t>(floorf((v * 128.f) / float(NF_PI) + 0.5f));
	}
	else
	{
		angle = static_cast<int8_t>(NFClamp(floorf((v * 254.f) / float(NF_PI) + 0.5f), -128.f, 127.f));
	}

	return angle;
}

typedef Vector2 NFVector2; // 表示3D位置变量类型
typedef Vector3 NFVector3; // 表示3D位置变量类型
typedef Vector4 NFVector4; // 表示3D位置变量类型

struct Direction3D // 表示方向位置变量类型
{
	Direction3D() : dir(0.f, 0.f, 0.f)
	{
	};

	Direction3D(const Vector3& v) : dir(v)
	{
	}

	Direction3D(float r, float p, float y) : dir(r, p, y)
	{
	}

	Direction3D(const Direction3D& v) : dir(v.dir)
	{
	}

	Direction3D& operator=(const Direction3D& v)
	{
		dir = v.dir;
		return *this;
	}

	float roll() const
	{
		return dir.x;
	}

	float pitch() const
	{
		return dir.y;
	}

	float yaw() const
	{
		return dir.z;
	}

	void roll(float v)
	{
		dir.x = v;
	}

	void pitch(float v)
	{
		dir.y = v;
	}

	void yaw(float v)
	{
		dir.z = v;
	}

	// roll, pitch, yaw
	Vector3 dir;
};

/** 浮点数比较 */
#define floatEqual(v1, v2) (abs(v1 - v2) < std::numeric_limits<float>::epsilon())

inline bool almostEqual(const float f1, const float f2, const float epsilon = 0.0004f)
{
	return fabsf(f1 - f2) < epsilon;
}

inline bool almostEqual(const double d1, const double d2, const double epsilon = 0.0004)
{
	return fabs(d1 - d2) < epsilon;
}

inline bool almostZero(const float f, const float epsilon = 0.0004f)
{
	return f < epsilon && f > -epsilon;
}

inline bool almostZero(const double d, const double epsilon = 0.0004)
{
	return d < epsilon && d > -epsilon;
}

template <typename T>
inline bool almostEqual(const T& c1, const T& c2, const float epsilon = 0.0004f)
{
	if (c1.size() != c2.size())
		return false;
	typename T::const_iterator iter1 = c1.begin();
	typename T::const_iterator iter2 = c2.begin();
	for (; iter1 != c1.end(); ++iter1 , ++iter2)
		if (!almostEqual(*iter1, *iter2, epsilon))
			return false;
	return true;
}

