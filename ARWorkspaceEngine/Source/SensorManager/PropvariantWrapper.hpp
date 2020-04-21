#pragma once

#pragma once

#include "SensorManagerDefine.hpp"

namespace WinSensor {
class PropvariantWrapper
{
private:
	PropvariantWrapper(const PropvariantWrapper&)				= delete;
	PropvariantWrapper& operator=(const PropvariantWrapper&)	= delete;
	PropvariantWrapper(PropvariantWrapper&&)					= delete;
	PropvariantWrapper& operator=(PropvariantWrapper&&)			= delete;
protected:
	PropvariantWrapper()
	{
		PropVariantInit(&this->pv_data);
	}
public:
	~PropvariantWrapper()
	{
		PropVariantClear(&this->pv_data);
	}
	inline bool IsError() const
	{
		return FAILED(this->result);
	}
	inline HRESULT GetResult() const
	{
		return this->result;
	}
	inline const PROPVARIANT& GetPropvariant() const
	{
		return this->pv_data;
	}
protected:
	HRESULT		result = S_OK;
	PROPVARIANT	pv_data;
};

}