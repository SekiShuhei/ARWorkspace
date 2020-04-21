
#include <atlbase.h>
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents(QuaternionCallbackFunction callback_func)
{
	this->ref_count = 0;
	this->AddRef();

	this->sp_sensor_events = std::make_unique<SensorEvents>(callback_func);

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

HRESULT SensorManagerEvents::AddSensor(REFSENSOR_TYPE_ID sensor_type)
{
	std::vector<std::wstring> vid_list;
	vid_list.emplace_back(L"VID_0483"); // BT-35E
	vid_list.emplace_back(L"VID_04B8"); // BT-30C


	HRESULT hr;
	CComPtr<ISensorCollection> sp_sensor_collection;
	hr = this->sp_sensor_manager->GetSensorsByType(sensor_type, &sp_sensor_collection);
	
	//hr = this->sp_sensor_manager->RequestPermissions(NULL, sp_sensor_collection, TRUE);
	//if (FAILED(hr))
	//{
	//	SENSOR_STATUS_DISABLED;
	//}
	
	if (SUCCEEDED(hr) && NULL != sp_sensor_collection)
	{
		ULONG sensor_count = 0;
		// とりあえず０番センサだけ見る.
		hr = sp_sensor_collection->GetCount(&sensor_count);
		if (SUCCEEDED(hr))
		{
			for (ULONG i = 0; i < sensor_count; i++)
			{
				CComPtr<ISensor> sp_sensor;
				hr = sp_sensor_collection->GetAt(i, &sp_sensor);
				if (SUCCEEDED(hr))
				{
					//...デバイス判定処理をここに追加
					auto device_path = Utility::GetDevicePath(sp_sensor);
					if (device_path)
					{
						if (Utility::StringContains(device_path.value(), vid_list))
						{
							hr = this->addSensor(sp_sensor);
							if (SUCCEEDED(hr))
							{
								// 接続1発目のデータ取得.
								//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
								break;
							}

						}
					}
					
				}
			}
		}
	}
	return hr;

}

HRESULT SensorManagerEvents::Uninitialize()
{
	HRESULT hr;

	POSITION pos = this->sensor_map.GetStartPosition();
	while (NULL != pos)
	{
		ISensor* p_sensor = this->sensor_map.GetNextValue(pos);
		this->removeSensor(p_sensor);
	}
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}

HRESULT SensorManagerEvents::addSensor(ISensor* p_sensor)
{
	if (p_sensor == nullptr) 
	{
		return E_POINTER;
	}
	HRESULT hr = S_OK;
	hr = p_sensor->SetEventSink(this->sp_sensor_events.get());
	SENSOR_ID sensor_id = GUID_NULL;
	hr = p_sensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		
		p_sensor->AddRef();
		this->sensor_map[sensor_id] = p_sensor;
	}
	//...

	return hr;
}

HRESULT SensorManagerEvents::removeSensor(ISensor* p_sensor)
{
	if (p_sensor == nullptr)
	{
		return E_POINTER;
	}
	HRESULT hr = S_OK;
	hr = p_sensor->SetEventSink(NULL);
	SENSOR_ID sensor_id = GUID_NULL;
	hr = p_sensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		this->sensor_map.RemoveKey(sensor_id);
		p_sensor->Release();
	}
	return hr;
}


}