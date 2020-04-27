#define SIV3D_WINDOWS_HIGH_DPI
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

			//double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
			//auto sensor_val = sensor.GetGravityVectorData();
			//auto  v3 = s3d::Vector3D(
			//	std::get<0>(sensor_val),
			//	std::get<1>(sensor_val),
			//	std::get<2>(sensor_val));
			//
			//auto a = s3d::ToDegrees(v3);
			//
			//x = a.x;
			//y = a.y;
			//z = a.z;
			//
			//
			////x = s3d::ToDegrees((double)v3.x);
			////y = s3d::ToDegrees((double)v3.y);
			////z = s3d::ToDegrees((double)v3.z);
			//font(U"angle x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(x, y, z)).draw(0.0, 100.0, Palette::Green);
		

			//// to ARWS
		}
		{
			auto pt = ar_workspace.GetEyePoint();
			int x = std::get<0>(pt);
			int y = std::get<1>(pt);
			font(U"eye_pt x:{},y:{}"_fmt(x, y)).draw(0.0, 200.0, Palette::Blueviolet);
		
		
		}



	}
	p_ar_screen->WriteConfigFile();

}