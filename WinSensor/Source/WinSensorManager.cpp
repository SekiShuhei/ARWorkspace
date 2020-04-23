
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
	request.vid_list.emplace_back(L"VID_0483"); // BT-35E
	request.vid_list.emplace_back(L"VID_04B8"); // BT-30C
	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		// とりあえず開発チェック用にBTが見つからなかったらSurfaceセンサを見つける.
		request = Helper::MakeSensorRequest(*this, request_sensor_type);
		hr = this->p_sensor_manager->AddSensor(request);
	}

	return true;
}

const Float4AndTimestamp& WinSensorManager::GetAggregatedDeviceOrientationData() const noexcept
{
	return this->last_quaternion_report;
}



}