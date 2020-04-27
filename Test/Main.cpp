﻿#define SIV3D_WINDOWS_HIGH_DPI
#include <Siv3D.hpp> // OpenSiv3D v0.4.2

#include "ARWorkspace.hpp"
#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"
#include "KeyCommand.hpp"
#include "WinSensorManager.hpp"

void Main()
{
	s3d::Window::Resize(s3d::Size(1200, 900));
	s3d::Window::SetStyle(WindowStyle::Sizable);
	s3d::WindowResizeOption::ResizeSceneSize;
	s3d::Scene::SetScaleMode(ScaleMode::ResizeFill);
	s3d::Scene::SetTextureFilter(TextureFilter::Linear);
	
	auto p_ar_screen = std::make_shared<ARWorkspace::ARVirtualScreen>();
	p_ar_screen->ReadConfigFile();

	ARWorkspace::ARWorkspace	ar_workspace;
	ARWorkspace::GuiMenu		gui_capture_menu(p_ar_screen);
	ARWorkspace::KeyCommand		key_command(p_ar_screen);

	WinSensor::WinSensorManager	sensor;

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	p_ar_screen->Initialize();
	p_ar_screen->SetAutoResizeMode(true);
	sensor.Initialize();
	sensor.SetPriorityVidList(WinSensor::Device::VidList_SmartGrass);
	sensor.AddSensor(WinSensor::SensorType::GravityVector);
	sensor.AddSensor(WinSensor::SensorType::Compass);
	sensor.AddSensor(WinSensor::SensorType::Gyrometer);

	while (System::Update())
	{
		

		key_command.Update();
		//p_ar_screen->Draw();
		gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
		{
			double delta_t = s3d::Scene::DeltaTime();

			ar_workspace.SetGravityVector(sensor.GetGravityVectorData(), delta_t);
			ar_workspace.SetCompassVector(sensor.GetCompassData(), delta_t);
			ar_workspace.SetGyroVector(sensor.GetGyrometerData(), delta_t);

			
			//// to ARWS
		}
		{
		
			ar_workspace.Update();
		}



	}
	p_ar_screen->WriteConfigFile();

}