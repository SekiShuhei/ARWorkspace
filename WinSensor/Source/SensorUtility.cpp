
#include "PropertyReporterString.hpp"

#include "SensorUtility.hpp"


namespace WinSensor {
bool Utility::StringContains(const std::wstring& base, const std::vector<std::wstring>& list)
{
	for (const auto& string : list)
	{
		if (base.find(string) != std::string::npos) 
		{
			return true;
		}
	}
	return false;
}

std::optional<std::wstring> Utility::GetDevicePath(ISensor* p_sensor)
{
	PropertyReporterString device_path_report(SENSOR_PROPERTY_DEVICE_PATH, p_sensor);
	if (device_path_report.IsError())
	{
		return std::nullopt;
	}
	return device_path_report.GetValue();
}

}
