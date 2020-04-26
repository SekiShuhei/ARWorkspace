#pragma once

#pragma once

#include "SensorManagerDefine.hpp"
#include "HRESULT_Support.hpp"

namespace WinSensor {
class PropvariantWrapper : public HRESULT_Support
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
	inline const PROPVARIANT& GetPropvariant() const
	{
		return this->pv_data;
	}
protected:
	PROPVARIANT	pv_data;
};

}