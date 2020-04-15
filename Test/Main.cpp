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

	ARWorkspace::WinSensorManager	sensor;

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
	
		double x = 0.0, y = 0.0, z = 0.0;
		//auto sensor_val = sensor.GetAccelerometerData();
		auto sensor_val = sensor.GetCompassData();
		if (sensor_val)
		{
			x = std::get<0>(sensor_val.value());
			y = std::get<1>(sensor_val.value());
			z = std::get<2>(sensor_val.value());

		}
		font(U"x:{:.0f},y:{:.0f},z:{:.0f}"_fmt(x, y, z)).draw(0.0, 100.0, Palette::Green);
	
		//s3d::Quaternion
	
	}

	p_ar_screen->WriteConfigFile();
}