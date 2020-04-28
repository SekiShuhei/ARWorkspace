
#include "ARWorkspace.hpp"

namespace ARWorkspace {
ARWorkspace::ARWorkspace()
{

}

void ARWorkspace::Update(const double delta_t)
{
	this->drawDebugString(0, 100);

	double scale = 1.0;
	int offset_x = 500;
	int offset_y = 500;

	{
		this->madgwick_filter.Update(
			this->gyro_raw.x, this->gyro_raw.y, this->gyro_raw.z,
			this->accel.x, this->accel.y, this->accel.z,
			this->compass.x, this->compass.y, this->compass.z,
			delta_t);

		auto roll  = this->madgwick_filter.getRollRadians() ;
		auto pitch = this->madgwick_filter.getPitchRadians();
		auto yaw   = this->madgwick_filter.getYawRadians()  ;

		this->DebugString(U"madgwick_filter roll:{:.1f},pitch:{:.1f},yaw{:.1f}"_fmt
		(roll, pitch, yaw));

		scale = 8.0 * 50;
		this->DrawSensorCursor(yaw, pitch, offset_x - 1400, offset_y, scale, roll / 12,
			U"MadgwickAngle", Palette::Goldenrod);

	}

	scale = 0.3 * 30;
	this->DrawSensorCursor(this->eye_point.x, this->eye_point.y,
		offset_x, offset_y, scale,
		this->eye_point.z * 0.02, U"gyro integral", Palette::Aquamarine);

	scale = 1.0;
	this->DrawSensorCursor(this->gyro.x, this->gyro.y,
		offset_x, offset_y, scale,
		this->gyro.z / 100, U"gyro", Palette::Ivory);

	scale = 300.0;
	this->DrawSensorCursor(this->gravity_dot.x, this->gravity_dot.y,
		offset_x, offset_y, scale,
		this->gravity_dot.z, U"gravity dot", Palette::Lightgreen);

	//scale = -1.0;
	//this->DrawSensorCursor(
	//	this->compass.x,
	//	this->compass.y,
	//	offset_x, offset_y, scale,
	//	this->compass.z, 
	//	U"compass", Palette::Gold);

	scale = 300.0;
	//this->DrawSensorCursor(
	//	this->orientation.x,
	//	this->orientation.y,
	//	offset_x, offset_y, scale,
	//	this->orientation.z, 
	//	U"orientation", Palette::Lightgreen);

}

void ARWorkspace::SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t)
{
	this->gravity.x = std::get<0>(arg_gravity);
	this->gravity.y = std::get<1>(arg_gravity);
	this->gravity.z = std::get<2>(arg_gravity);

	{
		this->gravity_dot.y = this->gravity.dot(s3d::Vec3::UnitY());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.y));
		this->DebugString(U"gravity_V_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.y, angle));
	}
	{
		this->gravity_dot.z = this->gravity.dot(s3d::Vec3::UnitZ());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.z));
		this->DebugString(U"gravity_H_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.z, angle));
	}
	{
		this->gravity_dot.x = this->gravity.dot(s3d::Vec3::UnitX());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.x));
		this->DebugString(U"gravity_H2_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.x, angle));
	}
	{

	}
}

void ARWorkspace::SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t)
{
	this->compass.x = std::get<0>(arg_compass);
	this->compass.y = std::get<1>(arg_compass);
	this->compass.z = std::get<2>(arg_compass);
}

void ARWorkspace::SetGyroVector(const Vector3AndTimestamp& arg_gyro, const double delta_t)
{
	this->gyro_raw.x = std::get<2>(arg_gyro) * -1;
	this->gyro_raw.y = std::get<0>(arg_gyro) * -1;
	this->gyro_raw.z = std::get<1>(arg_gyro) * -1;

	this->gyro.x = std::get<1>(arg_gyro) * -1; //BT30 Y axis => -X
	this->gyro.y = std::get<0>(arg_gyro) * -1; //BT30 X axis => -Y
	this->gyro.z = std::get<2>(arg_gyro) * -1;

	this->eye_point.x += this->gyro.x * delta_t; // * -1;
	this->eye_point.y += this->gyro.y * delta_t; // * -1;
	this->eye_point.z += this->gyro.z * delta_t; // * -1;

	
}

void ARWorkspace::SetAccelVector(const Vector3AndTimestamp& arg_accel, const double delta_t)
{
	this->accel.x = std::get<0>(arg_accel);
	this->accel.y = std::get<1>(arg_accel);
	this->accel.z = std::get<2>(arg_accel);

}

void ARWorkspace::SetOrientationQuaternion(const Float4AndTimestamp& arg_quaternion, const double delta_t)
{
	auto q = s3d::Quaternion(
		std::get<0>(arg_quaternion),
		std::get<1>(arg_quaternion),
		std::get<2>(arg_quaternion),
		std::get<3>(arg_quaternion));

	this->orientation = q.toAxisAngle().first;

	
}

void ARWorkspace::SetEyeAngle(double arg_x, double arg_y, double arg_z)
{
}

void ARWorkspace::SetEyePoint(int64_t arg_x, int64_t arg_y)
{
}


void ARWorkspace::DrawSensorCursor(double x, double y, 
	int offset_x, int offset_y, double display_scale, double angle, 
	const s3d::String& name, s3d::Color color)
{
	int string_offset_x = 60;

	int disp_x = (x * display_scale) + offset_x;
	int disp_y = (y * display_scale) + offset_y;
	int width = 40;
	int height = 50;
	Triangle(
		Vec2(disp_x, disp_y - height),
		Vec2(disp_x - width, disp_y + height),
		Vec2(disp_x + width, disp_y + height)).
		rotated(angle).draw(color);

	font(name + U" x:{:.2f},y:{:.2f},angle{:.2f}"_fmt(x, y, angle)).
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