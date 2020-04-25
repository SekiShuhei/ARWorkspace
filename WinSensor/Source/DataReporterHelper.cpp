
#include "DataReporterHelper.hpp"
#include "WinSensorManager.hpp"


namespace WinSensor {

HRESULT DataReporterHelper::GetReportVector3(
	ISensor* p_sensor, ISensorDataReport* p_data, 
	Double3AndTimestamp& result_data, const DataReporterVector3& data_report,
	bool use_timestamp) noexcept
{
	if (data_report.IsError())
	{
		return data_report.GetResult();
	}
	result_data = data_report.GetValue();
	if (use_timestamp)
	{
		DataReporterTimeStamp time_stamp(p_data);
		if (!time_stamp.IsError())
		{
			std::get<3>(result_data) = time_stamp.GetValue();
		}
	}
	return data_report.GetResult();
}

}