#pragma once

#include <vector>
#include <tuple>
#include <Siv3D.hpp>
#include "ThirdParty/ArduinoMadgwickAHRS/MadgwickAHRS.hpp"
#include "SensorVectorRelative.hpp"
#include "SensorVectorIntegral.hpp"

namespace ARWorkspace {
using EyePosition = std::tuple<int64_t, int64_t, double>;
class HMD_SensorAnalyzer
{
using Vector3AndTimestamp = std::tuple<double, double, double, uint64_t>;
using Float4AndTimestamp = std::tuple<float, float, float, float, uint64_t>;

public:
	HMD_SensorAnalyzer();

public:

	const EyePosition& GetEyePosition() const
	{
		return this->eye_pos;
	}

	void Update(const double delta_t);
	void ResetCenterAngle();

	void SetGravityVector(const Vector3AndTimestamp& arg_gravity, const double delta_t);
	void SetCompassVector(const Vector3AndTimestamp& arg_compass, const double delta_t);
	void SetGyroVector(const  Vector3AndTimestamp& arg_gyro, const double delta_t);
	void SetAccelVector(const  Vector3AndTimestamp& arg_accel, const double delta_t);
	void SetOrientationQuaternion(const  Float4AndTimestamp& arg_quaternion, const double delta_t);


	void SetEyeAngle(double arg_x, double arg_y, double arg_z);
	void SetEyePoint(int64_t arg_x, int64_t arg_y);

	void DrawSensorCursor(double x, double y, int offset_x, int offset_y, double eye_angle_scale, double angle, const s3d::String& name, s3d::Color color);


	void drawDebugString(int arg_x, int arg_y);
	bool IsDeviceRollFlat() const;
	bool IsDeviceCompassStartAngle() const;
	bool IsDeviceStartPosition() const;
	bool IsDeviceCompassCenterAngle() const;

	constexpr void SetDebugDisplayMode(bool arg)
	{
		this->debug_display_mode = arg;
	}
	constexpr bool IsDebugDisplayMode() const
	{
		return this->debug_display_mode;
	}

private:
	void updateEyePoint();
	void insertDebugString(const s3d::String& arg_string);


	inline bool IsDeviceStaticAngle() const
	{
		return this->gyro.GetInput().IsRange(0.0, 1.5);
	}
	inline void ResetGyroIntegral()
	{
		this->gyro.ResetIntegral();
	}

private:

	const s3d::Font	font = s3d::Font(40);

	s3d::Vec3	gravity;
	Vector3		gravity_dot; // 重力ベクトルの各規準軸との内積[cos].
	s3d::Vec3	gyro_raw; //to Madgwick Filter
	s3d::Vec3	orientation;
	s3d::Vec3	accel;

	SensorVectorIntegral	gyro;
	SensorVectorRelative	compass;
	SensorVectorRelative	madgwick;

	s3d::Vec3	eye_angle1;	// 重力内積+ジャイロ積分.
	s3d::Vec3	eye_angle2; // 重力内積+コンパス差分.
	s3d::Vec3	eye_angle3; // 重力内積＋Madgwickヨー角.
	EyePosition	eye_pos;
	double		eye_pos_scale = 1.0;
	
	double		device_roll_flat_margin = 0.20;
	double		device_nearly_compass_start_margin = 30.0;

	int		display_size_x = 800;
	int		display_size_y = 600;

	bool	debug_display_mode = true;

	std::vector<s3d::String> debug_strings = std::vector<s3d::String>(30);

	Madgwick::MadgwickFilter madgwick_filter;

};

}