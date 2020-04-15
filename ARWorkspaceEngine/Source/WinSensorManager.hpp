#pragma once


#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")

#include <tuple>
#include <optional>

namespace ARWorkspace {

using Vector3 = std::tuple<double, double, double>;
using Quaternion = std::tuple<double, double, double, double>;
class WinSensorManager
{
public:
	WinSensorManager();
	~WinSensorManager();
	bool Initialize();

	std::optional<Vector3>		GetAccelerometerData();
	std::optional<Vector3>		GetCompassData();
	std::optional<Vector3>		GetGyrometerData();
	std::optional<float>		GetAmbientLightData();
	std::optional<Vector3>		GetGravityVectorData();
	std::optional<Vector3>		GetLinearAccelerometerData();
	std::optional<Quaternion>	GetAggregatedDeviceOrientationData();

private:
	bool selectSensorByCategory(const REFSENSOR_CATEGORY_ID arg_sensor_category_id);
	bool selectSensorByType(const REFSENSOR_CATEGORY_ID arg_sensor_type_id);

	template<typename T>
	T getCurrentSensorValue(const PROPERTYKEY arg_property_key);
	template<>
	double getCurrentSensorValue<double>(const PROPERTYKEY arg_property_key);
	template<>
	float getCurrentSensorValue<float>(const PROPERTYKEY arg_property_key);


	bool getData(PROPVARIANT& ref_propvariant ,const PROPERTYKEY arg_property_key);

private:
	CComPtr<ISensorManager>		p_sensor_manager;
	CComPtr<ISensor>			p_current_sensor;
	
	bool intialized = false;
};

}