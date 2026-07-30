#include <Arduino.h>
#include <Wire.h>
#include <micro_ros_platformio.h>
#include <stdio.h>

#include <geometry_msgs/msg/twist.h>
#include <sensor_msgs/msg/imu.h>
#ifdef LULU_ROBOT_CONFIG
    #include "lulu_robot.h"
#elif defined(CUSTOM_ROBOT_CONFIG)
    #include "custom_robot.h"
#else
    #include "luna_robot.h"
#endif
#include "imu.h"
#include "motor.h"
#include "pid.h"

#define ENCODER_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS
#include "encoder.h"
#include "kinematics.h"

#ifndef BAUDRATE
#define BAUDRATE 115200
#endif
#define SAMPLE_TIME 5  // seconds

Encoder motor1_encoder(MOTOR1_ENCODER_A, MOTOR1_ENCODER_B, COUNTS_PER_REV1, MOTOR1_ENCODER_INV);
Encoder motor2_encoder(MOTOR2_ENCODER_A, MOTOR2_ENCODER_B, COUNTS_PER_REV2, MOTOR2_ENCODER_INV);
Encoder motor3_encoder(MOTOR3_ENCODER_A, MOTOR3_ENCODER_B, COUNTS_PER_REV3, MOTOR3_ENCODER_INV);
Encoder motor4_encoder(MOTOR4_ENCODER_A, MOTOR4_ENCODER_B, COUNTS_PER_REV4, MOTOR4_ENCODER_INV);

LUNA_MOTOR_DRIVE motor1_controller(PWM_FREQUENCY, PWM_BITS, MOTOR1_INV, MOTOR1_PWM, MOTOR1_IN_A, MOTOR1_IN_B);
LUNA_MOTOR_DRIVE motor2_controller(PWM_FREQUENCY, PWM_BITS, MOTOR2_INV, MOTOR2_PWM, MOTOR2_IN_A, MOTOR2_IN_B);
LUNA_MOTOR_DRIVE motor3_controller(PWM_FREQUENCY, PWM_BITS, MOTOR3_INV, MOTOR3_PWM, MOTOR3_IN_A, MOTOR3_IN_B);
LUNA_MOTOR_DRIVE motor4_controller(PWM_FREQUENCY, PWM_BITS, MOTOR4_INV, MOTOR4_PWM, MOTOR4_IN_A, MOTOR4_IN_B);

PID motor1_pid(PWM_MIN, PWM_MAX, K_P1, K_I1, K_D1);
PID motor2_pid(PWM_MIN, PWM_MAX, K_P2, K_I2, K_D2);
PID motor3_pid(PWM_MIN, PWM_MAX, K_P3, K_I3, K_D3);
PID motor4_pid(PWM_MIN, PWM_MAX, K_P4, K_I4, K_D4);

Kinematics kinematics(
    Kinematics::LUNA_BASE,
    MOTOR_MAX_RPM, MAX_RPM_RATIO,
    MOTOR_OPERATING_VOLTAGE, MOTOR_POWER_MAX_VOLTAGE,
    WHEEL_DIAMETER, LR_WHEELS_DISTANCE);

IMU imu;

long long int counts_per_rev[4];
int total_motors = 4;
LUNA_MOTOR_DRIVE   *motors[4]   = {&motor1_controller, &motor2_controller, &motor3_controller, &motor4_controller};
Encoder *encoders[4] = {&motor1_encoder,    &motor2_encoder,    &motor3_encoder,    &motor4_encoder};
PID     *pids[4]     = {&motor1_pid,        &motor2_pid,        &motor3_pid,        &motor4_pid};
String   labels[4]   = {"FRONT LEFT - M1: ", "FRONT RIGHT - M2: ", "REAR LEFT - M3: ", "REAR RIGHT - M4: "};

#include "cmd_spin.h"
#include "cmd_ticks.h"
#include "cmd_test.h"
#include "cmd_testall.h"
#include "cmd_imu.h"

void setup()
{
    Serial.begin(BAUDRATE);
    while (!Serial) {}
    Serial.println("Sampling process will spin the motors at its maximum RPM.");
    Serial.println("Please ensure that the robot is ELEVATED and there are NO OBSTRUCTIONS to the wheels.");
    Serial.println("");
    Serial.println("Type 'spin'   - spin motors at max RPM");
    Serial.println("Type 'sample' - spin motors and show summary");
    Serial.println("Type 'ticks'  - measure ticks per revolution");
    Serial.println("Type 'test'   - test PID with cmd_vel");
    Serial.println("Type 'testall' - run all 4 motors and plot data");
    Serial.println("Type 'imu'    - stream IMU accel/gyro data");
    Serial.println("Press enter to clear command.");
    Serial.println("");
}

void loop()
{
    static String cmd = "";

    while (Serial.available())
    {
        char character = Serial.read();
        cmd.concat(character);
        Serial.print(character);
        delay(1);

        if (character != '\r') continue;

        Serial.println("");
        if      (cmd.equals("spin\r"))   { cmd = ""; sampleMotors(false); }
        else if (cmd.equals("sample\r")) { cmd = ""; sampleMotors(true); }
        else if (cmd.equals("ticks\r"))  { cmd = ""; testMotorForTicksPerRevolution(); }
        else if (cmd.equals("test\r"))    { cmd = ""; testMotorsWithCmdVel(); }
        else if (cmd.equals("testall\r")) { cmd = ""; testAllMotors(); }
        else if (cmd.equals("imu\r"))     { cmd = ""; testIMU(); }
        else                              { cmd = ""; }
    }
}
