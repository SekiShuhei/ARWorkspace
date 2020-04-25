
#include <ctime>
#include <chrono>
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
		//////.
		// TODO:
		//using namespace std::literals::chrono_literals;
		//auto hour = 12h;
		//std::chrono::time_point< t;
		SYSTEMTIME system_time;
		this->result = p_report->GetTimestamp(&system_time);
		if (SUCCEEDED(result))
		{
			//int a = 1;
		}
		//	system_time.wYear;
		//std::chrono::system_clock::time_point time_point;
		//std::chrono::
		////}

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
