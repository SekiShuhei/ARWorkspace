

#include "WinSensorManager.hpp"

namespace WinSensor {

WinSensorManager::WinSensorManager()
{
	this->p_sensor_manager = 
		std::make_unique<SensorManagerEvents>(
			[this](Float4AndTimestamp arg_report)
			{
				this->last_quaternion_report = arg_report;
				return true; 
			});
	//SensorManagerEvents([this](Float4AndTimestamp){return true;});
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
		return false;
	}
	SensorRequest request;
	request.type_id = SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION;
	//request.vid_list.emplace_back(L"VID_0483"); // BT-35E
	//request.vid_list.emplace_back(L"VID_04B8"); // BT-30C


	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		int i = 1;
	}
	//test
	//hr = this->p_sensor_manager->AddSensor(SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION);


	return true;



}

bool WinSensorManager::Uninitialize()
{
	this->p_sensor_manager->Uninitialize();
	return false;
}

const Float4AndTimestamp& WinSensorManager::GetAggregatedDeviceOrientationData()
{
	//if (!this->intialized)
	//{
	//	return std::nullopt;
	//}

	return this->last_quaternion_report;

}


}