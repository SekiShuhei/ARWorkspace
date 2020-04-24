#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include "SensorManagerDefine.hpp"

namespace WinSensor {
class SensorEvents : public ISensorEvents
{
public:
	SensorEvents(SensorEventCallbackFunction callback_func);
	// -------------IUnknownInterface.-------------
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(const IID&, void**);
private:
	unsigned long ref_count;


	// -------------ISensorEvents.-------------
public:
	HRESULT __stdcall OnEvent(__RPC__in_opt ISensor* p_sensor, REFGUID eventID, IPortableDeviceValues* p_event_data);
	HRESULT __stdcall OnDataUpdated(__RPC__in_opt ISensor* p_sensor, ISensorDataReport* p_data);
	HRESULT __stdcall OnLeave(__RPC__in REFSENSOR_ID sensorID);
	HRESULT __stdcall OnStateChanged(__RPC__in_opt ISensor* p_sensor, SensorState state);
	// ---------------------------------------
public:

private:
	SensorEventCallbackFunction callback_func;
};

}