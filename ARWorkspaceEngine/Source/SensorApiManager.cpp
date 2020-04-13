

#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")


#include "SensorApiManager.hpp"

namespace ARWorkspace {

SensorApiManager::SensorApiManager()
{
}


SensorApiManager::~SensorApiManager()
{
}

void SensorApiManager::GetAccelerometerSensorData(double& rx, double& ry, double& rz)
{

	rx = 0.f;
	ry = 0.f;
	rz = 0.f;

	CComPtr<ISensorManager> sensor_manager;
	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensor> sensor;
	CComPtr<ISensorDataReport> data;

	// .COM�ɂ��ėv����
	// ���񏉊�������K�v�͂��Ԃ�Ȃ�.
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))) 
	{
		return;
	}
	// ����}�l�[�W�����쐬����K�v���Ȃ��H�H.
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

void SensorApiManager::GetGyrometerSensorData(double& ref_x, double& ref_y, double& ref_z)
{
	ref_x = 0.f;
	ref_y = 0.f;
	ref_z = 0.f;

	CComPtr<ISensorManager> sensor_manager;
	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensor> sensor;
	CComPtr<ISensorDataReport> data;

	// .COM�ɂ��ėv����
	// ���񏉊�������K�v�͂��Ԃ�Ȃ�.
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return;
	}
	// ����}�l�[�W�����쐬����K�v���Ȃ��H�H.
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
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_X_DEGREES_PER_SECOND_SQUARED, &x)))
	{
		return;
	}
	PROPVARIANT y = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_Y_DEGREES_PER_SECOND_SQUARED, &y)))
	{
		return;
	}
	PROPVARIANT z = {};
	if (FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_Z_DEGREES_PER_SECOND_SQUARED, &z)))
	{
		return;
	}

	ref_x = x.dblVal;
	ref_y = y.dblVal;
	ref_z = z.dblVal;
}

}