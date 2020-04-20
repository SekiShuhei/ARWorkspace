
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

HRESULT __stdcall SensorManagerEvents::OnSensorEnter(ISensor* pSensor, SensorState state)
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
			CComPtr<ISensorCollection> spSensors;
			hr = this->sp_sensor_manager->GetSensorsByType(SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION, &spSensors);
			if (SUCCEEDED(hr) && NULL != spSensors)
			{
				ULONG ulCount = 0;
				// �Ƃ肠�����O�ԃZ���T��������.
				//hr = spSensors->GetCount(&ulCount);
				if (SUCCEEDED(hr))
				{
					for (ULONG i = 0; i < 1; i++)
					{
						CComPtr<ISensor> spSensor;
						hr = spSensors->GetAt(i, &spSensor);
						if (SUCCEEDED(hr))
						{
							//if (SUCCEEDED(IsMoverio(spSensor)))
							//{
								hr = this->AddSensor(spSensor);
								//if (SUCCEEDED(hr))
								//{
									///////// 
									//hr = this->sp_sensor_events->GetSensorData(spSensor);
								//}
							//}
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
		ISensor* pSensor = this->sensor_map.GetNextValue(pos);
		this->RemoveSensor(pSensor);
	}
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}

HRESULT SensorManagerEvents::AddSensor(ISensor* pSensor)
{
	if (pSensor == nullptr) 
	{
		return E_POINTER;
	}
	HRESULT hr = S_OK;
	hr = pSensor->SetEventSink(this->sp_sensor_events.get());
	SENSOR_ID sensor_id = GUID_NULL;
	hr = pSensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		SENSOR_ID sensor_id = GUID_NULL;
		hr = pSensor->GetID(&sensor_id);
		if (SUCCEEDED(hr))
		{
			pSensor->AddRef();
			this->sensor_map[sensor_id] = pSensor;
		}
	}
	//...

	return hr;
}

HRESULT SensorManagerEvents::RemoveSensor(ISensor* pSensor)
{
	if (pSensor == nullptr)
	{
		return E_POINTER;
	}
	HRESULT hr = S_OK;
	hr = pSensor->SetEventSink(NULL);
	SENSOR_ID sensor_id = GUID_NULL;
	hr = pSensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		this->sensor_map.RemoveKey(sensor_id);
		pSensor->Release();
	}
}

}