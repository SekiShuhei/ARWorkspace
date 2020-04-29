#pragma once

#include "Vector3.hpp"

namespace ARWorkspace {
class SensorVectorIntegral
{
public:
	void SetData(const double& x, const double& y, const double& z)
	{
		this->SetData(Vector3(x, y, z));
	}
	void SetData(const Vector3 arg)
	{
		this->input = arg;
		this->update_value();
	}
	const Vector3& GetInput() const
	{
		return this->input;
	}
	const Vector3& GetIntegral() const
	{
		return this->integral;
	}
public:
private:
	Vector3	input;
	Vector3	integral;
private:
	void update_value()
	{
		this->integral += this->input;
	}
private:
};

}