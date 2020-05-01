#include "ARVirtualScreen.hpp"

#include "Utility.hpp"

namespace ARWorkspace {

ARVirtualScreen::ARVirtualScreen(bool use_cauture_region_guide) :
	capture_region_guide_enable(use_cauture_region_guide)
{
	this->p_texture = std::make_unique<s3d::DynamicTexture>();
}

ARVirtualScreen::~ARVirtualScreen()
{
	this->capture_thread_run = false;
	this->capture_thread.join();

	if (this->capture_region_guide_enable)
	{
		this->capture_region_guide_thread_run = false;
		this->capture_region_guide_thread.join();
	}
}

void ARVirtualScreen::Initialize()
{
	this->capture_thread_run = true;
	this->capture_thread = std::thread([this]()
		{
			::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
			while (this->capture_thread_run)
			{
				if (!this->capture_region_guide_counter.IsCount())
				{
					this->Capture();
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		});

	if (this->capture_region_guide_enable)
	{
		this->capture_region_guide_thread_run = true;
		this->capture_region_guide_thread = std::thread([this]()
			{
				::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
				while (this->capture_region_guide_thread_run)
				{
					
					if (this->capture_region_updated)
					{
						this->capture_region_updated = false;
						this->capture_region_guide_counter.Reset();
						this->capture_region_guide_counter.Count();
					}
					if (this->capture_region_guide_counter.IsCount())
					{
						if (this->capture_region_guide_counter.Count())
						{
							this->capture_region_guide.Invalidate();
							this->capture_region_guide_counter.Reset();
						} else {
							this->capture_region_guide.Draw();
						}
					}
					
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				}
			});
	}
}

bool ARVirtualScreen::ReadConfigFile()
{

	auto exec_path = Utility::GetExecFilePath();
	s3d::String setting_file_path = exec_path + U"userconfig.json";
	JSONReader json(setting_file_path);
	
	if (json.open(setting_file_path))
	{
		this->capture_point.x = json[U"CapturePoint.x"].get<int32>();
		this->capture_point.y = json[U"CapturePoint.y"].get<int32>();
		this->capture_region.SetX(json[U"CaptureRegion.x"].get<int32>());
		this->capture_region.SetY(json[U"CaptureRegion.y"].get<int32>());
		this->capture_region.SetWidth(json[U"CaptureRegion.width"].get<int32>());
		this->capture_region.SetHeight(json[U"CaptureRegion.height"].get<int32>());
		this->scale = json[U"CaptureScale"].get<double>();
		
		return true;
	}


	return false;
}

bool ARVirtualScreen::WriteConfigFile()
{
	JSONWriter	json;
	json.startObject();
	{
		json.key(U"CaptureScale").write(this->scale);

		json.key(U"CaptureRegion").startObject();
		{
			json.key(U"x").write((int)this->capture_region.GetX());
			json.key(U"y").write((int)this->capture_region.GetY());
			json.key(U"width").write((int)this->capture_region.GetWidth());
			json.key(U"height").write((int)this->capture_region.GetHeight());
		}
		json.endObject();
	}
	json.endObject();

	auto exec_path = Utility::GetExecFilePath();
	json.save(exec_path + U"userconfig.json");


	return false;
}




bool ARVirtualScreen::GetCaptureRect()
{
	// ���S�_����L���v�`���̈������o��.
	// �v����.
	//auto client_size = s3d::Scene::Size();
	//
	//auto capture_size = client_size / this->scale;
	//
	//this->capture_region.GetX() = this->capture_point.x - (capture_size.x / 2);
	//this->capture_region.GetY() = this->capture_point.y - (capture_size.y / 2);
	//this->capture_region.GetWidth() = this->capture_point.x + (capture_size.x / 2);
	//this->capture_region.GetHeight() = this->capture_point.y + (capture_size.y / 2);

	return true;
}

void ARVirtualScreen::Capture()
{
	
	this->GetCaptureRect();
	
	
	this->screen_capture.CaptureScreen(
		this->capture_image[this->imageindex_reading],
		(int)this->capture_region.GetX(),
		(int)this->capture_region.GetY(),
		(int)this->capture_region.GetWidth(),
		(int)this->capture_region.GetHeight());
	
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
	
	
	if (! this->capture_region_guide_counter.IsCount())
	{
		this->drawTexture();
	}
	
	
	{
		// �L���v�`���e�N�X�`���̊Ǘ�.
		if (this->capture_size_updated)
		{
			this->capture_size_updated = false;
			this->texture_reflesh_counter.Reset();
		} else {
			
			if (this->texture_reflesh_counter.Count() &&
				this->p_texture->size() != this->GetDrawImage().size())
			{
				this->p_texture = std::make_unique<s3d::DynamicTexture>();
				
			}
		}	
	}

	
	//...
}

void ARVirtualScreen::SetCaptureRegion(int arg_x, int arg_y, int arg_width, int arg_height)
{
	this->capture_region.SetX(arg_x);
	this->capture_region.SetY(arg_y);
	this->capture_region.SetWidth(arg_width);
	this->capture_region.SetHeight(arg_height);
	this->CaptureRegionUpdate();
	this->CaptureSizeUpdate();
}

void ARVirtualScreen::SetCaptureRegionPosition(int arg_x, int arg_y)
{
	this->capture_region.SetX(arg_x);
	this->capture_region.SetY(arg_y);
	this->CaptureRegionUpdate();
}

void ARVirtualScreen::SetCaptureRegionSize(int arg_width, int arg_height)
{
	this->capture_region.SetWidth(arg_width);
	this->capture_region.SetHeight(arg_height);
	this->CaptureRegionUpdate();
	this->CaptureSizeUpdate();
}

void ARVirtualScreen::SetCapturePosition(int x, int y, double arg_angle, double scale)
{

	this->SetCaptureRegionPosition((x * 0.5) + 300, (y * 0.5) + 300);

	this->angle = arg_angle * -1;
	///
	//...
}

void ARVirtualScreen::drawTexture()
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

		if (p_texture->fill(this->GetDrawImage()))
		{
			if (this->texture_auto_resize)
			{
				p_texture->resized(s3d::Window::ClientWidth(), s3d::Window::ClientHeight()).
					draw(0,0);
			} else {
				p_texture->scaled(this->scale).
					rotatedAt(s3d::Window::ClientCenter(), this->angle).
					drawAt(s3d::Window::ClientCenter());
			}

		}
		else {
			// �e�N�X�`���̓��T�C�Y�ł��Ȃ�.
			// ����̃T�C�Y�ƈႤImage��Fill���悤�Ƃ����false���Ԃ�.
			// �����ԂŃe�N�X�`���č쐬���邱�Ƃŉ���.
		}
	}
	
	
}

}