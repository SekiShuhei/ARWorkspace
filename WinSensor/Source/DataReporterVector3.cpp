#include "DataReporterVector3.hpp"

namespace WinSensor {
DataReporterVector3::DataReporterVector3(ISensorDataReport* p_report, 
	const PROPERTYKEY& property_key_x,
	const PROPERTYKEY& property_key_y,
	const PROPERTYKEY& property_key_z) noexcept :
	data_double_x(p_report, property_key_x),
	data_double_y(p_report, property_key_y),
	data_double_z(p_report, property_key_z),
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
	else if (this->data_double_x.IsError()) 
	{
		this->result = this->data_double_x.GetResult();
	}
	else if (this->data_double_y.IsError())
	{
		this->result = this->data_double_y.GetResult();
	}
	else {
		this->result = this->data_double_z.GetResult();
	}
}

DataReporterVector3::~DataReporterVector3() noexcept
{
}

Double3AndTimestamp DataReporterVector3::GetValue() const noexcept
{
	if (!this->IsError())
	{
		return Double3AndTimestamp(
			this->data_double_x.GetValue<double>(),
			this->data_double_y.GetValue<double>(),
			this->data_double_z.GetValue<double>(),
			this->time_stamp.GetValue());
	}
	return Double3AndTimestamp();
}

}