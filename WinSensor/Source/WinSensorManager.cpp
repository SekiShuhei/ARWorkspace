
#include "DataReporterQuaternion.hpp"

#include "WinSensorManager.hpp"

namespace WinSensor {

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
		return false;
	}
	SensorRequest request;
	request.type_id = SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION;
	//request.vid_list.emplace_back(L"VID_0483"); // BT-35E
	//request.vid_list.emplace_back(L"VID_04B8"); // BT-30C
	request.callback_func = 
		[this](ISensor * p_sensor, ISensorDataReport* p_data)
		{
			DataReporterQuaternion data_report(p_data);
			if (!data_report.IsError())
			{
				this->last_quaternion_report = data_report.GetValue();
			}
			return data_report.GetResult();
		};

	hr = this->p_sensor_manager->AddSensor(request);
	if (FAILED(hr))
	{
		int i = 1;
	}
	//test 2‰ñ“o˜^ƒeƒXƒg.
	hr = this->p_sensor_manager->AddSensor(request);


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