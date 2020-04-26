

#include "SensorType.hpp"
#include "SensorMethodHelper.hpp"

namespace WinSensor {

HRESULT SensorMethodHelper::AddSensor(
	SensorRequest& request, 
	ISensorManager* p_sensor_manager,
	SensorControlManager& sensor_control_manager)
{
	HRESULT hr;
	CComPtr<ISensorCollection> sp_sensor_collection;
	hr = p_sensor_manager->GetSensorsByType(request.type_id, &sp_sensor_collection);
	if (FAILED(hr))
	{
		if (hr = ERROR_NOT_FOUND)
		{
			request.state = SensorRequestState::SensorNotFound;
		} else {
			request.state = SensorRequestState::SensorTypeError;
		}
		return hr;
	}
	ULONG sensor_count = 0;
	hr = sp_sensor_collection->GetCount(&sensor_count);
	if (FAILED(hr))
	{
		request.state = SensorRequestState::SensorNotFound;
		return hr;
	}
	for (ULONG i = 0; i < sensor_count; i++)
	{
		CComPtr<ISensor> sp_sensor;
		hr = sp_sensor_collection->GetAt(i, &sp_sensor);
		if (FAILED(hr))
		{
			request.state = SensorRequestState::SensorNotFound;
			continue;
		}
		// �����ŃZ���T�X�e�[�g�����āADENIED��������A�N�Z�X�p�[�~�b�V������v������.
		//hr = p_sensor->GetState(&state);
		//if (state == SENSOR_STATE_ACCESS_DENIED)
		// ���[�U�[�A�N�Z�X�����Ȃ��ꍇ.
		//hr = this->sp_sensor_manager->RequestPermissions(NULL, sp_sensor_collection, TRUE);
		//if (FAILED(hr))
		//{
		//}

		if (request.vid_list.size() == 0)
		{
			hr = sensor_control_manager.Add(sp_sensor, request);
			if (SUCCEEDED(hr))
			{
				request.state = SensorRequestState::Connected;
				return hr;
			}
		}
		else {
			auto device_path = Utility::GetDevicePath(sp_sensor);
			if (device_path)
			{
				if (Utility::StringContains(device_path.value(), request.vid_list))
				{
					hr = sensor_control_manager.Add(sp_sensor, request);
					if (SUCCEEDED(hr))
					{
						request.state = SensorRequestState::Connected;
						return hr;
					}
				}
			}
		}
	}
	request.state = SensorRequestState::DeviceNotFound;
	hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	return hr;
}

}
