
//#include <sensorsdef.h>
//#include <initguid.h>
//#include <cassert>

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
	this->p_sensor_manager->Initialize();	
	

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