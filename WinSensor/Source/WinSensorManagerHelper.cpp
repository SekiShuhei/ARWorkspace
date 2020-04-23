
#include "DataReporterQuaternion.hpp"

#include "WinSensorManagerHelper.hpp"

namespace WinSensor {
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
				manager.last_quaternion_report = data_report.GetValue();
			}
			return data_report.GetResult();
		};
	return request;
}

}
