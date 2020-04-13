#pragma once

//#include <initguid.h>

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

	// éQçlà¯óp.
	void GetAccelerometerSensorData(double& rx, double& ry, double& rz);

	void GetGyrometerSensorData(double& ref_x, double& ref_y, double& ref_z);


private:
	CComPtr<ISensorManager>		sensor_manager;
	CComPtr<ISensorCollection>	sensor_collection;
	CComPtr<ISensor>			sensor;
	CComPtr<ISensorDataReport>	data;

	bool intialized = false;
};

}