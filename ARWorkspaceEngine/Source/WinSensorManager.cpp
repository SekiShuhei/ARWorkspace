
#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")
#include <initguid.h>


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
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return false;
	}
	// CComPtrÇÇªÇÃÇ‹Ç‹ìnÇ∑Ç∆óéÇøÇÈ.
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

std::optional<Vector3> WinSensorManager::GetAccelerometerSensorData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensor(SENSOR_TYPE_ACCELEROMETER_3D))
	{
		return std::nullopt;
	}
	Vector3 report_value;
	std::get<0>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G);
	std::get<1>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G);
	std::get<2>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G);

	return report_value;
}

std::optional<Vector3> WinSensorManager::GetGyrometerSensorData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensor(SENSOR_TYPE_GYROMETER_3D))
	{
		return std::nullopt;
	}
	Vector3 report_value;
	std::get<0>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND);
	std::get<1>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND);
	std::get<2>(report_value) = this->getCurrentSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND);

	return report_value;

}

bool WinSensorManager::selectSensor(const REFSENSOR_CATEGORY_ID arg_sensor_category_id)
{
	// Ç∆ÇËÇ†Ç¶Ç∏êÊì™ÇÃÉZÉìÉTÇå©Ç¬ÇØÇÈÇæÇØ.
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

double WinSensorManager::getCurrentSensorValue(const PROPERTYKEY arg_property_key)
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