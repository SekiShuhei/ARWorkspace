#pragma once

namespace WinSensor {
enum class SensorType
{
	None = 0,
	Default = 0,
	AggregatedDeviceOrientation,
	/// ������.
	AmbientLight,
	/// ������...
	Accelerometer,
	Compass,
	Gyrometer,
	GravityVector,
	LinearAccelerometer,
	//...
};

}