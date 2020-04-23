#pragma once

namespace WinSensor {
enum class SensorManagerState
{
	NotInitialized = 0,
	InitializeCompleted,
	InitializeError,
	UnInitialized,
};

}