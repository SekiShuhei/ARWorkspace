
#include <atlbase.h>
#include "SensorRequest.hpp"
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents()
{
	this->ref_count = 0;
	this->AddRef();

	this->sensor_event_map.reserve(20);
}
ULONG __stdcall SensorManagerEvents::AddRef()
{
	return InterlockedIncrement(&this->ref_count);
}
ULONG __stdcall SensorManagerEvents::Release()
{
	ULONG count = InterlockedDecrement(&this->ref_count);
	if (count == 0)
	{
		delete this;
		return 0;
	}
	return count;
}
HRESULT __stdcall SensorManagerEvents::QueryInterface(const IID& iid, void** ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(ISensorManagerEvents))
	{
		*ppv = static_cast<ISensorManagerEvents*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	this->AddRef();
	return S_OK;
}

HRESULT __stdcall SensorManagerEvents::OnSensorEnter(ISensor* p_sensor, SensorState state)
{
	return S_OK;
}

HRESULT SensorManagerEvents::Initialize()
{
	HRESULT hr;
	hr = this->sp_sensor_manager.CoCreateInstance(CLSID_SensorManager);
	if (SUCCEEDED(hr))
	{
		hr = this->sp_sensor_manager->SetEventSink(this);
		if (SUCCEEDED(hr))
		{
			this->initialized = true;
		}
	}
	return hr;
}

HRESULT SensorManagerEvents::AddSensor(const SensorRequest& request)
{
	
	HRESULT hr;
	CComPtr<ISensorCollection> sp_sensor_collection;
	hr = this->sp_sensor_manager->GetSensorsByType(request.type_id, &sp_sensor_collection);
	
	// ユーザーアクセス許可がない場合.
	//hr = this->sp_sensor_manager->RequestPermissions(NULL, sp_sensor_collection, TRUE);
	//if (FAILED(hr))
	//{
	//	SENSOR_STATUS_DISABLED;
	//}
	
	if (FAILED(hr)) // || sp_sensor_collection == nullptr
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
		if (request.vid_list.size() == 0)
		{
			hr = this->addSensor(sp_sensor, request);
			if (SUCCEEDED(hr))
			{
				// 接続1発目のデータ取得.
				//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
				return hr;
			}
		} else {
			auto device_path = Utility::GetDevicePath(sp_sensor);
			if (device_path)
			{
				if (Utility::StringContains(device_path.value(), request.vid_list))
				{
					hr = this->addSensor(sp_sensor, request);
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
	return hr;
}

HRESULT SensorManagerEvents::Uninitialize()
{
	HRESULT hr = S_OK;

	this->sensor_map.RemoveAll();
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}

HRESULT SensorManagerEvents::addSensor(ISensor* p_sensor, const SensorRequest& request)
{
	if (p_sensor == nullptr) 
	{
		return E_POINTER;
	}
	auto sp_sensor_events = std::make_unique<SensorEvents>(request.callback_func);

	// センサーの生ポインタは別途コンテナで管理したいが危ないので要注意.
	HRESULT hr = S_OK;
	SENSOR_ID sensor_id = GUID_NULL;
	hr = p_sensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		this->sensor_map.Add(p_sensor, request);
	}
	//...
	// 生成したスマポをコンテナに格納して管理.
	this->sensor_event_map.emplace_back(std::move(sp_sensor_events));


	return hr;
}

HRESULT SensorManagerEvents::removeSensor(ISensor* p_sensor)
{
	if (p_sensor == nullptr)
	{
		return E_POINTER;
	}
	
	HRESULT hr = S_OK;
	return hr;
}


}