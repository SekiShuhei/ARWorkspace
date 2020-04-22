#pragma once

#include "SensorManagerDefine.hpp"
#include "PropvariantWrapper.hpp"
#include "DataReporterTimeStamp.hpp"

namespace WinSensor {
class DataReporterQuaternion : public PropvariantWrapper
{
private:
	DataReporterQuaternion() = delete;
public:
	DataReporterQuaternion(ISensorDataReport* p_report);
	~DataReporterQuaternion();

	Float4AndTimestamp GetValue() const;
private:
	DataReporterTimeStamp time_stamp;
};

}