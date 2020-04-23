#pragma once

// Sensor.
#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")

// Std.
#include <tuple>
#include <functional>
#include <string>
// WinSensor
#include "SensorUtility.hpp"

namespace WinSensor {

using FloatAndTimestamp = std::tuple<float, unsigned __int64>;
using Double3AndTimestamp = std::tuple<double, double, double, unsigned __int64>;
using Float4AndTimestamp = std::tuple<float, float, float, float, unsigned __int64>;

using SensorEventCallbackFunction = std::function<HRESULT(ISensor*, ISensorDataReport*)>;

}