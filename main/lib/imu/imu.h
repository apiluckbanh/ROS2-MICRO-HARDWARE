// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com
#ifndef IMU_CONFIG_H
#define IMU_CONFIG_H

#include "default_imu.h"

#ifdef IMU_MPU6050
    #define IMU MPU6050IMU
#endif

#ifndef IMU
    #define USE_FAKE_IMU
    #define IMU FakeIMU
#endif

#endif
