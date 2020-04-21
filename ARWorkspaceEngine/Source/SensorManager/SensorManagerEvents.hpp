#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include <atlbase.h>
#include <atlcoll.h>
#include <memory>
#include <string>
#include <optional>
#include "SensorManagerDefine.hpp"
#include "SensorEvents.hpp"
namespace WinSensor {
class SensorManagerEvents : public ISensorManagerEvents
{
public:
	SensorManagerEvents(QuaternionCallbackFunction callback_func);

	//------------ IUnknownInterface.-------------
public:
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(const IID&, void**);
private:
	unsigned long ref_count;

	//------------ ISensorManagerEvents.-------------
public:
	HRESULT __stdcall OnSensorEnter(ISensor* pSensor, SensorState state);
	// ----------------------------------------------
public:
	HRESULT Initialize();
	HRESULT Uninitialize();

	HRESULT AddSensor(REFSENSOR_TYPE_ID sensor_type);
	HRESULT AddSensor(ISensor* pSensor);
	HRESULT RemoveSensor(ISensor* pSensor);
	
private:
	bool initialized = false;

	CComPtr<ISensorManager>			sp_sensor_manager;
	std::unique_ptr<SensorEvents>	sp_sensor_events;
	CAtlMap<SENSOR_ID, ISensor*>	sensor_map;
};

}