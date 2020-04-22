
#include <algorithm>
#include "SensorInfoManager.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
SensorInfoManager::SensorInfoManager()
{
	this->p_info_list.reserve(20);
}
SensorInfoManager::~SensorInfoManager()
{
	this->RemoveAll();
}
bool SensorInfoManager::Add(ISensor* p_sensor, const SensorRequest& request) noexcept
{

	if (p_sensor == nullptr)
	{
		return false;
	}
	HRESULT hr = S_OK;
	SENSOR_ID sensor_id = GUID_NULL;
	{
		hr = p_sensor->GetID(&sensor_id);
		if (FAILED(hr))
		{
			return false;
		}
	}

	this->RemoveSensorInfoFromID(sensor_id);
	auto p_info = SensorInfo::Create(sensor_id, p_sensor, request);
	if (p_info)
	{
		this->p_info_list.push_back(p_info.value());
	}
	return false;
}

bool SensorInfoManager::RemoveAll()
{

	// 逆順イテレータを使いたいがとりあえず.
	// std::remove_ifだとムーブコンストラクタが発生するか？.
	for (auto it = this->p_info_list.begin(); it != this->p_info_list.end();)
	{
		this->deleteSensorInfo(*it);
		it = this->p_info_list.erase(it);
	}
	
	return true;
}

bool SensorInfoManager::RemoveSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept
{
	for (auto it = this->p_info_list.begin(); it != this->p_info_list.end();) 
	{	
		if (TRUE == ::IsEqualGUID((*it)->GetSensorID(), arg_sensor_id))
		{
			this->deleteSensorInfo(*it);
			it = this->p_info_list.erase(it);
		} else {
			++it;
		}
	}
	return true;
}

bool SensorInfoManager::RemoveSensorInfoFromType(SENSOR_TYPE_ID arg_type_id) noexcept
{
	for (auto it = this->p_info_list.begin(); it != this->p_info_list.end();)
	{
		if (TRUE == ::IsEqualGUID((*it)->GetSensorTypeID(), arg_type_id))
		{
			this->deleteSensorInfo(*it);
			it = this->p_info_list.erase(it);
		}
		else {
			++it;
		}
	}
	return true;
}

}
