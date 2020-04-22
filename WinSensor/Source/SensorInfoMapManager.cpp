
#include <algorithm>
#include "SensorInfoMapManager.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
SensorInfoMapManager::SensorInfoMapManager()
{
	this->info_map.reserve(20);
}
SensorInfoMapManager::~SensorInfoMapManager()
{
	this->RemoveAll();
}
bool SensorInfoMapManager::Add(ISensor* p_sensor, const SensorRequest& request) noexcept
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

	this->removeSensorInfoFromID(sensor_id);
	this->info_map.push_back(new SensorInfo(sensor_id, p_sensor, request));

	return false;
}

bool SensorInfoMapManager::RemoveAll()
{

	// 逆順イテレータを使いたいがとりあえず.
	// std::remove_ifだとムーブコンストラクタが発生するか？.
	for (auto it = this->info_map.begin(); it != this->info_map.end();)
	{
		this->deleteSensorInfo(*it);
		it = this->info_map.erase(it);
	}
	
	return true;
}

bool SensorInfoMapManager::removeSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept
{
	for (auto it = this->info_map.begin(); it != this->info_map.end();) 
	{	
		if (TRUE == ::IsEqualGUID((*it)->GetSensorID(), arg_sensor_id))
		{
			this->deleteSensorInfo(*it);
			it = this->info_map.erase(it);
		} else {
			++it;
		}
	}
	return true;
}

bool SensorInfoMapManager::removeSensorInfoFromType(SENSOR_TYPE_ID arg_type_id) noexcept
{
	for (auto it = this->info_map.begin(); it != this->info_map.end();)
	{
		if (TRUE == ::IsEqualGUID((*it)->GetSensorTypeID(), arg_type_id))
		{
			this->deleteSensorInfo(*it);
			it = this->info_map.erase(it);
		}
		else {
			++it;
		}
	}
	return true;
}

}
