#pragma once

#include "Vector3.hpp"

namespace ARWorkspace {

class SensorVectorRelative
{
public:

	void ResetBase()
	{
		this->base = this->input;
		this->update_value();
	}

	void SetData(const double& x, const double& y, const double& z)
	{
		this->SetData(Vector3(x, y, z));
	}
	void SetData(const Vector3 arg)
	{
		this->input = arg;
		if (! this->base_initialized)
		{
			this->base_initialized = true;
			this->base = arg;
		}
		this->update_value();
	}
	void SetBaseData(const Vector3 arg)
	{
		this->base = arg;
	}

	const Vector3& GetInput() const
	{
		return this->input;
	}
	const Vector3& GetBase() const
	{
		return this->base;
	}
	const Vector3& GetRelative() const
	{
		return this->relative;
	}
public:
//private:
	Vector3	input;
	Vector3	base;
	Vector3	relative;
private:
	void update_value()
	{
		this->relative = this->input - this->base;
	}
private:
	bool	base_initialized = false;
};

}