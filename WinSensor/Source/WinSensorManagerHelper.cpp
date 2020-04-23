
#include "DataReporterQuaternion.hpp"

#include "SensorType.hpp"
#include "WinSensorManagerHelper.hpp"

namespace WinSensor {
using Helper = WinSensorManagerHelper;
SensorRequest WinSensorManagerHelper::MakeSensorRequest(WinSensorManager& manager, const SensorType sensor_type) noexcept
{
	switch (sensor_type)
	{
	case SensorType::AggregatedDeviceOrientation:
		return Helper::MakeSensorRequest_AggregatedDeviceOrientation(manager);
	default:
		return SensorRequest();
	}
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_AggregatedDeviceOrientation(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.type_id = SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
		{
			DataReporterQuaternion data_report(p_data);
			if (!data_report.IsError())
			{
				manager.last_orientation_quaternion_report = data_report.GetValue();
			}
			return data_report.GetResult();
		};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_AmbientLight(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.type_id = SENSOR_TYPE_AMBIENT_LIGHT;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		///
		DataReporterQuaternion data_report(p_data);
		if (!data_report.IsError())
		{
			manager.last_orientation_quaternion_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Accelerometer(WinSensorManager& manager) noexcept
{
	return SensorRequest();
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Compass(WinSensorManager& manager) noexcept
{
	return SensorRequest();
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Gyrometer(WinSensorManager& manager) noexcept
{
	return SensorRequest();
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_GravityVector(WinSensorManager& manager) noexcept
{
	return SensorRequest();
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_LinearAccelerometer(WinSensorManager& manager) noexcept
{
	return SensorRequest();
}

}
