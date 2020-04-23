#pragma once

#include "WinSensorManager.hpp"
#include "SensorRequest.hpp"

namespace WinSensor {
class WinSensorManagerHelper
{
public:	
	static SensorRequest MakeSensorRequest_AggregatedDeviceOrientation(
		WinSensorManager& manager) noexcept;
	


};

}