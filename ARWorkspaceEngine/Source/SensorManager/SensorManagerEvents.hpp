#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include <memory>
#include <atlbase.h>

#include "SensorEvents.hpp"
namespace WinSensor {
class SensorManagerEvents : public ISensorManagerEvents
{
public:
	SensorManagerEvents();

	//------------ IUnknownInterface.-------------
public:
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(const IID&, void**);
private:
	unsigned long ref_count;

	//------------ ISensorManagerEvents.-------------
public:
	HRESULT __stdcall OnSensorEnter(ISensor* pSensor, SensorState state)
	{
		return S_OK;
	}
	// ----------------------------------------------
public:
	HRESULT Initialize();
	HRESULT Uninitialize();

private:
	CComPtr<ISensorManager>			sp_sensor_manager;
	std::unique_ptr<SensorEvents>	sp_sensor_events;

};

}