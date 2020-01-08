

#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")


#include "SensorApiManager.hpp"



SensorApiManager::SensorApiManager()
{
}


SensorApiManager::~SensorApiManager()
{
}

void SensorApiManager::get_accelerometer_3d_value(double& rx, double& ry, double& rz)
{

	rx = 0.f;
	ry = 0.f;
	rz = 0.f;

	CComPtr<ISensorManager> sensor_manager;
	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensor> sensor;
	CComPtr<ISensorDataReport> data;

	// .COMについて要調査
	// 毎回初期化する必要はたぶんない.
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))) 
	{
		return;
	}
	// 毎回マネージャを作成する必要もない？？.
	if (FAILED(::CoCreateInstance(CLSID_SensorManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&sensor_manager)))) 
	{
		return;
	}
	// .
	if (FAILED(sensor_manager->GetSensorsByCategory(SENSOR_TYPE_ACCELEROMETER_3D, &sensor_collection))) 
	{
		return;
	}
	if (FAILED(sensor_collection->GetAt(0, &sensor))) 
	{
		return;
	}
	if (FAILED(sensor->GetData(&data))) 
	{
		return;
	}

	PROPVARIANT x = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, &x))) 
	{
		return;
	}
	PROPVARIANT y = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, &y))) 
	{
		return;
	}
	PROPVARIANT z = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, &z))) 
	{
		return;
	}

	rx = x.dblVal;
	ry = y.dblVal;
	rz = z.dblVal;
}

