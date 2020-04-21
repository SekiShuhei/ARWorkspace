
#include "PropertyReporterString.hpp"

#include "SensorUtility.hpp"

namespace WinSensor {
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
