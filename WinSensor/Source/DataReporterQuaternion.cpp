#include "DataReporterQuaternion.hpp"

namespace WinSensor {
DataReporterQuaternion::DataReporterQuaternion(ISensorDataReport* p_report) :
	PropvariantWrapper()
{
	if (p_report == nullptr)
	{
		this->result = E_INVALIDARG;
	} else {
		this->result = p_report->GetSensorValue(SENSOR_DATA_TYPE_QUATERNION, &this->pv_data);
	}
}

DataReporterQuaternion::~DataReporterQuaternion()
{
}

Float4AndTimestamp DataReporterQuaternion::GetValue() const
{
	if (this->IsError())
	{
		return Float4AndTimestamp();
	}
	if (this->pv_data.vt == (VT_VECTOR | VT_UI1))
	{
		auto p_element = (float*)this->pv_data.caub.pElems;
		return Float4AndTimestamp(p_element[0], p_element[1], p_element[2], p_element[3], 0);
	}
	return Float4AndTimestamp();
}

}