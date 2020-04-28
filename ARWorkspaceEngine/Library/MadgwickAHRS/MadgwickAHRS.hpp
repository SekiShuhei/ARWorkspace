//=============================================================================================
// MadgwickAHRS
//=============================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
//
// From the x-io website "Open-source resources available on this website are
// provided under the GNU General Public Licence unless an alternative licence
// is provided in source."
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 19/02/2012	SOH Madgwick	Magnetometer measurement is normalised
//
//=============================================================================================
// https://os.mbed.com/users/Gaku0606/
// Author : Gaku Matsumoto
// https://github.com/SekiShuhei/ARWorkspace
// Author : Shuhei Seki

#pragma once

#include <math.h>
#include <tuple>

#define BETA_DEF 0.1

namespace Madgwick {
class MadgwickFilter 
{

public:
    // この値を大きくすると重力の影響を大きく取るようになります．
    MadgwickFilter(double B = BETA_DEF);

public:
    // MadgwickFilterによって角速度・加速度・地磁気データを統合し，姿勢計算します．
    // gx,gy,gz    ジャイロ角速度データ[rad\s].
    // ax,ay,az    加速度データ, 特に規格化は必要ありません
    // mx,my,mz    地磁気データ, キャリブレーションを確実に行って下さい．
    void MadgwickAHRSupdate(
        double gx, double gy, double gz, 
        double ax, double ay, double az, 
        double mx, double my, double mz, double delta_t);

    // MadgwickFilterを角速度と加速度のみで動かし，姿勢計算を更新します．
    // 通常の関数でも，地磁気成分を0.0にすればこの関数が呼ばれます．
    void MadgwickAHRSupdateIMU(
        double gx, double gy, double gz, 
        double ax, double ay, double az, double delta_t);

    
    [[nodiscord]]
    inline std::tuple<double, double, double, double> GetQuaternion() const noexcept
    {
        return std::tuple<double, double, double, double>(this->q0, this->q1, this->q2, this->q3);
    }

    //void getEulerAngle(double* val); // old.
    
    [[nodiscord]]
    inline std::tuple<double, double, double> GetEulerAngleRad() const noexcept
    {
        double q0q0 = q0 * q0;
        double q1q1q2q2 = q1 * q1 - q2 * q2;
        double q3q3 = q3 * q3;
        double roll  = (atan2(2.0f * (q0 * q1 + q2 * q3), q0q0 - q1q1q2q2 + q3q3));
        double pitch = (-asin(2.0f * (q1 * q3 - q0 * q2)));
        double yaw   = (atan2(2.0f * (q1 * q2 + q0 * q3), q0q0 + q1q1q2q2 - q3q3));
        return std::tuple<double, double, double>(roll, pitch, yaw);
    }

private:
    double q0, q1, q2, q3;  // quaternion.
    double beta;
};

MadgwickFilter::MadgwickFilter(double B) {
    beta = B;
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}

//void MadgwickFilter::getEulerAngle(double* val) {
//    double q0q0 = q0 * q0, q1q1q2q2 = q1 * q1 - q2 * q2, q3q3 = q3 * q3;
//    val[0] = (atan2(2.0f * (q0 * q1 + q2 * q3), q0q0 - q1q1q2q2 + q3q3));
//    val[1] = (-asin(2.0f * (q1 * q3 - q0 * q2)));
//    val[2] = (atan2(2.0f * (q1 * q2 + q0 * q3), q0q0 + q1q1q2q2 - q3q3));
//}

//---------------------------------------------------------------------------------------------------
// AHRS algorithm update

inline void MadgwickFilter::MadgwickAHRSupdate(
    double gx, double gy, double gz, 
    double ax, double ay, double az, 
    double mx, double my, double mz, double delta_t) {

    double acc_norm;
    //static double delta_t = 0;
    static unsigned int newTime = 0, oldTime = 0;
    double recipNorm;
    double s0, s1, s2, s3;
    double qDot1, qDot2, qDot3, qDot4;
    double hx, hy;
    double _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) 
    {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az, delta_t);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        acc_norm = sqrt(ax * ax + ay * ay + az * az);
        recipNorm = 1.0 / acc_norm;
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = 1.0 / sqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = 1.0 / sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        double deltaA = fabs(acc_norm - 1.00);
        //beta = 0.1*exp(-1.0*deltaA*deltaA);
        //beta = 0.3*exp(-20.0*deltaA*deltaA);
        beta = 0.3 * exp(-30.0 * deltaA * deltaA);
        //printf("%f\r\n", beta);
        //beta = 0.1;
        //if(deltaA > 0.3)    beta = 0.0;
        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    q0 += qDot1 * delta_t;//(1.0f / sampleFreq);
    q1 += qDot2 * delta_t;//(1.0f / sampleFreq);
    q2 += qDot3 * delta_t;//(1.0f / sampleFreq);
    q3 += qDot4 * delta_t;//(1.0f / sampleFreq);

    // Normalise quaternion
    recipNorm = 1.0 / sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

inline void MadgwickFilter::MadgwickAHRSupdateIMU(
    double gx, double gy, double gz, 
    double ax, double ay, double az, double delta_t) 
{
    //static double delta_t = 0;
    static unsigned int newTime = 0, oldTime = 0;
    double recipNorm;
    double s0, s1, s2, s3;
    double qDot1, qDot2, qDot3, qDot4;
    double _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
    double acc_norm;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        acc_norm = sqrt(ax * ax + ay * ay + az * az);
        recipNorm = 1.0 / acc_norm;
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
        recipNorm = 1.0 / sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        double deltaA = fabs(acc_norm - 1.00);
        //beta = 0.5*exp(-20.0*deltaA*deltaA);
        if (deltaA > 0.3) beta = 0.0;
        else    beta = 0.1;
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    q0 += qDot1 * delta_t;;
    q1 += qDot2 * delta_t;;
    q2 += qDot3 * delta_t;;
    q3 += qDot4 * delta_t;;

    // Normalise quaternion
    recipNorm = 1.0 / sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

};

}