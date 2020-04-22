#pragma once

#include <atlbase.h>
#include <atlcoll.h>
#include <memory>
#include <string>
#include <optional>
#include <map>
#include "SensorManagerDefine.hpp"
#include "SensorEvents.hpp"
#include "SensorRequest.hpp"
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
	HRESULT __stdcall OnSensorEnter(ISensor* pSensor, SensorState state);
	// ----------------------------------------------
public:
	HRESULT Initialize();
	HRESULT Uninitialize();

	HRESULT AddSensor(const SensorRequest& request);
	
private:
	
	HRESULT addSensor(ISensor* pSensor, const SensorRequest& request);
	HRESULT removeSensor(ISensor* pSensor);


private:
	bool initialized = false;

	CComPtr<ISensorManager>			sp_sensor_manager;
	std::unique_ptr<SensorEvents>	sp_sensor_events;
	CAtlMap<SENSOR_ID, ISensor*>	sensor_map;

	//std::map<SENSOR_ID, SensorEvents*> sensor_event_map;
	std::map<SENSOR_ID, std::shared_ptr<SensorEvents>> sensor_event_map;
};

}