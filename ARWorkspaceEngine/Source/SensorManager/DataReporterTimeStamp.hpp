#pragma once

#include "SensorManagerDefine.hpp"

namespace WinSensor {
class DataReporterTimeStamp
{
private:
	DataReporterTimeStamp()											= delete;
	DataReporterTimeStamp(const DataReporterTimeStamp&)				= delete;
	DataReporterTimeStamp& operator=(const DataReporterTimeStamp&)	= delete;
	DataReporterTimeStamp(DataReporterTimeStamp&&)					= delete;
	DataReporterTimeStamp& operator=(DataReporterTimeStamp&&)		= delete;
public:
	DataReporterTimeStamp(ISensorDataReport* p_report);
	~DataReporterTimeStamp();

	bool IsError() const
	{
		return FAILED(this->result);
	}
	UINT64 GetValue() const ;
private:
	HRESULT		result = S_OK;
	PROPVARIANT	pv_data;
};

}