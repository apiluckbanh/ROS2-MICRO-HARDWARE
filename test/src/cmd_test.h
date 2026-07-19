#ifndef CMD_TEST_H
#define CMD_TEST_H

void testMotorsWithCmdVel()
{
    if (Kinematics::LUNA_BASE == Kinematics::DIFFERENTIAL_DRIVE)
        total_motors = 2;

    geometry_msgs__msg__Twist twist_msg;
    twist_msg.linear.x  = 0.5;
    twist_msg.linear.y  = 0.0;
    twist_msg.angular.z = 0.0;

    for (int i = 0; i < total_motors; i++)
    {
        unsigned long start_time = micros();
        while (true)
        {
            if (micros() - start_time >= SAMPLE_TIME * 1000000UL)
            {
                Serial.println("STOP motor");
                motors[i]->brake();
                Serial.println("=============");
                break;
            }

            Kinematics::rpm req_rpm = kinematics.getRPM(
                twist_msg.linear.x, twist_msg.linear.y, twist_msg.angular.z);

            float req_rpm_val = 0;
            switch (i)
            {
                case 0: req_rpm_val = req_rpm.motor1; break;
                case 1: req_rpm_val = req_rpm.motor2; break;
                case 2: req_rpm_val = req_rpm.motor3; break;
                case 3: req_rpm_val = req_rpm.motor4; break;
            }

            float current_rpm = encoders[i]->getRPM();
            int   pwm         = pids[i]->compute(req_rpm_val, current_rpm);

            Serial.print("req_rpm:: ");    Serial.print(req_rpm_val);
            Serial.print(" current_rpm:: "); Serial.print(current_rpm);
            Serial.print(" pwm:: ");       Serial.println(pwm);

            motors[i]->spin(pwm);
            delay(100);
        }
    }
}

#endif
