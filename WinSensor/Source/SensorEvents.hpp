#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include "SensorManagerDefine.hpp"

namespace WinSensor {
class SensorEvents : public ISensorEvents
{
public:
	SensorEvents(
		SensorEventCallback_OnDataUpdatedFunction	arg_callback_data_updated,
		SensorEventCallback_OnLeavedFunction		arg_callback_sensor_leaved);
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
private:
	SensorEventCallback_OnDataUpdatedFunction	callback_data_updated;
	SensorEventCallback_OnLeavedFunction		callback_sensor_leaved;

};

}