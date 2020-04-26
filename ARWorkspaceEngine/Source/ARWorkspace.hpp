#pragma once

#include <tuple>
#include <Siv3D.hpp>

namespace ARWorkspace {
class ARWorkspace
{
public:
	ARWorkspace();

public:

	void SetEyeAngle(double arg_x, double arg_y, double arg_z);
	void SetEyePoint(int64_t arg_x, int64_t arg_y);
	
	std::tuple<int64_t, int64_t> GetEyePoint() const
	{
		return std::tuple<int64_t, int64_t>(this->eye_point_x, this->eye_point_y);
	}

private:
	double	start_angle_x = 0.0;
	double	start_angle_y = 0.0;
	double	start_angle_z = 0.0;

	double	eye_angle_scale = 10.0;
	int64_t	eye_point_x = 0;
	int64_t	eye_point_y = 0;

	double	display_scale = 1.0;
	int		display_size_x = 800;
	int		display_size_y = 600;
};

}