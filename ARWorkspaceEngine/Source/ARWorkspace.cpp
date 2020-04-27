
#include "MadgwickAHRS/MadgwickAHRS.h"
#include "ARWorkspace.hpp"

namespace ARWorkspace {
ARWorkspace::ARWorkspace()
{
}

void ARWorkspace::Update()
{
	this->drawDebugString(0, 100);

	double scale = 1.0;
	int offset_x = 500;
	int offset_y = 500;

	this->DrawSensorCursor(this->eye_point_x, this->eye_point_y,
		offset_x, offset_y, scale,
		this->eye_angle, U"gyro integral", Palette::Aquamarine);

	scale = -1.0;
	this->DrawSensorCursor(
		this->v3_compass.x,
		this->v3_compass.y,
		offset_x, offset_y, scale,
		this->v3_compass.z, 
		U"compass", Palette::Gold);

	scale = 300.0;
	this->DrawSensorCursor(
		this->v3_orientation.x,
		this->v3_orientation.y,
		offset_x, offset_y, scale,
		this->v3_orientation.z, 
		U"orientation", Palette::Lightgreen);

}

void ARWorkspace::SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t)
{
	this->v3_gravity.x = std::get<0>(arg_gravity);
	this->v3_gravity.y = std::get<1>(arg_gravity);
	this->v3_gravity.z = std::get<2>(arg_gravity);

	{
		auto a = this->v3_gravity.dot(s3d::Vec3::UnitY());
		auto angle = s3d::ToDegrees(std::acos(a));
		this->DebugString(U"gravity_V_dot:{:.3f},angle{:.1f}"_fmt(a, angle));
	}
	{
		auto a = this->v3_gravity.dot(s3d::Vec3::UnitZ());
		auto angle = s3d::ToDegrees(std::acos(a));
		this->DebugString(U"gravity_H_dot:{:.3f},angle{:.1f}"_fmt(a, angle));
	}
	{
		auto a = this->v3_gravity.dot(s3d::Vec3::UnitX());
		auto angle = s3d::ToDegrees(std::acos(a));
		this->DebugString(U"gravity_H2_dot:{:.3f},angle{:.1f}"_fmt(a, angle));
	}
}

void ARWorkspace::SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t)
{
	this->v3_compass.x = std::get<0>(arg_compass);
	this->v3_compass.y = std::get<1>(arg_compass);
	this->v3_compass.z = std::get<2>(arg_compass);
}

void ARWorkspace::SetGyroVector(const Vector3AndTimestamp& arg_gyro, const double delta_t)
{
	this->v3_gyro.x = std::get<1>(arg_gyro) * -1; //BT30 Y axis => -X
	this->v3_gyro.y = std::get<0>(arg_gyro) * -1; //BT30 X axis => -Y
	this->v3_gyro.z = std::get<2>(arg_gyro) * -1;

	double scale = 30;
	this->eye_point_x	+= this->v3_gyro.x * scale * delta_t * -1;
	this->eye_point_y	+= this->v3_gyro.y * scale * delta_t * -1;
	this->eye_angle		+= this->v3_gyro.z * 0.02 * delta_t  * -1;

	{
		// ジャイロ角速度の積分値.
		//this->DebugString(U"gyro integral x:{:.3f},angle{:.1f}"_fmt
		//(this->eye_point_x, this->eye_point_y, this->eye_angle));
	}
}

void ARWorkspace::SetOrientationQuaternion(const Float4AndTimestamp& arg_quaternion, const double delta_t)
{
	auto q = s3d::Quaternion(
		std::get<0>(arg_quaternion),
		std::get<1>(arg_quaternion),
		std::get<2>(arg_quaternion),
		std::get<3>(arg_quaternion));

	this->v3_orientation = q.toAxisAngle().first;

	
}

void ARWorkspace::SetEyeAngle(double arg_x, double arg_y, double arg_z)
{
	double x = (arg_x - this->start_angle_x) * this->eye_angle_scale;
	double y = (arg_y - this->start_angle_y) * this->eye_angle_scale;
	double z = (arg_z - this->start_angle_z) * this->eye_angle_scale;

	//this->SetEyePoint((int64_t)x, (int64_t)y);

	//Madgwick madgwick_filter;
	

}

void ARWorkspace::SetEyePoint(int64_t arg_x, int64_t arg_y)
{
	this->eye_point_x = arg_x;
	this->eye_point_y = arg_y;
}


void ARWorkspace::DrawSensorCursor(double x, double y, 
	int offset_x, int offset_y, double display_scale, double angle, 
	const s3d::String& name, s3d::Color color)
{
	int string_offset_x = 60;

	int disp_x = (x * display_scale) + offset_x;
	int disp_y = (y * display_scale) + offset_y;

	Triangle(disp_x, disp_y, 100).
		rotated(angle).draw(color);

	font(name + U" x:{},y:{},angle{:.2f}"_fmt(x, y, angle)).
		draw(disp_x + string_offset_x, disp_y, color);
}

void ARWorkspace::DebugString(const s3d::String& arg_string)
{
	this->debug_strings.emplace_back(arg_string);
}

void ARWorkspace::drawDebugString(int arg_x, int arg_y)
{
	int x = arg_x;
	int y = arg_y;
	int counter = 0;
	for (const auto& string : this->debug_strings)
	{
		this->font(string).draw(x, y + (counter * 50), HSV(counter * 100));
		++counter;
	}
	this->debug_strings.clear();
}



}