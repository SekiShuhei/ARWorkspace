#pragma once

#include "SensorManagerDefine.hpp"
#include "PropvariantWrapper.hpp"

namespace WinSensor {
class DataReporterTimeStamp : public PropvariantWrapper
{
private:
	DataReporterTimeStamp()	= delete;
public:
	DataReporterTimeStamp(ISensorDataReport* p_report);
	~DataReporterTimeStamp();

	UINT64 GetValue() const ;
};

}