#pragma once

#include "SensorManagerDefine.hpp"
#include "WinSensorManager.hpp"
#include "SensorRequest.hpp"
#include "SensorControlManager.hpp"

namespace WinSensor {
class SensorMethodHelper
{
public:	
	
	static HRESULT AddSensor(
		SensorRequest& request, 
		ISensorManager* p_sensor_manager, 
		SensorControlManager& sensor_control_manager);

};

}