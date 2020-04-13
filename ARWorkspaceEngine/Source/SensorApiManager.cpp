
#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")
#include <initguid.h>


#include "SensorApiManager.hpp"

namespace ARWorkspace {

SensorApiManager::SensorApiManager()
{
	
}


SensorApiManager::~SensorApiManager()
{
}

bool SensorApiManager::Initialize()
{
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return false;
	}
	// CComPtr�����̂܂ܓn���Ɨ�����.
	if (FAILED(::CoCreateInstance(CLSID_SensorManager, 
		NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->p_sensor_manager.p))))
	{
		return false;
	}

	this->intialized = true;

	return true;
}

void SensorApiManager::GetAccelerometerSensorData(double& rx, double& ry, double& rz)
{

	rx = 0.f;
	ry = 0.f;
	rz = 0.f;

	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensor> sensor;
	CComPtr<ISensorDataReport> data;

	if (! this->intialized)
	{
		return;
	}

	if (FAILED(p_sensor_manager->GetSensorsByCategory(SENSOR_TYPE_ACCELEROMETER_3D, &sensor_collection))) 
	{
		return;
	}
	if (FAILED(sensor_collection->GetAt(0, &sensor))) 
	{
		return;
	}
	ULONG ulCount = 0;
	if (FAILED(sensor_collection->GetCount(&ulCount)))
	{
		return;
	}
	if (FAILED(sensor->SetEventSink(NULL)))
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

void SensorApiManager::GetGyrometerSensorData(double& ref_x, double& ref_y, double& ref_z)
{
	return; //kari.

	ref_x = 0.f;
	ref_y = 0.f;
	ref_z = 0.f;

	CComPtr<ISensorManager> p_sensor_manager;
	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensor> sensor;
	CComPtr<ISensorDataReport> data;

	//if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	//{
	//	return;
	//}
	// ����}�l�[�W�����쐬����K�v���Ȃ��H�H.
	if (FAILED(::CoCreateInstance(CLSID_SensorManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&p_sensor_manager))))
	{
		return;
	}
	// .
	if (FAILED(p_sensor_manager->GetSensorsByCategory(SENSOR_TYPE_GYROMETER_3D, &sensor_collection)))
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
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND, &x)))
	{
		return;
	}
	PROPVARIANT y = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND, &y)))
	{
		return;
	}
	PROPVARIANT z = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND, &z)))
	{
		return;
	}

	ref_x = x.dblVal;
	ref_y = y.dblVal;
	ref_z = z.dblVal;
}

}