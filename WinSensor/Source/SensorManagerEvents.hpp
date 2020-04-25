#pragma once

#include <atlbase.h>
#include <memory>
#include <string>
#include <optional>
#include <map>
#include "SensorManagerDefine.hpp"
#include "SensorEvents.hpp"
#include "SensorRequest.hpp"
#include "SensorControlManager.hpp"
namespace WinSensor {
class SensorManagerEvents : public ISensorManagerEvents
{
public:
	SensorManagerEvents() = delete;
	SensorManagerEvents(SensorManagerEventsCallbackFunction arg_callback_func);
	//------------ IUnknownInterface.-------------
public:
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall QueryInterface(const IID&, void**);
private:
	unsigned long ref_count;

	//------------ ISensorManagerEvents.-------------
public:
	HRESULT __stdcall OnSensorEnter(__RPC__in_opt ISensor* pSensor, SensorState state);
	// ----------------------------------------------
private:
	SensorManagerEventsCallbackFunction callback_func;
};

}