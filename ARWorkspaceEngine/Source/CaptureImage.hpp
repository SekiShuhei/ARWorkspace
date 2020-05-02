#pragma once

#include <Siv3D.hpp>
#include "ScreenRegion.hpp"

namespace ARWorkspace {

//enum class CaptureImageState
//{
//	NotInitialized = -1,
//	Reading = 0,
//	Standby,
//	Drawing,
//	Drawed
//};

struct CaptureImage
{
public:

public:
	s3d::Image			image;
	//CaptureImageState	state = CaptureImageState::NotInitialized;

	ScreenRegion		region;

private:

};

}