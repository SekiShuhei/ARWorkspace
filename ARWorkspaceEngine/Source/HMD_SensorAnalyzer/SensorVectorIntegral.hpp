#pragma once

#include "Vector3.hpp"

namespace ARWorkspace {
class SensorVectorIntegral
{
public:
	void SetData(const double& x, const double& y, const double& z, double delta_t)
	{
		this->SetData(Vector3(x, y, z), delta_t);
	}
	void SetData(const Vector3 arg, double delta_t)
	{
		this->input = arg;
		this->integral += (this->input * delta_t);
	}
	const Vector3& GetInput() const
	{
		return this->input;
	}
	const Vector3& GetIntegral() const
	{
		return this->integral;
	}
	void ResetIntegral()
	{
		this->integral = Vector3();
	}
public:
//private:
	Vector3	input;
	Vector3	integral;
private:
	
private:
};

}