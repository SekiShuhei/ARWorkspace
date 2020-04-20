#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include <memory>
#include <atlbase.h>

namespace WinSensor {
class SensorManagerEvents : public ISensorManagerEvents
{
public:
	SensorManagerEvents()
	{
		this->ref_count = 0;
		this->AddRef();
	}

	ULONG __stdcall AddRef()
	{
		return ++this->ref_count;
	}

	ULONG __stdcall Release()
	{
		return --this->ref_count;
	}
	HRESULT __stdcall QueryInterface(const IID& id, void** p)
	{
		if (id == IID_IUnknown || id == __uuidof(ISensorManagerEvents))
		{
			*p = this;
			this->AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	HRESULT __stdcall OnSensorEnter(ISensor* pSensor, SensorState state)
	{
		return S_OK;
	}
private:
	unsigned long ref_count;

public:
	HRESULT Initialize();
	HRESULT Uninitialize();

private:
	CComPtr<ISensorManager> sp_sensor_manager;
	//std::unique_ptr<AggregatedDeviceOrientationSensorEvents> m_pSensorEvents; // Sensor Events class used for event sinking

};

}