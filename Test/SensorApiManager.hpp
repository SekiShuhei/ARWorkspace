#pragma once

//#include <atlbase.h>
//#include <SensorsApi.h>
//#include <sensors.h>
//#pragma comment(lib, "Sensorsapi.lib")


class SensorApiManager
{
public:
	SensorApiManager();
	~SensorApiManager();


	void get_accelerometer_3d_value(double& rx, double& ry, double& rz);

private:
	//CComPtr<ISensorManager>		sensor_manager;
	//CComPtr<ISensorCollection>	sensor_collection;
	//CComPtr<ISensor>			sensor;
	//CComPtr<ISensorDataReport>	data;

};

