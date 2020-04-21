#pragma once

#include <optional>
#include <string>
#include <vector>
#include "SensorManagerDefine.hpp"

namespace WinSensor {
class Utility 
{
public:

	static bool StringContains(const std::wstring& base, const std::vector<std::wstring>& list);

	static std::optional<std::wstring> GetDevicePath(ISensor* p_sensor);


};
}