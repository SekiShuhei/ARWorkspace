#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")

namespace WinSensor {
class SensorEvents : public ISensorEvents
{
public:
	SensorEvents();
	// -------------IUnknownInterface.-------------
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(const IID&, void**);
private:
	unsigned long ref_count;


	// -------------ISensorEvents.-------------
public:
	HRESULT __stdcall OnEvent(ISensor* pSensor, REFGUID eventID, IPortableDeviceValues* pEventData);
	HRESULT __stdcall OnDataUpdated(ISensor* pSensor, ISensorDataReport* pNewData);
	HRESULT __stdcall OnLeave(REFSENSOR_ID sensorID);
	HRESULT __stdcall OnStateChanged(ISensor* pSensor, SensorState state);
	// ---------------------------------------
public:

};

}