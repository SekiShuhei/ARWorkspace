#pragma once

#include <vector>
#include <tuple>
#include <Siv3D.hpp>

namespace ARWorkspace {
class ARWorkspace
{
using Vector3AndTimestamp = std::tuple<double, double, double, uint64_t>;
using Float4AndTimestamp = std::tuple<float, float, float, float, uint64_t>;

public:
	ARWorkspace();

public:

	void Update();

	void SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t);
	void SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t);
	void SetGyroVector(const  Vector3AndTimestamp& arg_gyro, const double delta_t);
	void SetOrientationQuaternion(const  Float4AndTimestamp& arg_quaternion, const double delta_t);

	void SetEyeAngle(double arg_x, double arg_y, double arg_z);
	void SetEyePoint(int64_t arg_x, int64_t arg_y);

	void DrawSensorCursor(double x, double y, int offset_x, int offset_y, double display_scale, double angle, const s3d::String& name, s3d::Color color);

	void DebugString(const s3d::String& arg_string);

	void drawDebugString(int arg_x, int arg_y);

	

	std::tuple<int64_t, int64_t> GetEyePoint() const
	{
		return std::tuple<int64_t, int64_t>(this->eye_point_x, this->eye_point_y);
	}

private:

	const s3d::Font	font = s3d::Font(40);

	s3d::Vec3	v3_gravity;
	s3d::Vec3	v3_compass;
	s3d::Vec3	v3_gyro;
	s3d::Vec3	v3_orientation;

	double	start_angle_x = 0.0;
	double	start_angle_y = 0.0;
	double	start_angle_z = 0.0;

	double	eye_angle_scale = 5.0;
	double	eye_angle = 0.0;
	int64_t	eye_point_x = 0;
	int64_t	eye_point_y = 0;

	double	display_scale = 1.0;
	int		display_size_x = 800;
	int		display_size_y = 600;


	std::vector<s3d::String> debug_strings = std::vector<s3d::String>(30);
};

}