#pragma once

#include <vector>
#include <tuple>
#include <Siv3D.hpp>
#include "ThirdParty/ArduinoMadgwickAHRS/MadgwickAHRS.hpp"
#include "SensorVectorRelative.hpp"
#include "SensorVectorIntegral.hpp"

namespace ARWorkspace {
class HMD_SensorAnalyzer
{
using Vector3AndTimestamp = std::tuple<double, double, double, uint64_t>;
using Float4AndTimestamp = std::tuple<float, float, float, float, uint64_t>;

public:
	HMD_SensorAnalyzer();

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
	bool IsDeviceNearlyStartAngle() const;
	bool IsDeviceNearlyCompassCenterAngle() const;

	std::tuple<int64_t, int64_t> GetEyePoint() const
	{
		return std::tuple<int64_t, int64_t>(0,0);
	}

private:
	void updateEyePoint();


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
	Vector3		gravity_dot; // �d�̓x�N�g���̊e�K�����Ƃ̓���[cos].
	s3d::Vec3	gyro_raw; //to Madgwick Filter
	s3d::Vec3	orientation;
	s3d::Vec3	accel;

	SensorVectorIntegral	gyro;
	SensorVectorRelative	compass;
	SensorVectorRelative	madgwick;

	s3d::Vec3	eye_point1;	// �d�͓���+�W���C���ϕ�.
	s3d::Vec3	eye_point2; // �d�͓���+�R���p�X����.
	s3d::Vec3	eye_point3; // �d�͓��ρ{Madgwick���[�p.

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