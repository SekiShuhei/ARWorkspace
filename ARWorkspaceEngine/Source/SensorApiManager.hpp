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

	void GetGyrometerSensorData(double& ref_x, double& ref_y, double& ref_z);

private:
	bool selectSensor(const REFSENSOR_CATEGORY_ID arg_sensor_category_id); 
	double getCurrentSensorValue(const PROPERTYKEY arg_property_key);

private:
	CComPtr<ISensorManager>		p_sensor_manager;
	CComPtr<ISensorCollection>	p_sensor_collection;
	CComPtr<ISensor>			p_current_sensor;
	CComPtr<ISensorDataReport>	p_data;

	bool intialized = false;
};

}