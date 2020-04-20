#pragma once


#include <memory>
#include <tuple>
#include <optional>

#include "SensorManagerDefine.hpp"
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
	const Float4AndTimestamp&	GetAggregatedDeviceOrientationData();

private:
	
private:
	bool intialized = false;
	
	std::unique_ptr<WinSensor::SensorManagerEvents> p_sensor_manager;

	Float4AndTimestamp	last_quaternion_report = Float4AndTimestamp();
};

}