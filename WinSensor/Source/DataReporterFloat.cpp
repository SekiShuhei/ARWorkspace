#include "DataReporterFloat.hpp"

namespace WinSensor {
DataReporterFloat::DataReporterFloat(ISensorDataReport* p_report, const PROPERTYKEY& property_key) :
	PropvariantWrapper()
{
	if (p_report == nullptr)
	{
		this->result = E_INVALIDARG;
	}
	else {
		this->result = p_report->GetSensorValue(property_key, &this->pv_data);
	}
}

DataReporterFloat::~DataReporterFloat()
{
}

float DataReporterFloat::GetValue() const
{
	if (!this->IsError())
	{
		return this->pv_data.fltVal;
	}
	return 0;
}
}