#define SIV3D_WINDOWS_HIGH_DPI
# include <Siv3D.hpp> // OpenSiv3D v0.4.2


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

	ARWorkspace::GuiMenu		gui_capture_menu(p_ar_screen);
	ARWorkspace::KeyCommand		key_command(p_ar_screen);

	WinSensor::WinSensorManager	sensor;

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	p_ar_screen->Initialize();
	p_ar_screen->SetAutoResizeMode(true);
	sensor.Initialize();
	sensor.AddSensor(WinSensor::SensorType::AggregatedDeviceOrientation);

	while (System::Update())
	{
		

		//key_command.Update();
		//p_ar_screen->Draw();
		//gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
		double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
		////auto sensor_val = sensor.GetAmbientLightData();
		//
		////auto sensor_val = sensor.GetAccelerometerData();
		auto sensor_val = sensor.GetAggregatedDeviceOrientationData();
		//if (sensor_val)
		//{
			//float f = (sensor_val.value());
			//font(U"light:{:.0f}"_fmt(f)).draw(0.0, 100.0, Palette::Green);
			auto  q = s3d::Quaternion(
				std::get<0>(sensor_val),
				std::get<1>(sensor_val),
				std::get<2>(sensor_val),
				std::get<3>(sensor_val));
			x = std::get<0>(sensor_val); //test
			y = std::get<1>(sensor_val); //test
			z = std::get<2>(sensor_val); //test
			w = std::get<3>(sensor_val); //test

			auto rt_q = q.toAxisAngle();
			
			x = (double)rt_q.first.x;
			y = (double)rt_q.first.y;
			z = (double)rt_q.first.z;
			//font(U"x:{},y:{},z:{},w:{}"_fmt(x, y, z,w)).draw(0.0, 100.0, Palette::Green);
			font(U"x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(x, y, z)).draw(0.0, 100.0, Palette::Green);
		//}
	
	
	}

	p_ar_screen->WriteConfigFile();

}