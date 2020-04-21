#include "DataReporterTimeStamp.hpp"


namespace WinSensor {
DataReporterTimeStamp::DataReporterTimeStamp(ISensorDataReport* p_report)
{
	PropVariantInit(&this->pv_data);
	if (p_report == nullptr)
	{
		this->result = E_INVALIDARG;
	}
	this->result = p_report->GetSensorValue(SENSOR_DATA_TYPE_TIMESTAMP, &this->pv_data);
}

DataReporterTimeStamp::~DataReporterTimeStamp()
{
	PropVariantClear(&this->pv_data);
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
