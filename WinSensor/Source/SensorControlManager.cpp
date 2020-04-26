
//#include <algorithm>
#include "SensorControlManager.hpp"

namespace WinSensor {
SensorControlManager::SensorControlManager()
{
	this->p_control_list.reserve(20);
}
SensorControlManager::~SensorControlManager()
{
	this->RemoveAll();
}
HRESULT SensorControlManager::Add(ISensor* p_sensor, SensorRequest& request) noexcept
{
	HRESULT hr = S_OK;
	if (p_sensor == nullptr)
	{
		return E_POINTER;
	}
	SENSOR_ID sensor_id = GUID_NULL;
	{
		hr = p_sensor->GetID(&sensor_id);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	this->RemoveSensorInfoFromID(sensor_id);
	
	request.callback_sensor_leaved_func =
		[this](REFSENSOR_ID sensor_id)
		{
			HRESULT hr = S_OK;
			//this->RemoveSensorInfoFromID(sensor_id); // 落ちる
			return hr;
		};

	auto p_control = SensorControl::Create(sensor_id, p_sensor, request);
	if (p_control)
	{
		this->p_control_list.push_back(p_control.value());
	}
	return hr;
}

bool SensorControlManager::RemoveAll()
{

	// 逆順イテレータを使いたいがとりあえず.
	// std::remove_ifだとムーブコンストラクタが発生するか？.
	for (auto it = this->p_control_list.begin(); it != this->p_control_list.end();)
	{
		this->deleteSensorInfo(*it);
		it = this->p_control_list.erase(it);
	}
	
	return true;
}

bool SensorControlManager::RemoveSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept
{
	for (auto it = this->p_control_list.begin(); it != this->p_control_list.end();) 
	{	
		if (TRUE == ::IsEqualGUID((*it)->GetSensorID(), arg_sensor_id))
		{
			this->deleteSensorInfo(*it);
			it = this->p_control_list.erase(it);
		} else {
			++it;
		}
	}
	return true;
}

bool SensorControlManager::RemoveSensorInfoFromType(SENSOR_TYPE_ID arg_type_id) noexcept
{
	for (auto it = this->p_control_list.begin(); it != this->p_control_list.end();)
	{
		if (TRUE == ::IsEqualGUID((*it)->GetSensorTypeID(), arg_type_id))
		{
			this->deleteSensorInfo(*it);
			it = this->p_control_list.erase(it);
		}
		else {
			++it;
		}
	}
	return true;
}

}
