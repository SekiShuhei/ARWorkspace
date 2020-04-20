#include "SensorEvents.hpp"

namespace WinSensor {
SensorEvents::SensorEvents()
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

HRESULT __stdcall SensorEvents::OnDataUpdated(ISensor* p_sensor, ISensorDataReport* pNewData)
{
	HRESULT hr = S_OK;
	if (nullptr == pNewData || nullptr == p_sensor)
	{
		return E_INVALIDARG;
	}
	PROPVARIANT pv_quaternion = {};
	PROPVARIANT pv_time_stamp = {};
	hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_QUATERNION, &pv_quaternion);
	if (SUCCEEDED(hr))
	{
		hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_TIMESTAMP, &pv_time_stamp);
		if (SUCCEEDED(hr))
		{
			UINT64 timestamp = ((UINT64)pv_time_stamp.filetime.dwHighDateTime << 32) | 
				pv_time_stamp.filetime.dwLowDateTime;
			if (pv_quaternion.vt == (VT_VECTOR | VT_UI1)) 
			{
				float* pElement = (float*)pv_quaternion.caub.pElems;
				//m_Callback(pElement[0], pElement[1], pElement[2], pElement[3], timestamp);
			}
		}
	}
	PropVariantClear(&pv_quaternion);
	PropVariantClear(&pv_time_stamp);
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
}

}