#include "PropertyReporterString.hpp"

namespace WinSensor {
PropertyReporterString::PropertyReporterString(const PROPERTYKEY property_key, ISensor* p_sensor) :
	PropvariantWrapper()
{
	if (p_sensor == nullptr)
	{
		this->result = E_INVALIDARG; 
	} else {
		this->result = p_sensor->GetProperty(property_key, &this->pv_data);
	}
}

PropertyReporterString::~PropertyReporterString()
{
}

std::wstring PropertyReporterString::GetValue() const
{
	if (this->IsError())
	{
		return std::wstring();
	}
	return std::wstring(this->pv_data.bstrVal);
}

}