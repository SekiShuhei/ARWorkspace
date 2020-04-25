#pragma once

// Sensor.
#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")
#include <sensorsdef.h>

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

using SensorManagerEventsCallbackFunction = std::function<HRESULT(ISensor * p_sensor, SensorState state)>;
using SensorEventCallback_OnDataUpdatedFunction = std::function<HRESULT(ISensor*, ISensorDataReport*)>;
using SensorEventCallback_OnLeavedFunction = std::function<HRESULT(REFSENSOR_ID)>;

}