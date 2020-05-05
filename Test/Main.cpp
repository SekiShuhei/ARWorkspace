#define SIV3D_WINDOWS_HIGH_DPI
#include <Siv3D.hpp> // OpenSiv3D v0.4.2

#include "HMD_SensorAnalyzer/HMD_SensorAnalyzer.hpp"
#include "ARVirtualScreen.hpp"
#include "GuiMenu.hpp"
#include "KeyCommand.hpp"
#include "WinSensorManager.hpp"

#include "DisplayInfo.hpp"

void Main()
{
	s3d::Window::Resize(s3d::Size(1200, 900));
	s3d::Window::SetStyle(WindowStyle::Sizable);
	s3d::WindowResizeOption::ResizeSceneSize;
	s3d::Scene::SetScaleMode(ScaleMode::ResizeFill);
	s3d::Scene::SetTextureFilter(TextureFilter::Linear);
	auto p_ar_screen = std::make_shared<ARWorkspace::ARVirtualScreen>(false);
	p_ar_screen->ReadConfigFile();

	ARWorkspace::HMD_SensorAnalyzer	hmd_analyzer;
	ARWorkspace::GuiMenu			gui_capture_menu(p_ar_screen);
	ARWorkspace::KeyCommand			key_command(p_ar_screen);

	WinSensor::WinSensorManager	sensor;

	// 大きさ 60 のフォントを用意
	const Font font(60);
	double value;

	p_ar_screen->Initialize();
	p_ar_screen->SetAutoResizeMode(false);
	sensor.Initialize();
	sensor.SetPriorityVidList(WinSensor::Device::VidList_SmartGrass);
	sensor.AddSensor(WinSensor::SensorType::GravityVector);
	sensor.AddSensor(WinSensor::SensorType::Compass);
	sensor.AddSensor(WinSensor::SensorType::Gyrometer);
	sensor.AddSensor(WinSensor::SensorType::AggregatedDeviceOrientation);
	sensor.AddSensor(WinSensor::SensorType::Accelerometer);
	
	hmd_analyzer.SetDebugDisplayMode(false);

	/// test
	ARWorkspace::DisplayInfoUtility	a;
	a.EnumDisplayInfo();
	
	while (System::Update())
	{
		

		//key_command.Update();
		p_ar_screen->Draw();

		auto eye_pos = hmd_analyzer.GetEyePosition();
		font(U"x:{},y:{}"_fmt(std::get<0>(eye_pos), std::get<1>(eye_pos))).draw(Vec2(0,400));
		p_ar_screen->SetCapturePosition(
			std::get<0>(eye_pos), std::get<1>(eye_pos), 
			std::get<2>(eye_pos), 3.0);
		
		gui_capture_menu.Draw();

		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
		{
			double delta_t = s3d::Scene::DeltaTime();

			hmd_analyzer.SetGravityVector(sensor.GetGravityVectorData(), delta_t);
			hmd_analyzer.SetCompassVector(sensor.GetCompassData(), delta_t);
			hmd_analyzer.SetGyroVector(sensor.GetGyrometerData(), delta_t);
			hmd_analyzer.SetOrientationQuaternion(sensor.GetAggregatedDeviceOrientationData(), delta_t);
			hmd_analyzer.SetAccelVector(sensor.GetAccelerometerData(), delta_t);
			
			hmd_analyzer.Update(delta_t);
		}
		{
			auto x = s3d::Scene::Size().x - 300;
			auto y = s3d::Scene::Size().y - 100;
			if (s3d::SimpleGUI::Button(U"HMD_Analyzer Debug Mode", Vec2(x, y)))
			{
				hmd_analyzer.SetDebugDisplayMode(
					! hmd_analyzer.IsDebugDisplayMode());
			}
			if (s3d::SimpleGUI::Button(U"HMD_Analyzer Reset", Vec2(x, y + 40)))
			{
				hmd_analyzer.ResetCenterAngle();
			}
		}



	}
	p_ar_screen->WriteConfigFile();

}