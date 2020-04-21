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

using Float4AndTimestamp = std::tuple<float, float, float, float, unsigned __int64>;

using SensorEventCallbackFunction = std::function<bool(ISensor*, ISensorDataReport*)>;
using QuaternionCallbackFunction = std::function<bool(Float4AndTimestamp)>;

}