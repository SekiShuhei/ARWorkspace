#include "ARVirtualScreen.hpp"


#include "Utilty.hpp"


ARVirtualScreen::ARVirtualScreen()
{

}

ARVirtualScreen::~ARVirtualScreen()
{

	this->capture_thread_run = false;
	this->capture_thread.join();

}

void ARVirtualScreen::initialize()
{
	this->capture_thread_run = true;
	this->capture_thread = std::thread([this]()
		{
			while (this->capture_thread_run)
			{
				// マルチスレッド処理.
				this->Update();

				//std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		});


}

bool ARVirtualScreen::LoadUserSetting()
{


	auto exec_path = s3d::Unicode::Widen(Utilty::GetModulePath());
	s3d::String setting_file_path = exec_path + U"setting.json";
	setting_file_path = U"C:\\Users\\sekishuhei\\010_develop\\ARWorkspace\\Test\\setting.json";
	JSONReader json(setting_file_path);

	if (json.open(setting_file_path))
	{

		//Println(json[L"CaptureRect.Width"].get<int32>());

		this->capture_point.x = json[U"CapturePoint.x"].get<int32>();
		this->capture_point.y = json[U"CapturePoint.y"].get<int32>();		
		this->scale = json[U"CaptureScale"].get<double>();
		
		return true;
	}


	return false;
}

bool ARVirtualScreen::GetCaptureRect()
{
	auto client_size = s3d::Scene::Size();
	
	auto capture_size = client_size / this->scale;

	this->capture_rect.x = this->capture_point.x - (capture_size.x / 2);
	this->capture_rect.y = this->capture_point.y - (capture_size.y / 2);
	this->capture_rect.w = this->capture_point.x + (capture_size.x / 2);
	this->capture_rect.h = this->capture_point.y + (capture_size.y / 2);

	return true;
}

void ARVirtualScreen::Update()
{

	this->GetCaptureRect();
	
	
	// メニューバーの分だけ少しY座標がずれるので注意.
	this->screen_capture.CaptureScreen(
		this->capture_image[this->imageindex_reading],
		this->capture_rect.x,
		this->capture_rect.y,
		this->capture_rect.w,
		this->capture_rect.h);
	
	{
		std::lock_guard<std::mutex>	lock(this->mutex);
		this->imageindex_standby = this->imageindex_reading;
		for (int i = 0; i < 3; i++)
		{
			if (i != this->imageindex_drawing && i != this->imageindex_reading )
			{
				this->imageindex_reading = i;
				break;
			}
		}
		
	}
	
}

void ARVirtualScreen::Draw()
{
	// キャプチャ領域指定.
	//double x = s3d::Scene::Size().x - 300;
	//double y = 0;
	//double h = 30;
	//double value;
	//
	//SimpleGUI::Slider(U"R {:.2f}"_fmt(this->capture_rect.x), value, 0, 600, Vec2(x, y), 100, 200);
	//SimpleGUI::Slider(U"R {:.2f}"_fmt(this->capture_rect.y), value, 0, 600, Vec2(x, y += h), 100, 200);
	//SimpleGUI::Slider(U"R {:.2f}"_fmt(this->capture_rect.w), value, 0, 600, Vec2(x, y += h), 100, 200);
	//SimpleGUI::Slider(U"R {:.2f}"_fmt(this->capture_rect.h), value, 0, 600, Vec2(x, y += h), 100, 200);





	////////キャプチャ系
	{
		{
			std::lock_guard<std::mutex>	lock(this->mutex);
			if (this->imageindex_standby >= 0)
			{
				this->imageindex_drawing = this->imageindex_standby;
				this->imageindex_standby = -1;
			}
		}
		if (this->imageindex_drawing >= 0)
		{
			if (texture.fill(capture_image[this->imageindex_drawing]))
			{
				texture.scaled(this->scale).
					rotatedAt(s3d::Window::ClientCenter(), radian).
					drawAt(s3d::Window::ClientCenter());


			} else {

			}
		}
	}
	//texture.resize(s3d::Window::Width(), s3d::Window::Height()).rotate(radian).draw();
	//texture.draw();

	// test.
	//radian += 0.01;
	
	// カーソル系.
	{
		::POINT	pos;
		::GetCursorPos(&pos);

		custom_cursor.DrawAt(pos.x, pos.y);
		custom_cursor.DrawAt(this->capture_point.x, this->capture_point.y);
	}
	//


	// センサ系.
	{
		double	rx, ry, rz;
		sensor_manager.get_accelerometer_3d_value(rx, ry, rz);

		//font(U"AccelSensor_X:", rx).draw(0.0, 0.0, Palette::Blue);

	}
}
