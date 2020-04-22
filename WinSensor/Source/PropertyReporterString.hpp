#pragma once

#pragma once

#include <string>
#include "SensorManagerDefine.hpp"
#include "PropvariantWrapper.hpp"

namespace WinSensor {
class PropertyReporterString : public PropvariantWrapper
{
private:
	PropertyReporterString() = delete;
public:
	PropertyReporterString(const PROPERTYKEY property_key, ISensor* p_sensor);
	~PropertyReporterString();

	std::wstring GetValue() const;
private:
};

}