#include "DataReporterFloatReport.hpp"

namespace WinSensor {
DataReporterFloatReport::DataReporterFloatReport(ISensorDataReport* p_report, const PROPERTYKEY& property_key) :
	data_float(p_report, property_key),
	time_stamp(p_report)
{
	if (p_report == nullptr)
	{
		this->result = E_INVALIDARG;
	}
	else if (this->time_stamp.IsError())
	{
		this->result = this->time_stamp.GetResult();
	}
	else {
		this->result = this->data_float.GetResult();
	}
}

DataReporterFloatReport::~DataReporterFloatReport()
{
}

FloatAndTimestamp DataReporterFloatReport::GetValue() const
{
	if (!this->IsError())
	{
		return FloatAndTimestamp(
			this->data_float.GetValue(), this->time_stamp.GetValue());
	}
	return FloatAndTimestamp();
}
}