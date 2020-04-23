#pragma once

#include "WinSensorManager.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
enum class SensorType;

class WinSensorManagerHelper
{
public:	
	static SensorRequest MakeSensorRequest(WinSensorManager& manager, 
		const SensorType sensor_type) noexcept;

	static SensorRequest MakeSensorRequest_AggregatedDeviceOrientation(
		WinSensorManager& manager) noexcept;
	


};

}