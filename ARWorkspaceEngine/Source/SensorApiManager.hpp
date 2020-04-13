#pragma once


#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")

#include <tuple>
#include <optional>

namespace ARWorkspace {

using Vector3 = std::tuple<double, double, double>;
class SensorApiManager
{
public:
	SensorApiManager();
	~SensorApiManager();
	bool Initialize();

	std::optional<Vector3> GetAccelerometerSensorData();
	std::optional<Vector3> GetGyrometerSensorData();

private:
	bool selectSensor(const REFSENSOR_CATEGORY_ID arg_sensor_category_id); 
	double getCurrentSensorValue(const PROPERTYKEY arg_property_key);

private:
	CComPtr<ISensorManager>		p_sensor_manager;
	CComPtr<ISensor>			p_current_sensor;
	
	bool intialized = false;
};

}