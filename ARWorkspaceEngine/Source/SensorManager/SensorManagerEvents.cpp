
#include <atlbase.h>
#include "SensorManagerEvents.hpp"

namespace WinSensor {
SensorManagerEvents::SensorManagerEvents()
{
	this->ref_count = 0;
	this->AddRef();

	this->sp_sensor_events = std::make_unique<SensorEvents>();

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
			CComPtr<ISensorCollection> spSensors;
			hr = this->sp_sensor_manager->GetSensorsByType(SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION, &spSensors);
			if (SUCCEEDED(hr) && NULL != spSensors)
			{
				ULONG ulCount = 0;
				// ‚Æ‚è‚ ‚¦‚¸‚O”ÔƒZƒ“ƒT‚¾‚¯Œ©‚é.
				//hr = spSensors->GetCount(&ulCount);
				if (SUCCEEDED(hr))
				{
					for (ULONG i = 0; i < 1; i++)
					{
						CComPtr<ISensor> spSensor;
						hr = spSensors->GetAt(i, &spSensor);
						if (SUCCEEDED(hr))
						{
							int i = 1;
							//if (SUCCEEDED(IsMoverio(spSensor)))
							//{
								//hr = AddSensor(spSensor);
								//if (SUCCEEDED(hr))
								//{
									///////// 
									//hr = m_pSensorEvents->GetSensorData(spSensor);
								//}
							//}
						}
					}
				}
			}
		}
	}

	return hr;


}

HRESULT SensorManagerEvents::Uninitialize()
{
	HRESULT hr;
	hr = this->sp_sensor_manager->SetEventSink(NULL);

	return hr;
}


}