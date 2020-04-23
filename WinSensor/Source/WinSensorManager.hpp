#pragma once


#include <memory>
#include <tuple>
#include <optional>

#include "SensorManagerDefine.hpp"
#include "SensorManagerEvents.hpp"
#include "SensorType.hpp"

namespace WinSensor {
class WinSensorManager
{
friend class WinSensorManagerHelper;

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
	const Float4AndTimestamp&	GetAggregatedDeviceOrientationData() const;

private:
	
private:
	bool initialized = false;
	
	std::unique_ptr<WinSensor::SensorManagerEvents> p_sensor_manager;

	Float4AndTimestamp	last_quaternion_report = Float4AndTimestamp();

	// TODO:
	// グローバル優先デバイスリスト.
	// センサー別優先デバイスリスト.
};

}