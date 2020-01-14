#include "ARVirtualScreen.hpp"


#include "Utilty.hpp"


ARVirtualScreen::ARVirtualScreen()
{

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
	screen_capture.CaptureScreen(
		this->capture_rect.x,
		this->capture_rect.y,
		this->capture_rect.w,
		this->capture_rect.h);

	//Clipboard::GetImage(capture_image);
	capture_image = this->screen_capture.GetImage();

}

void ARVirtualScreen::Draw()
{
	
	////////キャプチャ系
	{
		
		if (texture.fillIfNotBusy(capture_image))
		{
			texture.scaled(this->scale).
				rotatedAt(s3d::Window::ClientCenter(), radian).
				drawAt(s3d::Window::ClientCenter());
		} else {

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
