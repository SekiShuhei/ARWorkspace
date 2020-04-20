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

	//std::optional<Vector3>		GetAccelerometerData();
	//std::optional<Vector3>		GetCompassData();
	//std::optional<Vector3>		GetGyrometerData();
	//std::optional<float>		GetAmbientLightData();
	//std::optional<Vector3>		GetGravityVectorData();
	//std::optional<Vector3>		GetLinearAccelerometerData();
	std::optional<Quaternion>	GetAggregatedDeviceOrientationData();

private:
	
private:
	
	bool intialized = false;
};

}