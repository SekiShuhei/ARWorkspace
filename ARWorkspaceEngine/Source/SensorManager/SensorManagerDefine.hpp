#pragma once

// Sensor.
#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")

// Std.
#include <tuple>
#include <functional>

namespace WinSensor {

using Float4AndTimestamp = std::tuple<float, float, float, float, unsigned __int64>;

using QuaternionCallbackFunction = std::function<bool(Float4AndTimestamp)>;

}