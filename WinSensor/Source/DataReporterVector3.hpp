#pragma once

#include "SensorManagerDefine.hpp"
#include "HRESULT_Support.hpp"
#include "DataReporter.hpp"
#include "DataReporterTimeStamp.hpp"

namespace WinSensor {
class DataReporterVector3 : public HRESULT_Support
{
private:
	DataReporterVector3() = delete;
public:
	DataReporterVector3(
		ISensorDataReport* p_report, 
		const PROPERTYKEY& property_key_x,
		const PROPERTYKEY& property_key_y, 
		const PROPERTYKEY& property_key_z ) noexcept;
	~DataReporterVector3() noexcept;

	Double3AndTimestamp GetValue() const noexcept;
private:
	DataReporter			data_double_x;
	DataReporter			data_double_y;
	DataReporter			data_double_z;
	DataReporterTimeStamp	time_stamp;
};

}