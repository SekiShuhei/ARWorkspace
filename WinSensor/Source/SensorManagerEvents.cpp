
#include <atlbase.h>
#include "SensorRequest.hpp"
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents()
{
	this->ref_count = 0;
	this->AddRef();

}
ULONG __stdcall SensorManagerEvents::AddRef()
{
	return InterlockedIncrement(&this->ref_count);
}
ULONG __stdcall SensorManagerEvents::Release()
{
	ULONG count = InterlockedDecrement(&this->ref_count);
	if (count == 0)
	{
		delete this;
		return 0;
	}
	return count;
}
HRESULT __stdcall SensorManagerEvents::QueryInterface(const IID& iid, void** ppv)
{
	if (ppv == nullptr)
	{
		return E_POINTER;
	}
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(ISensorManagerEvents))
	{
		*ppv = static_cast<ISensorManagerEvents*>(this);
	}
	else
	{
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	this->AddRef();
	return S_OK;
}

HRESULT __stdcall SensorManagerEvents::OnSensorEnter(__RPC__in_opt ISensor* p_sensor, SensorState state)
{
	// TODO:
	// ���N�G�X�g���̃Z���T���ڑ����ꂽ�玩���I�ɒǉ������悤�ɂ�����.
	// �}�l�[�W�����Ń��N�G�X�g����ێ����Ă����K�v������.
	//...

	return S_OK;
}

HRESULT SensorManagerEvents::Initialize()
{
	HRESULT hr;
	hr = this->sp_sensor_manager.CoCreateInstance(CLSID_SensorManager);
	if (SUCCEEDED(hr))
	{
		hr = this->sp_sensor_manager->SetEventSink(this);
		if (SUCCEEDED(hr))
		{
			this->initialized = true;
		}
	}
	return hr;
}

HRESULT SensorManagerEvents::AddSensor(const SensorRequest& request)
{
	
	HRESULT hr;
	CComPtr<ISensorCollection> sp_sensor_collection;
	hr = this->sp_sensor_manager->GetSensorsByType(request.type_id, &sp_sensor_collection);
	
	// ���[�U�[�A�N�Z�X�����Ȃ��ꍇ.
	//hr = this->sp_sensor_manager->RequestPermissions(NULL, sp_sensor_collection, TRUE);
	//if (FAILED(hr))
	//{
	//	SENSOR_STATUS_DISABLED;
	//}
	
	if (FAILED(hr))
	{
		return hr;
	}
	ULONG sensor_count = 0;
	hr = sp_sensor_collection->GetCount(&sensor_count);
	if (FAILED(hr))
	{
		return hr;
	}
	for (ULONG i = 0; i < sensor_count; i++)
	{
		CComPtr<ISensor> sp_sensor;
		hr = sp_sensor_collection->GetAt(i, &sp_sensor);
		if (FAILED(hr))
		{
			continue;
		}
		if (request.vid_list.size() == 0)
		{
			hr = this->addSensor(sp_sensor, request);
			if (SUCCEEDED(hr))
			{
				// �ڑ�1���ڂ̃f�[�^�擾.
				//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
				return hr;
			}
		} else {
			auto device_path = Utility::GetDevicePath(sp_sensor);
			if (device_path)
			{
				if (Utility::StringContains(device_path.value(), request.vid_list))
				{
					hr = this->addSensor(sp_sensor, request);
					if (SUCCEEDED(hr))
					{
						// �ڑ�1���ڂ̃f�[�^�擾.
						//hr = this->sp_sensor_events->GetSensorData(sp_sensor);
						return hr;
					}
				}
			}
		}
	}
	hr = E_FAIL;
	return hr;
}

HRESULT SensorManagerEvents::Uninitialize()
{
	HRESULT hr = S_OK;

	this->info_manager.RemoveAll();
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}

HRESULT SensorManagerEvents::addSensor(ISensor* p_sensor, const SensorRequest& request)
{
	if (p_sensor == nullptr) 
	{
		return E_POINTER;
	}
	
	HRESULT hr = S_OK;
	hr = this->info_manager.Add(p_sensor, request);
	
	return hr;
}

HRESULT SensorManagerEvents::removeSensor(ISensor* p_sensor)
{
	if (p_sensor == nullptr)
	{
		return E_POINTER;
	}
	HRESULT hr = S_OK;

	//...
	// �ǉ�.

	return hr;
}


}