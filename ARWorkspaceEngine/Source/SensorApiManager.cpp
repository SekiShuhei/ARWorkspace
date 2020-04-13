
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
	// CComPtrをそのまま渡すと落ちる.
	auto result = ::CoCreateInstance(CLSID_SensorManager, 
		NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->p_sensor_manager.p));
	if (FAILED(result))
	{
		return false;
	}
	if (result == HRESULT_FROM_WIN32(ERROR_ACCESS_DISABLED_BY_POLICY))
	{
		return false;
	}
	

	this->intialized = true;

	return true;
}

std::optional<Vector3> SensorApiManager::GetAccelerometerSensorData()
{
	CComPtr<ISensorCollection> sensor_collection;
	CComPtr<ISensorDataReport> data;

	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensor(SENSOR_TYPE_ACCELEROMETER_3D))
	{
		return std::nullopt;
	}
	if (FAILED(this->p_current_sensor->GetData(&data)))
	{
		return std::nullopt;
	}
	// 以下はセンサー種類によってデータが異なる.
	std::tuple<double, double, double> report_value;
	PROPVARIANT value = {};
	if (! FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, &value)))
	{
		if (value.vt == VT_R8)
		{
			std::get<0>(report_value) = value.dblVal;
		}
	}
	PropVariantClear(&value);
	if (! FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, &value)))
	{
		if (value.vt == VT_R8)
		{
			std::get<1>(report_value) = value.dblVal;
		}
	}
	PropVariantClear(&value);
	if (! FAILED(data->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, &value)))
	{
		if (value.vt == VT_R8)
		{
			std::get<2>(report_value) = value.dblVal;
		}
	}
	return report_value;
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
	// 毎回マネージャを作成する必要もない？？.
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

bool SensorApiManager::selectSensor(const REFSENSOR_CATEGORY_ID arg_sensor_category_id)
{
	// とりあえず先頭のセンサを見つけるだけ.
	CComPtr<ISensorCollection> sensor_collection;
	if (FAILED(p_sensor_manager->GetSensorsByCategory(arg_sensor_category_id, &sensor_collection)))
	{
		return false;
	}
	if (FAILED(sensor_collection->GetAt(0, &this->p_current_sensor.p)))
	{
		return false;
	}
	ULONG ulCount = 0;
	if (FAILED(sensor_collection->GetCount(&ulCount)))
	{
		return false;
	}
	if (FAILED(this->p_current_sensor->SetEventSink(NULL)))
	{
		return false;
	}

	return true;
}

double SensorApiManager::getCurrentSensorValue(const PROPERTYKEY arg_property_key)
{
	if (this->p_current_sensor.IsEqualObject(nullptr))
	{
		return 0.0;
	}
	CComPtr<ISensorDataReport> data;
	if (FAILED(this->p_current_sensor->GetData(&data)))
	{
		return 0.0;
	}
	PROPVARIANT value = {};
	if (FAILED(data->GetSensorValue(arg_property_key, &value)))
	{
		return 0.0;
	}

	if (value.vt == VT_R8)
	{
		return value.dblVal;
	}
}

}