
#include "MadgwickAHRS/MadgwickAHRS.h"
#include "ARWorkspace.hpp"

namespace ARWorkspace {
ARWorkspace::ARWorkspace()
{
}

void ARWorkspace::Update()
{

	this->DrawSensorCursor(this->eye_point_x, this->eye_point_y, 
		this->eye_angle, U"test", Palette::Aquamarine);


}

void ARWorkspace::SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t)
{
	this->v3_gravity.x = std::get<0>(arg_gravity);
	this->v3_gravity.y = std::get<1>(arg_gravity);
	this->v3_gravity.z = std::get<2>(arg_gravity);
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

	this->font(U"gyro:{},{},{}"_fmt
	(this->v3_gyro.x, this->v3_gyro.y, this->v3_gyro.z))
		.draw(0, 100);

	double scale = 30;
	this->eye_point_x	+= this->v3_gyro.x * scale * delta_t * -1;
	this->eye_point_y	+= this->v3_gyro.y * scale * delta_t * -1;
	this->eye_angle		+= this->v3_gyro.z * 0.02 * delta_t  * -1;

	
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


void ARWorkspace::DrawSensorCursor(int x, int y, double angle, const s3d::String& name, s3d::Color color)
{
	int offset_x = 500;
	int offset_y = 500;
	int string_offset_x = 60;

	Triangle(x + offset_x, y + offset_y, 100).
		rotated(this->eye_angle).draw(color);

	font(name + U" x:{},y:{},angle{:.2f}"_fmt(x, y, angle)).
		draw(x + offset_x + string_offset_x, y + offset_y, color);
}

}