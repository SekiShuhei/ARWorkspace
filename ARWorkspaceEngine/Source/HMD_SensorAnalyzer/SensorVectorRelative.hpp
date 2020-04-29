#pragma once

#include "Vector3.hpp"

namespace ARWorkspace {

class SensorVectorRelative
{
public:
	void SetData(const Vector3 arg)
	{
		this->input = arg;
		if (! this->base_initialized)
		{
			this->base = arg;
		}
		this->update_value();
	}
	void SetBaseData(const Vector3 arg)
	{
		this->base = arg;
	}

	const Vector3& GetRelative()
	{
		return this->relative;
	}
public:
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