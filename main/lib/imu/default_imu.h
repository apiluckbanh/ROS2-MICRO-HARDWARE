// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com
#ifndef DEFAULT_IMU
#define DEFAULT_IMU

#include "imu_interface.h"
#include "MPU6050.h"

class MPU6050IMU: public IMUInterface
{
    private:
        const float accel_scale_ = 1 / 16384.0;
        const float gyro_scale_ = 1 / 131.0;

        MPU6050 accelerometer_;
        MPU6050 gyroscope_;

        geometry_msgs__msg__Vector3 accel_;
        geometry_msgs__msg__Vector3 gyro_;

    public:
        MPU6050IMU()
        {
        }

        bool startSensor() override
        {
            Wire.begin();
            bool ret;
            accelerometer_.initialize();
            ret = accelerometer_.testConnection();
            if(!ret)
                return false;

            gyroscope_.initialize();
            ret = gyroscope_.testConnection();
            if(!ret)
                return false;

            return true;
        }

        geometry_msgs__msg__Vector3 readAccelerometer() override
        {
            int16_t ax, ay, az;

            accelerometer_.getAcceleration(&ax, &ay, &az);

            accel_.x = ax * (double) accel_scale_ * g_to_accel_;
            accel_.y = ay * (double) accel_scale_ * g_to_accel_;
            accel_.z = az * (double) accel_scale_ * g_to_accel_;

            return accel_;
        }

        geometry_msgs__msg__Vector3 readGyroscope() override
        {
            int16_t gx, gy, gz;

            gyroscope_.getRotation(&gx, &gy, &gz);

            gyro_.x = gx * (double) gyro_scale_ * DEG_TO_RAD;
            gyro_.y = gy * (double) gyro_scale_ * DEG_TO_RAD;
            gyro_.z = gz * (double) gyro_scale_ * DEG_TO_RAD;

            return gyro_;
        }
};

class FakeIMU: public IMUInterface
{
    private:
        geometry_msgs__msg__Vector3 accel_;
        geometry_msgs__msg__Vector3 gyro_;

    public:
        FakeIMU()
        {
        }

        bool startSensor() override
        {
            return true;
        }

        geometry_msgs__msg__Vector3 readAccelerometer() override
        {
            return accel_;
        }

        geometry_msgs__msg__Vector3 readGyroscope() override
        {
            return gyro_;
        }
};

#endif
