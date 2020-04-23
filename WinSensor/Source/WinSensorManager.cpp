
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
	HRESULT hr;
	SensorRequest request;
	request = Helper::MakeSensorRequest(*this, request_sensor_type);
	// �f�o�C�X�D�惊�X�g�͊O������o�^�ł���悤��.
	// �S�̃��X�g�ƃZ���T�����X�g�𓝍����đ���.
	// �������X�g���K�v�H.
	request.vid_list.emplace_back(L"VID_0483"); // BT-35E
	request.vid_list.emplace_back(L"VID_04B8"); // BT-30C
	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		// �Ƃ肠�����J���`�F�b�N�p��BT��������Ȃ�������Surface�Z���T��������.
		request = Helper::MakeSensorRequest(*this, request_sensor_type);
		hr = this->p_sensor_manager->AddSensor(request);
	}

	return true;
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



}