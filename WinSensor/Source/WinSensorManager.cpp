
#include "DeviceList.hpp"
#include "WinSensorManagerHelper.hpp"
#include "WinSensorManager.hpp"

namespace WinSensor {
using Helper = WinSensorManagerHelper;
WinSensorManager::WinSensorManager()
{
	this->p_sensor_manager = 
		std::make_unique<SensorManagerEvents>();

}


WinSensorManager::~WinSensorManager()
{
	this->Uninitialize();
}

bool WinSensorManager::Initialize()
{
	HRESULT hr;
	hr = this->p_sensor_manager->Initialize();	
	if (FAILED(hr))
	{
		this->state = SensorManagerState::InitializeError;
		return false;
	}
	this->state = SensorManagerState::InitializeCompleted;
	return true;
}

bool WinSensorManager::Uninitialize()
{
	if (this->state != SensorManagerState::UnInitialized)
	{
		auto result = this->p_sensor_manager->Uninitialize();
		if (SUCCEEDED(result))
		{
			this->state = SensorManagerState::UnInitialized;
			return true;
		}
		return false;
	}
	return true;
}

bool WinSensorManager::AddSensor(const SensorType request_sensor_type)
{
	bool result = this->addSensor(request_sensor_type, this->priority_vid_list);
	if (! result)
	{
		this->addSensor(request_sensor_type);
	}
	return true;
}

bool WinSensorManager::AddSensorFromVidList(const SensorType request_sensor_type, const std::vector<std::wstring>& vid_list)
{
	return this->addSensor(request_sensor_type, vid_list);
}

const Double3AndTimestamp& WinSensorManager::GetAccelerometerData() const noexcept
{
	return this->last_accelerometer_report;
}

const Double3AndTimestamp& WinSensorManager::GetCompassData() const noexcept
{
	return this->last_compass_report;
}

const Double3AndTimestamp& WinSensorManager::GetGyrometerData() const noexcept
{
	return this->last_gyrometer_report;
}

const FloatAndTimestamp WinSensorManager::GetAmbientLightData() const noexcept
{
	return this->last_ambient_light_report;
}

const Double3AndTimestamp& WinSensorManager::GetGravityVectorData() const noexcept
{
	return this->last_gravity_vector_report;
}

const Double3AndTimestamp& WinSensorManager::GetLinearAccelerometerData() const noexcept
{
	return this->last_linear_accelerometer_report;
}

const Float4AndTimestamp& WinSensorManager::GetAggregatedDeviceOrientationData() const noexcept
{
	return this->last_orientation_quaternion_report;
}

bool WinSensorManager::addSensor(const SensorType request_sensor_type, 
	const std::optional<const std::vector<std::wstring>>& vid_list)
{
	HRESULT hr;
	SensorRequest request;
	request = Helper::MakeSensorRequest(*this, request_sensor_type);
	if (vid_list)
	{
		if (vid_list.value().size() > 0)
		{
			request.vid_list = vid_list.value();
		}
	}
	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}



}