#pragma once

#include <tuple>
#include <functional>

namespace WinSensor {
using Float4AndTimestamp = std::tuple<float, float, float, float, unsigned __int64>;

using QuaternionCallbackFunction = std::function<bool(Float4AndTimestamp)>;

}