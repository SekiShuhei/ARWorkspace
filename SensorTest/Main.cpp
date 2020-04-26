#define SIV3D_WINDOWS_HIGH_DPI
#include <Siv3D.hpp> // OpenSiv3D v0.4.2

#include "WinSensorManager.hpp"

void Main()
{
	s3d::Window::Resize(s3d::Size(1200, 900));
	s3d::Window::SetStyle(WindowStyle::Sizable);
	s3d::WindowResizeOption::ResizeSceneSize;
	s3d::Scene::SetScaleMode(ScaleMode::ResizeFill);
	s3d::Scene::SetTextureFilter(TextureFilter::Linear);
	
	WinSensor::WinSensorManager	sensor;

	const Font font(60);
	double value;

	sensor.Initialize();
	sensor.SetPriorityVidList(WinSensor::Device::VidList_SmartGrass);
	sensor.AddSensor(WinSensor::SensorType::AggregatedDeviceOrientation);
	sensor.AddSensor(WinSensor::SensorType::AmbientLight);
	sensor.AddSensor(WinSensor::SensorType::Accelerometer);
	sensor.AddSensor(WinSensor::SensorType::Compass);
	sensor.AddSensor(WinSensor::SensorType::Gyrometer);
	sensor.AddSensor(WinSensor::SensorType::GravityVector);
	sensor.AddSensor(WinSensor::SensorType::LinearAccelerometer);

	while (System::Update())
	{
		
		font(Profiler::FPS(), U"fps").draw(0.0, 0.0, Palette::Blue);
		
		{
			// orientation quaternion to radian angle.
			double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
			auto sensor_val = sensor.GetAggregatedDeviceOrientationData();
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
			font(U"angle x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(x, y, z)).draw(0.0, 100.0, Palette::Green);
		}
		{
			auto sensor_val = sensor.GetAmbientLightData();
			float f = std::get<0>(sensor_val);
			float t = std::get<1>(sensor_val);
			font(U"light:{:.2f},time:{:.2f}"_fmt(f,t)).draw(0.0, 160.0, Palette::Aqua);
		}
		{
			auto sensor_val = sensor.GetAccelerometerData();
			font(U"accel x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(
					std::get<0>(sensor_val), 
					std::get<1>(sensor_val), 
					std::get<2>(sensor_val))).
				draw(0.0, 220.0, Palette::Green);
		}
		{
			auto sensor_val = sensor.GetCompassData();
			font(U"compass x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(
				std::get<0>(sensor_val),
				std::get<1>(sensor_val),
				std::get<2>(sensor_val))).
				draw(0.0, 280.0, Palette::Beige);
		}
		{
			auto sensor_val = sensor.GetGyrometerData();
			font(U"gyro x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(
				std::get<0>(sensor_val),
				std::get<1>(sensor_val),
				std::get<2>(sensor_val))).
				draw(0.0, 340.0, Palette::Blue);
		}
		{
			auto sensor_val = sensor.GetGravityVectorData();
			font(U"gravity x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(
				std::get<0>(sensor_val),
				std::get<1>(sensor_val),
				std::get<2>(sensor_val))).
				draw(0.0, 400.0, Palette::Cyan);
		}
		{
			auto sensor_val = sensor.GetLinearAccelerometerData();
			font(U"linear_accel x:{:.2f},y:{:.2f},z:{:.2f}"_fmt(
				std::get<0>(sensor_val),
				std::get<1>(sensor_val),
				std::get<2>(sensor_val))).
				draw(0.0, 460.0, Palette::Pink);
		}
	}

}