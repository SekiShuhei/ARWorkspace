#pragma once

#include "SensorManagerDefine.hpp"
#include "DataReporter.hpp"
#include "DataReporterQuaternion.hpp"
#include "DataReporterVector3.hpp"
#include "DataReporterTimestamp.hpp"


namespace WinSensor {
class DataReporterHelper
{
	static HRESULT GetReportVector3(
		ISensor* p_sensor, ISensorDataReport* p_data,
		Double3AndTimestamp& result_data, const DataReporterVector3& data_report) noexcept;

};

}