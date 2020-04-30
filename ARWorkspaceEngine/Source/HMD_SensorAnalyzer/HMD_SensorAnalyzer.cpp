
#include "HMD_SensorAnalyzer.hpp"

namespace ARWorkspace {
HMD_SensorAnalyzer::HMD_SensorAnalyzer()
{

}

void HMD_SensorAnalyzer::Update(const double delta_t)
{
	if (this->IsDeviceRollFlat())
	{
		this->font(U"IsDeviceRollFlat = true").draw(Vec2(0, 600));
	}
	if (this->IsDeviceNearlyCompassStartAngle())
	{
		this->font(U"IsDeviceNearlyCompassStartAngle = true").draw(Vec2(0, 660));
	}
	if (this->IsDeviceNearlyStartAngle())
	{
		if (this->IsDeviceRollFlat() && this->IsDeviceStaticAngle())
		{
			this->gyro.integral.Smoothing(0.0, 1 * delta_t);
			this->madgwick.base.Smoothing(this->madgwick.GetInput(), 1 * delta_t);
			this->font(U"IsDeviceNearlyStartAngle = true").draw(Vec2(0, 720));
		}
	}
	if (this->IsDeviceNearlyCompassCenterAngle())
	{
		Vector3::Smoothing(this->gyro.integral.x, 0.0, 0.1 * delta_t);
		
		this->font(U"IsDeviceNearlyCompassCenterAngle = true").draw(Vec2(0, 780));
	}

	this->updateEyePoint();

	this->drawDebugString(0, 100);

	double scale = 1.0;
	int offset_x = 500;
	int offset_y = 500;

	{
		this->madgwick_filter.Update(
			this->gyro_raw.x, this->gyro_raw.y, this->gyro_raw.z,
			this->accel.x, this->accel.y, this->accel.z,
			this->compass.GetRelative().x, 
			this->compass.GetRelative().y, 
			this->compass.GetRelative().z,
			delta_t);

		auto roll  = this->madgwick_filter.getRollRadians() + 1;
		auto pitch = this->madgwick_filter.getPitchRadians();
		auto yaw   = this->madgwick_filter.getYawRadians() + 2;

		this->madgwick.SetData(Vector3(yaw, pitch, roll));
		
		this->insertDebugString(U"madgwick_filter roll:{:.1f},pitch:{:.1f},yaw{:.1f}"_fmt
		(roll, pitch, yaw));

		scale = 8.0 * 50;
		this->DrawSensorCursor(yaw, pitch, offset_x - 1100, offset_y, 
			scale, roll * 2,
			U"MadgwickAngle", Palette::Goldenrod);

	}

	scale = 0.3 * 30;
	this->DrawSensorCursor(this->gyro.GetIntegral().x, this->gyro.GetIntegral().y,
		offset_x, offset_y, scale,
		this->gyro.GetIntegral().z * 0.018, U"gyro integral", Palette::Aquamarine);

	scale = 1.0;
	this->DrawSensorCursor(this->gyro.GetInput().x, this->gyro.GetInput().y,
		offset_x, offset_y, scale,
		this->gyro.GetInput().z / 100, U"gyro", Palette::Ivory);

	scale = 300.0;
	this->DrawSensorCursor(
		this->eye_angle1.x, this->eye_angle1.y,
		offset_x, offset_y, scale,
		this->eye_angle1.z * 1.7, 
		U"eye_pt1", Palette::Lightgreen);

	scale = 300.0;
	this->DrawSensorCursor(
		this->orientation.x, this->orientation.y,
		offset_x, offset_y, scale,
		this->orientation.z, 
		U"orientation", Palette::Beige);

	scale = 300.0;
	this->DrawSensorCursor(
		this->eye_angle2.x, this->eye_angle2.y,
		offset_x, offset_y, scale,
		this->eye_angle2.z * 1.7,
		U"eye_pt2", Palette::Orange);

	scale = 300.0;
	this->DrawSensorCursor(
		this->eye_angle3.x, this->eye_angle3.y,
		offset_x, offset_y, scale,
		this->eye_angle3.z * 1.7,
		U"eye_pt3", Palette::Pink);

}

void HMD_SensorAnalyzer::ResetCenterAngle()
{
	this->madgwick.ResetBase();
	this->compass.ResetBase();
	this->gyro.ResetIntegral();
}

void HMD_SensorAnalyzer::SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t)
{
	this->gravity.x = std::get<0>(arg_gravity);
	this->gravity.y = std::get<1>(arg_gravity);
	this->gravity.z = std::get<2>(arg_gravity);

	{
		this->gravity_dot.y = this->gravity.dot(s3d::Vec3::UnitY());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.y));
		this->insertDebugString(U"gravity_V_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.y, angle));
	}
	{
		this->gravity_dot.z = this->gravity.dot(s3d::Vec3::UnitZ());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.z));
		this->insertDebugString(U"gravity_H_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.z, angle));
	}
	{
		this->gravity_dot.x = this->gravity.dot(s3d::Vec3::UnitX());
		auto angle = s3d::ToDegrees(std::acos(this->gravity_dot.x));
		this->insertDebugString(U"gravity_H2_dot:{:.3f},angle{:.1f}"_fmt(this->gravity_dot.x, angle));
	}
	{

	}
}

void HMD_SensorAnalyzer::SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t)
{
	this->compass.SetData(
		std::get<0>(arg_compass),
		std::get<1>(arg_compass),
		std::get<2>(arg_compass));
}

void HMD_SensorAnalyzer::SetGyroVector(const Vector3AndTimestamp& arg_gyro, const double delta_t)
{
	this->gyro_raw.x = std::get<2>(arg_gyro) * -1;
	this->gyro_raw.y = std::get<1>(arg_gyro);
	this->gyro_raw.z = std::get<0>(arg_gyro) * -1;

	this->gyro.SetData(
		std::get<1>(arg_gyro) * -1,
		std::get<0>(arg_gyro) * -1,
		std::get<2>(arg_gyro) * -1, delta_t);	
}

void HMD_SensorAnalyzer::SetAccelVector(const Vector3AndTimestamp& arg_accel, const double delta_t)
{
	this->accel.x = std::get<0>(arg_accel);
	this->accel.y = std::get<1>(arg_accel);
	this->accel.z = std::get<2>(arg_accel);

}

void HMD_SensorAnalyzer::SetOrientationQuaternion(const Float4AndTimestamp& arg_quaternion, const double delta_t)
{
	auto q = s3d::Quaternion(
		std::get<0>(arg_quaternion),
		std::get<1>(arg_quaternion),
		std::get<2>(arg_quaternion),
		std::get<3>(arg_quaternion));

	this->orientation = q.toAxisAngle().first;

	
}

void HMD_SensorAnalyzer::SetEyeAngle(double arg_x, double arg_y, double arg_z)
{
}

void HMD_SensorAnalyzer::SetEyePoint(int64_t arg_x, int64_t arg_y)
{
}


void HMD_SensorAnalyzer::DrawSensorCursor(double x, double y, 
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

void HMD_SensorAnalyzer::insertDebugString(const s3d::String& arg_string)
{
	this->debug_strings.emplace_back(arg_string);
}

void HMD_SensorAnalyzer::drawDebugString(int arg_x, int arg_y)
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

bool HMD_SensorAnalyzer::IsDeviceRollFlat() const
{
	if (this->gravity_dot.y < 0.0)
	{
		return false;
	}
	if (Vector3::IsRange(this->gravity_dot.x, 0.0, 
		this->device_roll_flat_margin))
	{
		return true;
	}
	return false;
}

bool HMD_SensorAnalyzer::IsDeviceNearlyCompassStartAngle() const
{
	return this->compass.GetRelative().IsRange( 0.0,
		this->device_nearly_compass_start_margin);
}

bool HMD_SensorAnalyzer::IsDeviceNearlyStartAngle() const
{
	bool result;
	result = this->IsDeviceNearlyCompassStartAngle();

	return result;
}

bool HMD_SensorAnalyzer::IsDeviceNearlyCompassCenterAngle() const
{
	return Vector3::IsRange(this->compass.GetRelative().x, 0.0, 20);
}

void HMD_SensorAnalyzer::updateEyePoint()
{
	
	if (this->gravity_dot.y <= 0)
	{
		return;
	}
	this->eye_angle1.y = this->gravity_dot.z;
	this->eye_angle1.z = this->gravity_dot.x * -1;
	this->eye_angle2.y = this->gravity_dot.z;
	this->eye_angle2.z = this->gravity_dot.x * -1;
	this->eye_angle3.y = this->gravity_dot.z;
	this->eye_angle3.z = this->gravity_dot.x * -1;

	// ƒˆ[Šp‚Í‘¼‚©‚ç‚à‚Á‚Ä‚­‚é.

	this->eye_angle1.x = this->gyro.GetIntegral().x / 30;

	this->eye_angle2.x = (this->compass.GetRelative().x) / 100 * -1;

	this->eye_angle3.x = this->madgwick.GetRelative().x;
	
	{
		this->eye_pos = EyePosition(
			this->eye_angle1.x * this->eye_pos_scale,
			this->eye_angle1.y * this->eye_pos_scale,
			this->eye_angle1.z);
	}


}




}