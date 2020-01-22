#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <cmath>
#include <limits>

constexpr const float INF = std::numeric_limits<float>::infinity();

class Vector3D
{
public:
	// XYZ or RGB
	union {
		struct {
			union { float x; float r; };
			union { float y; float g; };
			union { float z; float b; };
		};
		float elements[3];
	};

	Vector3D()							: x(0), y(0), z(0) {}
	Vector3D(float f				  ) : x(f), y(f), z(f) {}
	Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

	inline Vector3D operator+(const Vector3D& f) const
	{
		return Vector3D(x + f.x, y + f.y, z + f.z);
	}

	inline Vector3D& operator+=(const Vector3D& f)
	{
		x += f.x; y += f.y; z += f.z;
		return *this;
	}

	inline Vector3D operator-(const Vector3D& f) const
	{
		return Vector3D(x - f.x, y - f.y, z - f.z);
	}

	inline Vector3D operator*(const Vector3D& f) const
	{
		return Vector3D(x * f.x, y * f.y, z * f.z);
	}

	inline Vector3D operator*(float f) const
	{
		return Vector3D(x * f, y * f, z * f);
	}

	inline Vector3D& operator*=(float f)
	{
		x *= f; y *= f; z *= f;
		return *this;
	}

	inline Vector3D& operator*=(const Vector3D& f)
	{
		x *= f.x; y *= f.y; z *= f.z;
		return *this;
	}
	
	inline Vector3D operator/(float f) const
	{
		return Vector3D(x / f, y / f, z / f);
	}

	inline Vector3D& operator/=(float f)
	{
		x /= f; y /= f; z /= f;
		return *this;
	}

	inline float Dot(const Vector3D& f) const
	{
		return x * f.x + y * f.y + z * f.z;
	}

	inline Vector3D Cross(const Vector3D& f) const
	{
		return Vector3D(
			y * f.z - z * f.y,
			z * f.x - x * f.z,
			x * f.y - y * f.x
		);
	}

	inline float Distance(const Vector3D& f) const
	{
		return sqrt(
			(x - f.x) * (x - f.x) +
			(y - f.y) * (y - f.y) +
			(z - f.z) * (z - f.z) );
	}

	inline Vector3D Normalized() const
	{
		float length = this->Dot(*this);
		if (length == 0) return *this;

		return *this / sqrt(length);
	}

	inline bool IsZero() const
	{
		return x == 0 && y == 0 && z == 0;
	}

	inline Vector3D Reciprocal() const
	{
		return Vector3D(1 / x, 1 / y, 1 / z);
	}

private:
	inline float fast_inv_sqrt(float x) const
	{
		float xhalf = 0.5f * x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1);
		x = *(float*)&i;
		x = x * (1.5f - xhalf * x * x);
		return x;
	}

	inline double inv_sqrt_double(double number) const
	{
		double y = number;
		double x2 = y * 0.5;
		long i = *(long*) & y;
		i = 0x5fe6eb50c7b537a9 - (i >> 1);
		y = *(double*)&i;
		y = y * (1.5 - (x2 * y * y));
		return y;
	}
};

#endif