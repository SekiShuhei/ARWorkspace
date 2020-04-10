#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#include <Windows.h>

class DisplayRegionGuideWindow
{
public:
	DisplayRegionGuideWindow(HWND main_window_handle);
	~DisplayRegionGuideWindow();

	void Show();
	void Update();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:

	HINSTANCE GetInstanceHandle(HWND arg_window_handle) const;

	bool createChildWindow();

	HWND main_window_handle = 0;
	HWND this_window_handle = 0;
	std::string class_name = "DisplayRegionGuideWindow";


};

