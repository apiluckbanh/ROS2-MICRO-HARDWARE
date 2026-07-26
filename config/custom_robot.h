// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com

#ifndef CUSTOM_ROBOT_H
#define CUSTOM_ROBOT_H
#define LUNA_ROBOT
#define LED_PIN 2
#define LUNA_BASE MECANUM   
#define LUNA_MOTOR_DRIVER          
// #define CYTRON_MD10C_MOTOR_DRIVER   // Cytron MD10C / MD13S  (PWM + DIR)

#define IMU_MPU6050

#define SDA_PIN 21
#define SCL_PIN 22

// ── PID GAINS ─────────────────────────────────────────────────────────────────
#define K_P1 0.01
#define K_I1 0.8
#define K_D1 0.2

#define K_P2 0.01
#define K_I2 0.8
#define K_D2 0.2

#define K_P3 0.01
#define K_I3 0.8
#define K_D3 0.2

#define K_P4 0.01
#define K_I4 0.8
#define K_D4 0.2

// ── MOTOR SPECS ───────────────────────────────────────────────────────────────
#define MOTOR_MAX_RPM              60
#define MAX_RPM_RATIO             0.8
#define MOTOR_OPERATING_VOLTAGE     6
#define MOTOR_POWER_MAX_VOLTAGE    12
#define MOTOR_POWER_MEASURED_VOLTAGE 12

// ── WHEEL & ROBOT GEOMETRY ────────────────────────────────────────────────────
#define WHEEL_DIAMETER      0.065   // meters
#define LR_WHEELS_DISTANCE  0.1975  // meters (left-right wheel center distance)

// ── ENCODER CPR (counts per revolution, full quadrature) ─────────────────────
#define COUNTS_PER_REV1 1200
#define COUNTS_PER_REV2 1200
#define COUNTS_PER_REV3 1200
#define COUNTS_PER_REV4 1200

//  PWM 
#define PWM_BITS      8
#define PWM_FREQUENCY 8000   // Hz

//  ENCODER PINS 
#define MOTOR1_ENCODER_A   34
#define MOTOR1_ENCODER_B   35
#define MOTOR1_ENCODER_INV false

#define MOTOR2_ENCODER_A   36
#define MOTOR2_ENCODER_B   39
#define MOTOR2_ENCODER_INV false

#define MOTOR3_ENCODER_A   32
#define MOTOR3_ENCODER_B   33
#define MOTOR3_ENCODER_INV false

#define MOTOR4_ENCODER_A   25
#define MOTOR4_ENCODER_B   26
#define MOTOR4_ENCODER_INV false

// LUNA_MOTOR_DRIVER    → PWM=-1  IN_A=<pwm_a>  IN_B=<pwm_b>
// CYTRON_MD10C_DRIVER  → PWM=<pwm>  IN_A=<dir>  IN_B=-1

#define MOTOR1_PWM  -1
#define MOTOR1_IN_A 18
#define MOTOR1_IN_B 17
#define MOTOR1_INV  false

#define MOTOR2_PWM  -1
#define MOTOR2_IN_A 16
#define MOTOR2_IN_B 15
#define MOTOR2_INV  false

#define MOTOR3_PWM  -1
#define MOTOR3_IN_A 7
#define MOTOR3_IN_B 6
#define MOTOR3_INV  false

#define MOTOR4_PWM  -1
#define MOTOR4_IN_A 4
#define MOTOR4_IN_B 5
#define MOTOR4_INV  false

// ─────────────────────────────────────────────────────────────────────────────
#define PWM_MAX pow(2, PWM_BITS) - 1
#define PWM_MIN -(pow(2, PWM_BITS) - 1)

#define NODE_NAME "custom_robot_base_node"
#endif
