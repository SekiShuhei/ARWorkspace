
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

	// コンテナに重複キーがあったら削除を先にする.
	this->removeSensorInfoFromID(sensor_id);

	// SensorInfoの動的生成.
	auto p_info = new SensorInfo(sensor_id, p_sensor, request);
	// コンテナに入れる.
	this->info_map.push_back(p_info);
	

	return false;
}
// 削除予定.
bool SensorInfoMapManager::Set(SENSOR_ID key, ISensor* p_sensor, ISensorEvents* p_sensor_events)
{
	// del...
	if( p_sensor == nullptr || p_sensor_events == nullptr)
	{
		return false;
	}
	//if (this->inner_map[key] != nullptr)
	//{
	//	this->Remove(key);
	//}
	p_sensor->SetEventSink(p_sensor_events);
	p_sensor->AddRef();
	this->inner_map[key] = p_sensor;
	////

	return true;
}

bool SensorInfoMapManager::Remove(SENSOR_ID arg_key) noexcept
{
	// del...
	auto p_sensor = this->inner_map[arg_key];
	SENSOR_ID sensor_id = GUID_NULL;
	if (SUCCEEDED(p_sensor->GetID(&sensor_id)))
	{
		p_sensor->SetEventSink(NULL);
		p_sensor->Release();
	}
	this->inner_map.RemoveKey(arg_key);
	/////////
	
	
	return true;
}

bool SensorInfoMapManager::RemoveAll()
{
	// del...
	POSITION pos = this->inner_map.GetStartPosition();
	while (NULL != pos)
	{
		auto key = this->inner_map.GetNextKey(pos);
		this->Remove(key);
	}
	///////////

	// 逆順イテレータを使いたいがとりあえず.
	// std::remove_ifだとムーブコンストラクタが発生するか？.
	for (auto it = this->info_map.begin(); it != this->info_map.end();)
	{
		it = this->info_map.erase(it);
		deleteSensorInfo(*it);
	}

	
	return true;
}

bool SensorInfoMapManager::removeSensorInfoFromID(SENSOR_ID arg_sensor_id) noexcept
{
	for (auto it = this->info_map.begin(); it != this->info_map.end();) 
	{	
		if (TRUE == ::IsEqualGUID((*it)->GetSensorID(), arg_sensor_id))
		{
			it = this->info_map.erase(it);
			deleteSensorInfo(*it);
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
			it = this->info_map.erase(it);
			deleteSensorInfo(*it);
		}
		else {
			++it;
		}
	}
	return true;
}

}
