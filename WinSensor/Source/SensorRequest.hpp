#pragma once

#include <string>
#include "SensorManagerDefine.hpp"

namespace WinSensor {

//TODO:
// ���N�G�X�g����enum class�i�G���[���܂ށj�̒�`.
// ���N�G�X�g���ʂ��\���̓��ɕێ�����
	
// �ڑ�����struct��V��.
// ���ʂň�����Z���T���͕ʓr�\���̂Ƃ��ēƗ�������.

struct SensorRequest
{
public:
	SensorRequest() : type_id(GUID_NULL)
	{

	};
	std::wstring				name;
	SENSOR_TYPE_ID				type_id;
	std::vector<std::wstring>	vid_list;

	SensorEventCallback_OnDataUpdatedFunction	callback_data_updated_func =
		[](ISensor*, ISensorDataReport*){ return S_OK; };

	SensorEventCallback_OnLeavedFunction callback_sensor_leaved_func = 
		[](REFSENSOR_ID){ return S_OK; };

};

}
