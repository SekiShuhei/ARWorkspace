#pragma once

#include <tuple>
#include <functional>

using Float4AndTimestamp = std::tuple<float, float, float, float, UINT64>;

using QuaternionCallbackFunction = std::function<bool(Float4AndTimestamp)>;
