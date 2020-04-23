#pragma once

#include "SensorManagerDefine.hpp"
#include "PropvariantWrapper.hpp"

namespace WinSensor {
class DataReporterFloat : public PropvariantWrapper
{
private:
	DataReporterFloat() = delete;
public:
	DataReporterFloat(ISensorDataReport* p_report, const PROPERTYKEY&);
	~DataReporterFloat();

	float GetValue() const;
};

}