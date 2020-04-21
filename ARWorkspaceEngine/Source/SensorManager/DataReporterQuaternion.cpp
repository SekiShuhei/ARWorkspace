#include "DataReporterQuaternion.hpp"

namespace WinSensor {
DataReporterQuaternion::DataReporterQuaternion(ISensorDataReport* p_report) :
	PropvariantWrapper()
{

}

DataReporterQuaternion::~DataReporterQuaternion()
{
}

Float4AndTimestamp DataReporterQuaternion::GetValue() const
{
	
	return Float4AndTimestamp();
}

}