
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
}

bool WinSensorManager::Initialize()
{
	HRESULT hr;
	hr = this->p_sensor_manager->Initialize();	
	if (FAILED(hr))
	{
		this->initialized = false;
		return false;
	}
	// AddSensor()‚É•ª—£.
	SensorType	sensor_type = SensorType::AggregatedDeviceOrientation;
	SensorRequest request;
	request = Helper::MakeSensorRequest(*this, sensor_type);
	request.vid_list.emplace_back(L"VID_0483"); // BT-35E
	request.vid_list.emplace_back(L"VID_04B8"); // BT-30C
	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		request = Helper::MakeSensorRequest(*this, sensor_type);
		hr = this->p_sensor_manager->AddSensor(request);
	}

	this->initialized = true;
	return true;



}

bool WinSensorManager::Uninitialize()
{
	this->p_sensor_manager->Uninitialize();
	return false;
}

const Float4AndTimestamp& WinSensorManager::GetAggregatedDeviceOrientationData() const noexcept
{
	return this->last_quaternion_report;
}



}