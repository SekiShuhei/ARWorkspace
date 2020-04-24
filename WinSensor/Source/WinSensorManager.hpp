#pragma once


#include <memory>
#include <tuple>
#include <optional>

#include "SensorManagerDefine.hpp"
#include "SensorManagerEvents.hpp"
#include "SensorType.hpp"
#include "SensorManagerState.hpp"
#include "DeviceList.hpp"

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
	bool AddSensorFromVidList(
		const SensorType request_sensor_type, const std::vector<std::wstring>& vid_list);


	const Double3AndTimestamp&	GetAccelerometerData() const noexcept;
	const Double3AndTimestamp&	GetCompassData() const noexcept;
	const Double3AndTimestamp&	GetGyrometerData() const noexcept;
	const FloatAndTimestamp&	GetAmbientLightData() const noexcept;
	const Double3AndTimestamp&	GetGravityVectorData() const noexcept;
	const Double3AndTimestamp&	GetLinearAccelerometerData() const noexcept;
	const Float4AndTimestamp&	GetAggregatedDeviceOrientationData() const noexcept;

	void SetPriorityVidList(const std::vector<std::wstring> vid_list)
	{
		this->priority_vid_list = vid_list;
	}
	const SensorManagerState	GetState() const noexcept
	{
		return this->state;
	}

private:
	bool addSensor(
		const SensorType request_sensor_type, 
		const std::optional<const std::vector<std::wstring>>& vid_list = std::nullopt);

private:
	SensorManagerState state = SensorManagerState::NotInitialized;

	std::unique_ptr<WinSensor::SensorManagerEvents> p_sensor_manager;

	Double3AndTimestamp	last_accelerometer_report			= Double3AndTimestamp();
	Double3AndTimestamp	last_compass_report					= Double3AndTimestamp();
	Double3AndTimestamp	last_gyrometer_report				= Double3AndTimestamp();
	Double3AndTimestamp	last_gravity_vector_report			= Double3AndTimestamp();
	Double3AndTimestamp	last_linear_accelerometer_report	= Double3AndTimestamp();
	FloatAndTimestamp	last_ambient_light_report			= FloatAndTimestamp();
	Float4AndTimestamp	last_orientation_quaternion_report	= Float4AndTimestamp();

	// TODO:
	// グローバル優先デバイスリスト.
	std::vector<std::wstring> priority_vid_list;

	// センサー別優先デバイスリスト.
};

}