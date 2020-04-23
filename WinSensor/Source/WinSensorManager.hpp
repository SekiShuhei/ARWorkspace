#pragma once


#include <memory>
#include <tuple>
#include <optional>

#include "SensorManagerDefine.hpp"
#include "SensorManagerEvents.hpp"
#include "SensorType.hpp"
#include "SensorManagerState.hpp"

namespace WinSensor {
class WinSensorManager
{
friend class WinSensorManagerHelper;

public:
	WinSensorManager();
	~WinSensorManager();
	bool Initialize();
	bool Uninitialize();

	bool AddSensor(const SensorType request_sensor_type);

	const Double3AndTimestamp&	GetAccelerometerData() const noexcept;
	const Double3AndTimestamp&	GetCompassData() const noexcept;
	const Double3AndTimestamp&	GetGyrometerData() const noexcept;
	const FloatAndTimestamp		GetAmbientLightData() const noexcept;
	const Double3AndTimestamp&	GetGravityVectorData() const noexcept;
	const Double3AndTimestamp&	GetLinearAccelerometerData() const noexcept;
	const Float4AndTimestamp&	GetAggregatedDeviceOrientationData() const noexcept;

	const SensorManagerState	GetState() const noexcept
	{
		return this->state;
	}

private:
	
private:
	SensorManagerState state = SensorManagerState::NotInitialized;

	std::unique_ptr<WinSensor::SensorManagerEvents> p_sensor_manager;

	Float4AndTimestamp	last_quaternion_report = Float4AndTimestamp();

	// TODO:
	// グローバル優先デバイスリスト.
	// センサー別優先デバイスリスト.
};

}