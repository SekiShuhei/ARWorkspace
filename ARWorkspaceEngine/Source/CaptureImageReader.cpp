#include "CaptureImageReader.hpp"

namespace ARWorkspace {
CaptureImageReader::CaptureImageReader()
{
}

CaptureImageReader::~CaptureImageReader()
{
	this->capture_thread_run = false;
	this->capture_thread.join();

}

void CaptureImageReader::Start()
{
	
	this->capture_thread_run = true;
	this->capture_thread = std::thread([this]()
		{
			::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
			while (this->capture_thread_run)
			{
				if (this->capture_enable)
				{
					this->Capture();
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		});

}

void CaptureImageReader::Capture()
{
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
			if (i != this->imageindex_drawing && i != this->imageindex_reading)
			{
				this->imageindex_reading = i;
				break;
			}
		}

	}

}

}
