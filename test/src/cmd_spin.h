#ifndef CMD_SPIN_H
#define CMD_SPIN_H

void printSummary()
{
    Serial.println("\r\n================MOTOR ENCODER READINGS================");
    Serial.print(labels[0]); Serial.print(encoders[0]->read()); Serial.print(" ");
    Serial.print(labels[1]); Serial.println(encoders[1]->read());
    Serial.print(labels[2]); Serial.print(encoders[2]->read()); Serial.print(" ");
    Serial.print(labels[3]); Serial.println(encoders[3]->read());
    Serial.println("");

    Serial.println("================COUNTS PER REVOLUTION=================");
    Serial.print(labels[0]); Serial.print(counts_per_rev[0]); Serial.print(" ");
    Serial.print(labels[1]); Serial.println(counts_per_rev[1]);
    Serial.print(labels[2]); Serial.print(counts_per_rev[2]); Serial.print(" ");
    Serial.print(labels[3]); Serial.println(counts_per_rev[3]);
    Serial.println("");

    Serial.println("====================MAX VELOCITIES====================");
    float max_rpm = kinematics.getMaxRPM();
    Kinematics::velocities max_linear  = kinematics.getVelocities( max_rpm,  max_rpm,  max_rpm,  max_rpm);
    Kinematics::velocities max_angular = kinematics.getVelocities(-max_rpm,  max_rpm, -max_rpm,  max_rpm);
    Serial.print("Linear Velocity: +- ");  Serial.print(max_linear.linear_x);  Serial.println(" m/s");
    Serial.print("Angular Velocity: +- "); Serial.print(max_angular.angular_z); Serial.println(" rad/s");
}

// spin command  → sampleMotors(false)
// sample command → sampleMotors(true)
void sampleMotors(bool show_summary)
{
    if (Kinematics::LUNA_BASE == Kinematics::DIFFERENTIAL_DRIVE)
        total_motors = 2;

    float measured_voltage = constrain(MOTOR_POWER_MEASURED_VOLTAGE, 0, MOTOR_OPERATING_VOLTAGE);
    float scaled_max_rpm   = (measured_voltage / MOTOR_OPERATING_VOLTAGE) * MOTOR_MAX_RPM;
    float total_rev        = scaled_max_rpm * (SAMPLE_TIME / 60.0);

    for (int i = 0; i < total_motors; i++)
    {
        Serial.print("SPINNING "); Serial.print(labels[i]);
        unsigned long start_time  = micros();
        unsigned long last_status = micros();
        encoders[i]->write(0);

        while (true)
        {
            if (micros() - start_time >= SAMPLE_TIME * 1000000UL)
            {
                motors[i]->spin(0);
                Serial.println("");
                break;
            }
            if (micros() - last_status >= 1000000UL)
            {
                last_status = micros();
                Serial.print(".");
            }
            delay(1);
            motors[i]->spin(PWM_MAX);
        }
        counts_per_rev[i] = encoders[i]->read() / total_rev;
    }
    if (show_summary)
        printSummary();
}

#endif
