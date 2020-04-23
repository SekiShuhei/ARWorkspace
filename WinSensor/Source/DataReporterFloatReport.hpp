#pragma once

#include "SensorManagerDefine.hpp"
#include "HRESULT_Support.hpp"
#include "DataReporterFloat.hpp"
#include "DataReporterTimeStamp.hpp"

namespace WinSensor {
class DataReporterFloatReport : public HRESULT_Support
{
private:
	DataReporterFloatReport() = delete;
public:
	DataReporterFloatReport(ISensorDataReport* p_report, const PROPERTYKEY& property_key);
	~DataReporterFloatReport();

	FloatAndTimestamp GetValue() const;
private:
	DataReporterFloat		data_float;
	DataReporterTimeStamp	time_stamp;
};

}