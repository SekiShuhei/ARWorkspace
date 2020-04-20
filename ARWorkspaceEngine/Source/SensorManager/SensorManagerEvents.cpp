
#include <atlbase.h>
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents()
{
	this->ref_count = 0;
	this->AddRef();

	this->sp_sensor_events = std::make_unique<SensorEvents>();

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
				// ‚Æ‚è‚ ‚¦‚¸‚O”ÔƒZƒ“ƒT‚¾‚¯Œ©‚é.
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
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}

HRESULT SensorManagerEvents::AddSensor(ISensor* pSensor)
{
	HRESULT hr = S_OK;
	if (nullptr == pSensor)
	{
		hr = E_POINTER;
	}
	hr = pSensor->SetEventSink(this->sp_sensor_events.get());
	SENSOR_ID idSensor = GUID_NULL;
	hr = pSensor->GetID(&idSensor);
	if (SUCCEEDED(hr))
	{
		pSensor->AddRef();
		//this->sensor_map[idSensor] = pSensor;
	}
	//...

	return hr;
}

}