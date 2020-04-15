
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

std::optional<Vector3> WinSensorManager::GetAccelerometerData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByCategory(SENSOR_TYPE_ACCELEROMETER_3D))
	{
		return std::nullopt;
	}
	return Vector3(
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_X_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Y_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Z_G));
}

std::optional<Vector3> WinSensorManager::GetCompassData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByCategory(SENSOR_TYPE_COMPASS_3D))
	{
		return std::nullopt;
	}
	return Vector3(
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_X_MILLIGAUSS),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Y_MILLIGAUSS),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Z_MILLIGAUSS));
}

std::optional<Vector3> WinSensorManager::GetGyrometerData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByCategory(SENSOR_TYPE_GYROMETER_3D))
	{
		return std::nullopt;
	}

	return Vector3(
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND));
}

std::optional<float> WinSensorManager::GetAmbientLightData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByCategory(SENSOR_TYPE_AMBIENT_LIGHT))
	{
		return std::nullopt;
	}
	return this->getCurrentSensorValue<float>(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX);
}

std::optional<Vector3> WinSensorManager::GetGravityVectorData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (! this->selectSensorByType(GUID_SensorType_GravityVector))
	{
		return std::nullopt;
	}

	return Vector3(
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_X_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Y_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Z_G));
}

std::optional<Vector3> WinSensorManager::GetLinearAccelerometerData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByType(GUID_SensorType_LinearAccelerometer))
	{
		return std::nullopt;
	}

	return Vector3(
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_X_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Y_G),
		this->getCurrentSensorValue<double>(SENSOR_DATA_TYPE_ACCELERATION_Z_G));
}

std::optional<Quaternion> WinSensorManager::GetAggregatedDeviceOrientationData()
{
	if (!this->intialized)
	{
		return std::nullopt;
	}
	if (!this->selectSensorByCategory(SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION))
	{
		return std::nullopt;
	}

	return this->getCurrentSensorValue<Quaternion>(SENSOR_DATA_TYPE_QUATERNION);
}

bool WinSensorManager::selectSensorByCategory(const REFSENSOR_CATEGORY_ID arg_sensor_category_id)
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

bool WinSensorManager::selectSensorByType(const REFSENSOR_CATEGORY_ID arg_sensor_category_id)
{
	// とりあえず先頭のセンサを見つけるだけ.
	CComPtr<ISensorCollection> sensor_collection;
	if (FAILED(p_sensor_manager->GetSensorsByType(arg_sensor_category_id, &sensor_collection)))
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

template<typename T>
T WinSensorManager::getCurrentSensorValue(const PROPERTYKEY arg_property_key)
{
	assert(0);
	return T;
}
template<>
double WinSensorManager::getCurrentSensorValue<double>(const PROPERTYKEY arg_property_key)
{
	PROPVARIANT value = {};
	if (!this->getData(value, arg_property_key))
	{
		return 0.0;
	}
	if (value.vt == VT_R8)
	{
		return value.dblVal;
	}
}
template<>
float WinSensorManager::getCurrentSensorValue<float>(const PROPERTYKEY arg_property_key)
{
	PROPVARIANT value = {};
	if (!this->getData(value, arg_property_key))
	{
		return 0.0;
	}
	if (value.vt == VT_R4)
	{
		return value.fltVal;
	}
}
template<>
Quaternion WinSensorManager::getCurrentSensorValue<Quaternion>(const PROPERTYKEY arg_property_key)
{
	PROPVARIANT quaternion = {};
	if (!this->getData(quaternion, arg_property_key))
	{
		return Quaternion();
	}
	if (quaternion.vt == (VT_VECTOR | VT_UI1)) 
	{
		const auto pElement = (float*)quaternion.caub.pElems;
		return Quaternion(pElement[0], pElement[1], pElement[2], pElement[3]);
	}
}

bool WinSensorManager::getData(PROPVARIANT& ref_propvariant, const PROPERTYKEY arg_property_key)
{
	if (this->p_current_sensor.IsEqualObject(nullptr))
	{
		return false;
	}
	CComPtr<ISensorDataReport> data;
	if (FAILED(this->p_current_sensor->GetData(&data)))
	{
		return false;
	}
	if (FAILED(data->GetSensorValue(arg_property_key, &ref_propvariant)))
	{
		return false;
	}
	return true;
}

}