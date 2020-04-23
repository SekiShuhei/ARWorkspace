#pragma once

#include "SensorManagerDefine.hpp"

namespace WinSensor {
class HRESULT_Support
{
public:
	inline bool IsError() const
	{
		return FAILED(this->result);
	}
	inline HRESULT GetResult() const
	{
		return this->result;
	}
protected:
	HRESULT	result = S_OK;
};

}