// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com

#ifndef LULU_ROBOT_H
#define LULU_ROBOT_H
#define LUNA_ROBOT
#define LED_PIN 2
#define LUNA_BASE MECANUM
#define CYTRON_MD10C_MOTOR_DRIVER
#define IMU_MPU6050
#define SDA_PIN 21
#define SCL_PIN 22

#define K_P1 0.01
#define K_I1 3.17
#define K_D1 0.00

#define K_P2 0.01
#define K_I2 3.19
#define K_D2 0.00

#define K_P3 0.01
#define K_I3 3.19
#define K_D3 0.00

#define K_P4 0.01
#define K_I4 3.07
#define K_D4 0.00
#define MOTOR_MAX_RPM 80
#define MAX_RPM_RATIO 0.6
#define MOTOR_OPERATING_VOLTAGE 12
#define MOTOR_POWER_MAX_VOLTAGE 12
#define MOTOR_POWER_MEASURED_VOLTAGE 12
#define COUNTS_PER_REV1 665
#define COUNTS_PER_REV2 669
#define COUNTS_PER_REV3 670
#define COUNTS_PER_REV4 696
#define WHEEL_DIAMETER 0.100
#define LR_WHEELS_DISTANCE 0.290
#define PWM_BITS 8
#define PWM_FREQUENCY 8000

// ENCODER PINS
#define MOTOR1_ENCODER_A 4
#define MOTOR1_ENCODER_B 16
#define MOTOR1_ENCODER_INV false

#define MOTOR2_ENCODER_A 14
#define MOTOR2_ENCODER_B 15
#define MOTOR2_ENCODER_INV true

#define MOTOR3_ENCODER_A 17
#define MOTOR3_ENCODER_B 5
#define MOTOR3_ENCODER_INV false

#define MOTOR4_ENCODER_A 18
#define MOTOR4_ENCODER_B 19
#define MOTOR4_ENCODER_INV true

// MOTOR PINS — Cytron MD10C: PWM = speed, IN_A = DIR, IN_B unused
#define MOTOR1_PWM  13
#define MOTOR1_IN_A 12
#define MOTOR1_IN_B -1
#define MOTOR1_INV  true

#define MOTOR2_PWM  23
#define MOTOR2_IN_A 27
#define MOTOR2_IN_B -1
#define MOTOR2_INV  false

#define MOTOR3_PWM  26
#define MOTOR3_IN_A 25
#define MOTOR3_IN_B -1
#define MOTOR3_INV  true

#define MOTOR4_PWM  33
#define MOTOR4_IN_A 32
#define MOTOR4_IN_B -1
#define MOTOR4_INV  false

#define PWM_MAX pow(2, PWM_BITS) - 1
#define PWM_MIN -(pow(2, PWM_BITS) - 1)

#define NODE_NAME "lulu_robot_base_node"
#endif
