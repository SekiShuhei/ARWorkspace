
#include "DeviceList.hpp"
#include "WinSensorManagerHelper.hpp"
#include "WinSensorManager.hpp"

namespace WinSensor {
using Helper = WinSensorManagerHelper;
WinSensorManager::WinSensorManager()
{
	
	this->sp_sensor_manager_events = 
		std::make_unique<SensorManagerEvents>(
			[](ISensor* p_sensor, SensorState state) // SensorManagerEvents::OnSensorEnter.
			{
				// TODO:
				// リクエスト中のセンサが接続されたら自動的に追加されるようにしたい.
				// マネージャ側でリクエスト情報を保持しておく必要がある.
				//...

				return S_OK; 
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
		this->info_manager.RemoveAll();
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
	bool result = this->addSensor(request_sensor_type, this->priority_vid_list);
	if (! result)
	{
		this->addSensor(request_sensor_type);
	}
	return true;
}

bool WinSensorManager::AddSensorFromVidList(const SensorType request_sensor_type, const std::vector<std::wstring>& vid_list)
{
	return this->addSensor(request_sensor_type, vid_list);
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

bool WinSensorManager::addSensor(const SensorType request_sensor_type, 
	const std::optional<const std::vector<std::wstring>>& vid_list)
{
	HRESULT hr;
	SensorRequest request;
	request = Helper::MakeSensorRequest(*this, request_sensor_type);
	if (vid_list)
	{
		if (vid_list.value().size() > 0)
		{
			request.vid_list = vid_list.value();
		}
	}
	hr = this->addSensor(request);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

HRESULT WinSensorManager::addSensor(const SensorRequest& request)
{

	HRESULT hr;
	CComPtr<ISensorCollection> sp_sensor_collection;
	hr = this->sp_sensor_manager->GetSensorsByType(request.type_id, &sp_sensor_collection);

	if (FAILED(hr))
	{
		return hr;
	}
	ULONG sensor_count = 0;
	hr = sp_sensor_collection->GetCount(&sensor_count);
	if (FAILED(hr))
	{
		return hr;
	}
	for (ULONG i = 0; i < sensor_count; i++)
	{
		CComPtr<ISensor> sp_sensor;
		hr = sp_sensor_collection->GetAt(i, &sp_sensor);
		if (FAILED(hr))
		{
			continue;
		}
		// ここでセンサステートを見て、DENIEDだったらアクセスパーミッションを要求する.
		//hr = p_sensor->GetState(&state);
		//if (state == SENSOR_STATE_ACCESS_DENIED)
		// ユーザーアクセス許可がない場合.
		//hr = this->sp_sensor_manager->RequestPermissions(NULL, sp_sensor_collection, TRUE);
		//if (FAILED(hr))
		//{
		//}

		if (request.vid_list.size() == 0)
		{
			hr = this->info_manager.Add(sp_sensor, request);
			//hr = this->addSensor(sp_sensor, request);
			if (SUCCEEDED(hr))
			{
				// 接続1発目のデータ取得.
				//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
				return hr;
			}
		}
		else {
			auto device_path = Utility::GetDevicePath(sp_sensor);
			if (device_path)
			{
				if (Utility::StringContains(device_path.value(), request.vid_list))
				{
					hr = this->info_manager.Add(sp_sensor, request);

					//hr = this->addSensor(sp_sensor, request);
					if (SUCCEEDED(hr))
					{
						// 接続1発目のデータ取得.
						//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
						return hr;
					}
				}
			}
		}
	}
	hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	return hr;
}





}