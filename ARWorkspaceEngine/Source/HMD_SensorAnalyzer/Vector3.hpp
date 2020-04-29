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
	bool operator == (const Vector3& arg)
	{
		return (this->x == arg.x && this->y == arg.y && this->z == arg.z);
	}
	Vector3 operator + (const Vector3& arg)
	{
		return Vector3(this->x + arg.x, this->y + arg.y, this->z + arg.z);
	}
	Vector3 operator - (const Vector3& arg)
	{
		return Vector3(this->x - arg.x, this->y - arg.y, this->z - arg.z);
	}



public:
	inline static bool IsRange(double base, double target, double range)
	{
		return (base > target - range && base < target + range);
	}
	inline bool IsRange(const Vector3& target, const double range)
	{
		return (
			Vector3::IsRange(this->x, target.x, range) &&
			Vector3::IsRange(this->y, target.y, range) &&
			Vector3::IsRange(this->z, target.z, range));
	}
	inline static bool Smoothing(double& base, double target, double ratio)
	{
		if (ratio <= 0.0)
		{
			return false;
		}
		ratio = (ratio > 1.0 ? 1.0 : ratio);
		base += ((target - base) * ratio);
		return true;
	}
	inline static bool Smoothing(Vector3& base, double target, double ratio)
	{
		if (ratio <= 0.0)
		{
			return false;
		}
		Smoothing(base.x, target, ratio);
		Smoothing(base.y, target, ratio);
		Smoothing(base.z, target, ratio);
	}
public:
public:
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

};

}