#pragma once

#include "WinSensorManager.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
enum class SensorType;

class WinSensorManagerHelper
{
public:	
	static SensorRequest MakeSensorRequest(WinSensorManager& manager, const SensorType sensor_type) noexcept;

	static SensorRequest MakeSensorRequest_AggregatedDeviceOrientation(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_AmbientLight(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_Accelerometer(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_Compass(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_Gyrometer(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_GravityVector(WinSensorManager& manager) noexcept;
	static SensorRequest MakeSensorRequest_LinearAccelerometer(WinSensorManager& manager) noexcept;

};

}