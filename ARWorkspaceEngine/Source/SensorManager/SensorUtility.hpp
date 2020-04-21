#pragma once

#include <optional>
#include <string>
#include "SensorManagerDefine.hpp"

namespace WinSensor {
class Utility 
{
public:
	static std::optional<std::wstring> GetDevicePath(ISensor* p_sensor);


};
}