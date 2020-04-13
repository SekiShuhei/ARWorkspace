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
	CComPtr<ISensorManager>		p_sensor_manager;
	CComPtr<ISensorCollection>	p_sensor_collection;
	CComPtr<ISensor>			p_sensor;
	CComPtr<ISensorDataReport>	p_data;

	bool intialized = false;
};

}