#pragma once

namespace WinSensor {
enum class SensorManagerStatus
{
	NotInitialized = 0,
	InitializeCompleted,
	InitializeError,
	UnInitialized,
};

}