#pragma once

#include <sensorsapi.h>
#include <sensors.h>
#pragma comment(lib,"sensorsapi.lib")

namespace WinSensor {
class SensorEvents : public ISensorEvents
{
public:
	SensorEvents()
	{
		this->ref_count = 0;
		this->AddRef();
	}

	ULONG __stdcall AddRef()
	{
		return ++this->ref_count;
	}

	ULONG __stdcall Release()
	{
		return --this->ref_count;
	}
	HRESULT __stdcall QueryInterface(const IID& id, void** p)
	{
		if (id == IID_IUnknown || id == __uuidof(ISensorEvents))
		{
			*p = this;
			this->AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	HRESULT __stdcall OnEvent(ISensor* pSensor, REFGUID eventID, IPortableDeviceValues* pEventData)
	{
		return S_OK;
	}
	HRESULT __stdcall OnDataUpdated(ISensor* pSensor, ISensorDataReport* pNewData)
	{
		if (pSensor == nullptr || pNewData == nullptr)
		{
			return E_INVALIDARG;
		}
		//UpdateData(pSensor, pNewData);
		return S_OK;
	}
	// sensor going away (unplugged) refresh the list
	HRESULT __stdcall OnLeave(REFSENSOR_ID sensorID)
	{
		//HWND hL = GetDlgItem(MainWindow, 901);
		//ListView_DeleteAllItems(hL);			// clear the list
		return S_OK;
	}
	// sensor state changed, refresh the list
	HRESULT __stdcall OnStateChanged(ISensor* pSensor, SensorState state)
	{
		//HWND hL = GetDlgItem(MainWindow, 901);
		//ListView_DeleteAllItems(hL);			// clear the list
		return S_OK;
	}
private:
	unsigned long ref_count;


};

}