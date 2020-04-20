#pragma once


#include <memory>
#include <tuple>
#include <optional>

#include "SensorManagerEvents.hpp"
namespace WinSensor {

using Vector3 = std::tuple<double, double, double>;
using Quaternion = std::tuple<double, double, double, double>;
class WinSensorManager
{
public:
	WinSensorManager();
	~WinSensorManager();
	bool Initialize();
	bool Uninitialize();
	//std::optional<Vector3>		GetAccelerometerData();
	//std::optional<Vector3>		GetCompassData();
	//std::optional<Vector3>		GetGyrometerData();
	//std::optional<float>		GetAmbientLightData();
	//std::optional<Vector3>		GetGravityVectorData();
	//std::optional<Vector3>		GetLinearAccelerometerData();
	std::optional<Quaternion>	GetAggregatedDeviceOrientationData();

private:
	
private:
	
	bool intialized = false;
	
	std::unique_ptr<WinSensor::SensorManagerEvents> p_sensor_manager;
};

}