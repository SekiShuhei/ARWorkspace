#pragma once

#include <vector>
#include <tuple>
#include <Siv3D.hpp>
#include "ThirdParty/ArduinoMadgwickAHRS/MadgwickAHRS.hpp"


namespace ARWorkspace {
class ARWorkspace
{
using Vector3AndTimestamp = std::tuple<double, double, double, uint64_t>;
using Float4AndTimestamp = std::tuple<float, float, float, float, uint64_t>;

public:
	ARWorkspace();

public:

	void Update(const double delta_t);

	void SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t);
	void SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t);
	void SetGyroVector(const  Vector3AndTimestamp& arg_gyro, const double delta_t);
	void SetAccelVector(const  Vector3AndTimestamp& arg_accel, const double delta_t);
	void SetOrientationQuaternion(const  Float4AndTimestamp& arg_quaternion, const double delta_t);


	void SetEyeAngle(double arg_x, double arg_y, double arg_z);
	void SetEyePoint(int64_t arg_x, int64_t arg_y);

	void DrawSensorCursor(double x, double y, int offset_x, int offset_y, double display_scale, double angle, const s3d::String& name, s3d::Color color);

	void DebugString(const s3d::String& arg_string);

	void drawDebugString(int arg_x, int arg_y);
	bool IsDeviceRollFlat() const;
	bool IsDeviceNearlyCompassStartAngle() const;

	std::tuple<int64_t, int64_t> GetEyePoint() const
	{
		return std::tuple<int64_t, int64_t>(0,0);
	}

private:
	void updateEyePoint();

	inline static bool IsApprox(double base, double target, double margin)
	{
		return (base > target - margin && base < target + margin);
	}
	inline static double Smoothing(double base, double target, double ratio)
	{
		if (ratio <= 0.0)
		{
			return base;
		}
		ratio = (ratio > 1.0 ? 1.0 : ratio);
		return base + ((target - base) / ratio);
	}


private:

	const s3d::Font	font = s3d::Font(40);

	s3d::Vec3	gravity;
	s3d::Vec3	gravity_dot; // �d�̓x�N�g���̊e�K�����Ƃ̓���[cos].
	s3d::Vec3	compass;
	s3d::Vec3	gyro;
	s3d::Vec3	gyro_raw; //to Madgwick Filter
	s3d::Vec3	gyro_integral;
	s3d::Vec3	orientation;
	s3d::Vec3	accel;

	bool		compass_startup_initialized = false;
	s3d::Vec3	compass_startup;
	s3d::Vec3	eye_point1;	// �d�͓���+�W���C���ϕ�.
	s3d::Vec3	eye_point2; // �d�͓���+�R���p�X����.
	double		compass_diff = 0.0;
	double		compass_diff_integral = 0.0;

	double		device_roll_flat_margin = 0.20;
	double		device_nearly_compass_start_margin = 30.0;



	double	start_angle_x = 0.0;
	double	start_angle_y = 0.0;
	double	start_angle_z = 0.0;
	
	double	display_scale = 1.0;
	int		display_size_x = 800;
	int		display_size_y = 600;


	std::vector<s3d::String> debug_strings = std::vector<s3d::String>(30);


	Madgwick::MadgwickFilter madgwick_filter;

};

}