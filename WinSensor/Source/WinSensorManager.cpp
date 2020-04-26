
#include "DeviceList.hpp"
#include "SensorRequestHelper.hpp"
#include "SensorMethodHelper.hpp"
#include "WinSensorManager.hpp"

namespace WinSensor {
using Helper = SensorRequestHelper;
WinSensorManager::WinSensorManager()
{
	this->request_list.reserve(20);
	this->connect_list.reserve(20);
	this->priority_vid_list.reserve(20);
	
	this->sp_sensor_manager_events = 
		std::make_unique<SensorManagerEvents>(
			[this](ISensor* p_sensor, SensorState state)
			{
				return this->onSensorEnterEvent(p_sensor, state);
			});

}


WinSensorManager::~WinSensorManager()
{
	this->Uninitialize();
}

bool WinSensorManager::Initialize()
{
	HRESULT hr;
	hr = this->sp_sensor_manager.CoCreateInstance(CLSID_SensorManager);
	if (SUCCEEDED(hr))
	{
		hr = this->sp_sensor_manager->SetEventSink(this->sp_sensor_manager_events.get());
		if (FAILED(hr))
		{
			this->state = SensorManagerState::InitializeError;
			return false;
		}
	}
	//////
	this->state = SensorManagerState::InitializeCompleted;
	return true;
}

bool WinSensorManager::Uninitialize()
{
	if (this->state != SensorManagerState::UnInitialized)
	{
		HRESULT result = S_OK;
		this->sensor_control_manager.RemoveAll();
		result = this->sp_sensor_manager->SetEventSink(NULL);
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
	bool result = this->addSensorWithMakeRequest(request_sensor_type, this->priority_vid_list);
	if (! result)
	{
		this->addSensorWithMakeRequest(request_sensor_type);
	}
	return true;
}

bool WinSensorManager::AddSensorFromVidList(const SensorType request_sensor_type, const std::vector<std::wstring>& vid_list)
{
	return this->addSensorWithMakeRequest(request_sensor_type, vid_list);
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

const FloatAndTimestamp& WinSensorManager::GetAmbientLightData() const noexcept
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

bool WinSensorManager::addSensorWithMakeRequest(const SensorType request_sensor_type, 
	const std::optional<const std::vector<std::wstring>>& vid_list)
{
	HRESULT hr;
	SensorRequest request;
	request = Helper::MakeSensorRequest(*this, request_sensor_type);
	if (vid_list)
	{
		if (vid_list.value().size() > 0)
		{
			request.target_state = SensorRequestTargetState::Priority;
			request.vid_list = vid_list.value();
		}
	}
	bool result = this->addSensor(request);
	this->addRequest(request);
	
	return result;
}
bool WinSensorManager::addSensor(SensorRequest& request)
{
	HRESULT hr;
	hr = SensorMethodHelper::AddSensor(request, this->sp_sensor_manager, this->sensor_control_manager);	
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool WinSensorManager::addRequest(SensorRequest& request)
{
	if (request.state != SensorRequestState::SensorTypeError &&
		request.state != SensorRequestState::RequestError)
	{
		//if (std::find(this->request_list.begin(),
		//	this->request_list.end(), request) == this->request_list.end())
		//{
		this->request_list.emplace_back(request);
		//}
		if (request.state == SensorRequestState::Connected)
		{
			this->connect_list.emplace_back(request);
		}
		return true;
	}
	return false;
}

HRESULT WinSensorManager::onSensorEnterEvent(ISensor* p_sensor, SensorState state)
{
	// 
	//HRESULT hr;
	//SENSOR_TYPE_ID type_id;
	//hr = p_sensor->GetType(&type_id);
	//if (FAILED(hr)) 
	//{
	//	return hr;
	//}
	//for (const auto& connected_sensor : this->connect_list)
	//{
	//	if (::IsEqualGUID(type_id, connected_sensor.type_id))
	//	{
	//		//if (connected_sensor.target_state == SensorRequestTargetState::Priority)
	//		{
	//			return hr;
	//		}
	//	}
	//}
	//for (auto& requested_sensor : this->request_list)
	//{
	//	if (::IsEqualGUID(type_id, requested_sensor.type_id))
	//	{
	//		this->addSensor(requested_sensor);
	//	}
	//}
	return S_OK;

}



}