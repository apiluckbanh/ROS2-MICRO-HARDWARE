// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com


#ifndef MOTOR_H
#define MOTOR_H

#include "default_motor.h"

#if defined(LUNA_MOTOR_DRIVER)
    // use LUNA_MOTOR_DRIVE class (dual PWM, BTS7960-style) — defined in default_motor.h
#elif defined(CYTRON_MD10C_MOTOR_DRIVER)
    #define LUNA_MOTOR_DRIVE CytronMD10C
#else
    #error "No motor driver selected. Define LUNA_MOTOR_DRIVER or CYTRON_MD10C_MOTOR_DRIVER in luna_robot.h"
#endif

#endif
