
#include <atlbase.h>
#include "SensorRequest.hpp"
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents(SensorManagerEventsCallbackFunction arg_callback_func) :
	callback_func(arg_callback_func)
{
	this->ref_count = 0;
	this->AddRef();
}

ULONG __stdcall SensorManagerEvents::AddRef()
{
	return InterlockedIncrement(&this->ref_count);
}
ULONG __stdcall SensorManagerEvents::Release()
{
	ULONG count = InterlockedDecrement(&this->ref_count);
	if (count == 0)
	{
		delete this;
		return 0;
	}
	return count;
}
HRESULT __stdcall SensorManagerEvents::QueryInterface(const IID& iid, void** ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(ISensorManagerEvents))
	{
		*ppv = static_cast<ISensorManagerEvents*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	this->AddRef();
	return S_OK;
}

HRESULT __stdcall SensorManagerEvents::OnSensorEnter(__RPC__in_opt ISensor* p_sensor, SensorState state)
{
	this->callback_func(p_sensor, state);
	return S_OK;
}

}