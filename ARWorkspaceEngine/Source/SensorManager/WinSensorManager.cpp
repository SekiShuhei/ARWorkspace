
#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")
#include <sensorsdef.h>
#include <initguid.h>
#include <cassert>

#include "WinSensorManager.hpp"

namespace ARWorkspace {

WinSensorManager::WinSensorManager()
{
	
}


WinSensorManager::~WinSensorManager()
{
}

bool WinSensorManager::Initialize()
{
	//if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	//{
	//	return false;
	//}
	//// CComPtr�����̂܂ܓn���Ɨ�����.
	//auto result = ::CoCreateInstance(CLSID_SensorManager, 
	//	NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->p_sensor_manager.p));
	//if (FAILED(result))
	//{
	//	return false;
	//}
	//if (result == HRESULT_FROM_WIN32(ERROR_ACCESS_DISABLED_BY_POLICY))
	//{
	//	return false;
	//}
	//
	//
	//this->intialized = true;

	return true;



}

std::optional<Quaternion> WinSensorManager::GetAggregatedDeviceOrientationData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	return std::nullopt;
}


}