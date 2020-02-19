#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#include <Windows.h>

class DisplayRegionSettingWindow
{
public:
	DisplayRegionSettingWindow(HWND main_window_handle);
	~DisplayRegionSettingWindow();

	void Show();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:

	ATOM initializeWindow(HINSTANCE hInst);
	bool createChildWindow();

	HWND main_window_handle;
	std::string class_name = "DisplayRegionGuideWindow";


};

