#include "DataReporterTimeStamp.hpp"

namespace WinSensor {
DataReporterTimeStamp::DataReporterTimeStamp(ISensorDataReport* p_report) :
	PropvariantWrapper()
{
	if (p_report == nullptr)
	{
		this->result = E_INVALIDARG;
	} else {
		this->result = p_report->GetSensorValue(SENSOR_DATA_TYPE_TIMESTAMP, &this->pv_data);
	}
}

DataReporterTimeStamp::~DataReporterTimeStamp()
{
}

UINT64 DataReporterTimeStamp::GetValue() const
{
	if (! this->IsError())
	{
		return ((UINT64)this->pv_data.filetime.dwHighDateTime << 32) |
			this->pv_data.filetime.dwLowDateTime;;
	}
	return 0;
}
}
