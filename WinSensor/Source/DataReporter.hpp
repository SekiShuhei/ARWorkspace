#pragma once

#include <cassert>
#include "SensorManagerDefine.hpp"
#include "PropvariantWrapper.hpp"

namespace WinSensor {
class DataReporter : public PropvariantWrapper
{
private:
	DataReporter() = delete;
public:
	DataReporter(ISensorDataReport* p_report, const PROPERTYKEY& property_key) noexcept
	{
		if (p_report == nullptr)
		{
			this->result = E_INVALIDARG;
		}
		else {
			this->result = p_report->GetSensorValue(property_key, &this->pv_data);
		}
	}
	~DataReporter()noexcept {};

	template<typename T> const T GetValue() const noexcept
	{
		assert("DataReporter Not Impl...");
	}
	template<> const float GetValue<float>() const noexcept
	{
		if (!this->IsError())
		{
			return this->pv_data.fltVal;
		}
		return 0;
	}

};

}