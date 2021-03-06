#pragma once

namespace ARWorkspace {

struct Vector3
{
public:
	Vector3()
	{

	}

	Vector3(double arg_x, double arg_y, double arg_z) :
		x(arg_x), y(arg_y), z(arg_z)
	{

	}
public:
	Vector3& operator = (const Vector3& arg)
	{
		this->x = arg.x;
		this->y = arg.y;
		this->z = arg.z;
		return *this;
	}
	bool operator == (const Vector3& arg) const
	{
		return (this->x == arg.x && this->y == arg.y && this->z == arg.z);
	}
	Vector3 operator + (const Vector3& arg) const
	{
		return Vector3(this->x + arg.x, this->y + arg.y, this->z + arg.z);
	}
	Vector3 operator - (const Vector3& arg) const
	{
		return Vector3(this->x - arg.x, this->y - arg.y, this->z - arg.z);
	}
	Vector3 operator - (const double& arg) const
	{
		return Vector3(this->x - arg, this->y - arg, this->z - arg);
	}
	Vector3& operator += (const Vector3& arg)
	{
		this->x += arg.x;
		this->y += arg.y;
		this->z += arg.z;
		return *this;
	}
	Vector3& operator -= (const Vector3& arg)
	{
		this->x -= arg.x;
		this->y -= arg.y;
		this->z -= arg.z;
		return *this;
	}
	Vector3 operator * (const Vector3& arg) const
	{
		return Vector3(this->x * arg.x, this->y * arg.y, this->z * arg.z);
	}
	Vector3 operator * (const double& arg) const
	{
		return Vector3(this->x * arg, this->y * arg, this->z * arg);
	}
	Vector3 operator / (const Vector3& arg) const
	{
		return Vector3(this->x / arg.x, this->y / arg.y, this->z / arg.z);
	}

public:
	constexpr double Dot(const Vector3& v) const noexcept
	{
		return x * v.x + y * v.y + z * v.z;
	}

public:
	inline static bool IsRange(double base, double target, double range)
	{
		return (base > target - range && base < target + range);
	}
	inline bool IsRange(const double& target, const double range) const
	{
		return (
			Vector3::IsRange(this->x, target, range) &&
			Vector3::IsRange(this->y, target, range) &&
			Vector3::IsRange(this->z, target, range));
	}
	inline bool IsRange(const Vector3& target, const double range) const
	{
		return (
			Vector3::IsRange(this->x, target.x, range) &&
			Vector3::IsRange(this->y, target.y, range) &&
			Vector3::IsRange(this->z, target.z, range));
	}
	inline static void Smoothing(double& base, const double& target, double ratio)
	{
		if (ratio <= 0.0)
		{
			return;
		}
		ratio = (ratio > 1.0 ? 1.0 : ratio);
		base -= ((base - target) * ratio);
	}
	inline bool Smoothing(const Vector3& target, const double ratio)
	{
		if (ratio <= 0.0)
		{
			return false;
		}
		Vector3::Smoothing(this->x, target.x, ratio);
		Vector3::Smoothing(this->y, target.y, ratio);
		Vector3::Smoothing(this->z, target.z, ratio);
	}
	inline void Smoothing(const double& target, const double ratio)
	{
		if (ratio <= 0.0)
		{
			return;
		}
		Vector3::Smoothing(this->x, target, ratio);
		Vector3::Smoothing(this->y, target, ratio);
		Vector3::Smoothing(this->z, target, ratio);
	}
public:
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

};

}