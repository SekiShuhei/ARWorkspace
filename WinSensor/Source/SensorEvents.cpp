
#include "DataReporterQuaternion.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
SensorEvents::SensorEvents(SensorEventCallbackFunction arg_callback_func) :
	callback_func(arg_callback_func)
{
	this->ref_count = 0;
	this->AddRef();

}
ULONG __stdcall SensorEvents::AddRef()
{
	return InterlockedIncrement(&this->ref_count);
}
ULONG __stdcall SensorEvents::Release()
{
	ULONG count = InterlockedDecrement(&this->ref_count);
	if (count == 0)
	{
		delete this;
		return 0;
	}
	return count;
}
HRESULT __stdcall SensorEvents::QueryInterface(const IID& iid, void** ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(ISensorEvents))
	{
		*ppv = static_cast<ISensorEvents*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	this->AddRef();
	return S_OK;
}

HRESULT __stdcall SensorEvents::OnEvent(ISensor* p_sensor, REFGUID eventID, IPortableDeviceValues* pEventData)
{
	return S_OK;
}

HRESULT __stdcall SensorEvents::OnDataUpdated(ISensor* p_sensor, ISensorDataReport* p_data)
{
	HRESULT hr = S_OK;
	if (nullptr == p_data || nullptr == p_sensor)
	{
		return E_INVALIDARG;
	}
	hr = this->callback_func(p_sensor, p_data);
	return hr;
}

HRESULT __stdcall SensorEvents::OnLeave(REFSENSOR_ID sensorID)
{
	return S_OK;
}

HRESULT __stdcall SensorEvents::OnStateChanged(ISensor* p_sensor, SensorState state)
{
	HRESULT hr = S_OK;
	if (nullptr == p_sensor)
	{
		return E_INVALIDARG;
	}
	SENSOR_ID sensor_id = GUID_NULL;
	hr = p_sensor->GetID(&sensor_id);
	if (SUCCEEDED(hr))
	{
		if (state == SENSOR_STATE_READY)
		{
			//hr = this->GetSensorData(p_sensor);
		}
		else if (state == SENSOR_STATE_ACCESS_DENIED)
		{	
			//wprintf_s(L"\nNo permission for the time sensor.\n");
			//wprintf_s(L"Enable the sensor in the control panel.\n");
		}
	}
	return hr;
}

}