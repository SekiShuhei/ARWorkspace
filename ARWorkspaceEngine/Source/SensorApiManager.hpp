#pragma once


#include <atlbase.h>
#include <SensorsApi.h>
#include <sensors.h>
#pragma comment(lib, "Sensorsapi.lib")

namespace ARWorkspace {

class SensorApiManager
{
public:
	SensorApiManager();
	~SensorApiManager();
	bool Initialize();

	// éQçlà¯óp.
	void GetAccelerometerSensorData(double& rx, double& ry, double& rz);

	void GetGyrometerSensorData(double& ref_x, double& ref_y, double& ref_z);

private:
	bool selectSensor(REFSENSOR_CATEGORY_ID arg_sensor_category_id); 

private:
	CComPtr<ISensorManager>		p_sensor_manager;
	CComPtr<ISensorCollection>	p_sensor_collection;
	CComPtr<ISensor>			p_current_sensor;
	CComPtr<ISensorDataReport>	p_data;

	bool intialized = false;
};

}