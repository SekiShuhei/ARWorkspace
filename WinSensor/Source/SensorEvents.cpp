
#include <atlbase.h>
#include "DataReporterQuaternion.hpp"
#include "SensorEvents.hpp"

namespace WinSensor {
SensorEvents::SensorEvents(
	SensorEventCallback_OnDataUpdatedFunction arg_callback_data_updated, 
	SensorEventCallback_OnLeavedFunction arg_callback_sensor_leaved) :
	callback_data_updated(arg_callback_data_updated),
	callback_sensor_leaved(arg_callback_sensor_leaved)
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

HRESULT __stdcall SensorEvents::OnEvent(__RPC__in_opt ISensor* p_sensor, REFGUID eventID, IPortableDeviceValues* pEventData)
{
	return S_OK;
}

HRESULT __stdcall SensorEvents::OnDataUpdated(__RPC__in_opt ISensor* p_sensor, ISensorDataReport* p_data)
{
	HRESULT hr = S_OK;
	if (nullptr == p_data || nullptr == p_sensor)
	{
		return E_INVALIDARG;
	}
	hr = this->callback_data_updated(p_sensor, p_data);
	return hr;
}

HRESULT __stdcall SensorEvents::OnLeave(__RPC__in REFSENSOR_ID sensor_id)
{
	this->callback_sensor_leaved(sensor_id);
	return S_OK;
}

HRESULT __stdcall SensorEvents::OnStateChanged(__RPC__in_opt ISensor* p_sensor, SensorState state)
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
			CComPtr<ISensorDataReport> sp_data;
			hr = p_sensor->GetData(&sp_data);
			if (SUCCEEDED(hr))
			{
				hr = this->callback_data_updated(p_sensor, sp_data);
			}
		}
		else if (state == SENSOR_STATE_ACCESS_DENIED)
		{
			//...
		}
	}
	return hr;
}

}