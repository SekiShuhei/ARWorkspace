
#include "DataReporterQuaternion.hpp"
#include "DataReporterFloatReport.hpp"
#include "DataReporterVector3.hpp"

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

	case SensorType::AmbientLight:
		return Helper::MakeSensorRequest_AmbientLight(manager);

	case SensorType::Accelerometer:
		return Helper::MakeSensorRequest_Accelerometer(manager);

	case SensorType::Compass:
		return Helper::MakeSensorRequest_Compass(manager);

	case SensorType::GravityVector:
		return Helper::MakeSensorRequest_GravityVector(manager);

	case SensorType::Gyrometer:
		return Helper::MakeSensorRequest_Gyrometer(manager);

	case SensorType::LinearAccelerometer:
		return Helper::MakeSensorRequest_LinearAccelerometer(manager);

	default:
		return SensorRequest();
	}
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_AggregatedDeviceOrientation(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"AGGREGATED_DEVICE_ORIENTATION";
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
	request.name = L"AMBIENT_LIGHT";
	request.type_id = SENSOR_TYPE_AMBIENT_LIGHT;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterFloatReport data_report(p_data, SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX);
		if (!data_report.IsError())
		{
			manager.last_ambient_light_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Accelerometer(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"ACCELEROMETER_3D";
	request.type_id = SENSOR_TYPE_ACCELEROMETER_3D;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterVector3 data_report(
			p_data, 
			SENSOR_DATA_TYPE_ACCELERATION_X_G,
			SENSOR_DATA_TYPE_ACCELERATION_Y_G,
			SENSOR_DATA_TYPE_ACCELERATION_Z_G);

		if (!data_report.IsError())
		{
			manager.last_accelerometer_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Compass(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"COMPASS_3D";
	request.type_id = SENSOR_TYPE_COMPASS_3D;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterVector3 data_report(
			p_data,
			SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_X_MILLIGAUSS,
			SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Y_MILLIGAUSS,
			SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Z_MILLIGAUSS);

		if (!data_report.IsError())
		{
			manager.last_compass_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_Gyrometer(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"GYROMETER_3D";
	request.type_id = SENSOR_TYPE_GYROMETER_3D;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterVector3 data_report(
			p_data,
			SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND,
			SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND,
			SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND);

		if (!data_report.IsError())
		{
			manager.last_gyrometer_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_GravityVector(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"GravityVector";
	request.type_id = GUID_SensorType_GravityVector;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterVector3 data_report(
			p_data,
			SENSOR_DATA_TYPE_ACCELERATION_X_G,
			SENSOR_DATA_TYPE_ACCELERATION_Y_G,
			SENSOR_DATA_TYPE_ACCELERATION_Z_G);

		if (!data_report.IsError())
		{
			manager.last_gravity_vector_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

SensorRequest WinSensorManagerHelper::MakeSensorRequest_LinearAccelerometer(WinSensorManager& manager) noexcept
{
	SensorRequest request;
	request.name = L"LinearAccelerometer";
	request.type_id = GUID_SensorType_LinearAccelerometer;
	request.callback_func =
		[&manager](ISensor* p_sensor, ISensorDataReport* p_data)
	{
		DataReporterVector3 data_report(
			p_data,
			SENSOR_DATA_TYPE_ACCELERATION_X_G,
			SENSOR_DATA_TYPE_ACCELERATION_Y_G,
			SENSOR_DATA_TYPE_ACCELERATION_Z_G);

		if (!data_report.IsError())
		{
			manager.last_linear_accelerometer_report = data_report.GetValue();
		}
		return data_report.GetResult();
	};
	return request;
}

}
