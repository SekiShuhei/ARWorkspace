#define SIV3D_WINDOWS_HIGH_DPI
# include <Siv3D.hpp> // OpenSiv3D v0.4.2


#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"
#include "KeyCommand.hpp"
#include "SensorApiManager.hpp"

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

	ARWorkspace::SensorApiManager	sensor;

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	p_ar_screen->Initialize();
	p_ar_screen->SetAutoResizeMode(true);
	sensor.Initialize();
	
	while (System::Update())
	{
		

		//key_command.Update();
		//p_ar_screen->Draw();
		//gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
	
		double gyro_x, gyro_y, gyro_z;
		auto sensor_val = sensor.GetAccelerometerSensorData();
		if (sensor_val)
		{
			gyro_x = std::get<0>(sensor_val.value());
			gyro_y = std::get<1>(sensor_val.value());
			gyro_z = std::get<2>(sensor_val.value());

		}
		//sensor.GetAccelerometerSensorData(gyro_x, gyro_y, gyro_z);
		//sensor.GetGyrometerSensorData(gyro_x, gyro_y, gyro_z);
		font(U"x:{:.0f},y:{:.0f},z:{:.0f}"_fmt(gyro_x, gyro_y, gyro_z)).draw(0.0, 100.0, Palette::Green);
	}

	p_ar_screen->WriteConfigFile();
}