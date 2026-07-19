#ifndef CMD_TESTALL_H
#define CMD_TESTALL_H

void testAllMotors()
{
    geometry_msgs__msg__Twist twist_msg;
    twist_msg.linear.x  = 0.5;
    twist_msg.linear.y  = 0.0;
    twist_msg.angular.z = 0.0;

    Serial.println("Running all 4 motors simultaneously.");
    Serial.println("Output format (Serial Plotter): M1_req,M1_cur,M2_req,M2_cur,M3_req,M3_cur,M4_req,M4_cur");
    Serial.println("Type any key to stop.");
    Serial.println("");

    // drain leftover bytes from the command input ('\n' etc.)
    delay(100);
    while (Serial.available()) Serial.read();

    // header for Serial Plotter / MATLAB
    Serial.println("M1_req,M1_cur,M1_pwm,M2_req,M2_cur,M2_pwm,M3_req,M3_cur,M3_pwm,M4_req,M4_cur,M4_pwm");

    unsigned long start_time = micros();

    while (true)
    {
        if (micros() - start_time >= SAMPLE_TIME * 1000000UL)
        {
            for (int i = 0; i < 4; i++) motors[i]->brake();
            Serial.println("STOP - time limit reached");
            break;
        }
        if (Serial.available())
        {
            Serial.read();
            for (int i = 0; i < 4; i++) motors[i]->brake();
            Serial.println("STOP - user interrupt");
            break;
        }

        Kinematics::rpm req_rpm = kinematics.getRPM(
            twist_msg.linear.x, twist_msg.linear.y, twist_msg.angular.z);

        float req[4] = {req_rpm.motor1, req_rpm.motor2, req_rpm.motor3, req_rpm.motor4};
        float cur[4];
        int   pwm[4];

        for (int i = 0; i < 4; i++)
        {
            cur[i] = encoders[i]->getRPM();
            pwm[i] = pids[i]->compute(req[i], cur[i]);
            motors[i]->spin(pwm[i]);
        }

        // CSV line: req,cur,pwm for each motor
        for (int i = 0; i < 4; i++)
        {
            Serial.print(req[i]); Serial.print(",");
            Serial.print(cur[i]); Serial.print(",");
            Serial.print(pwm[i]);
            if (i < 3) Serial.print(",");
        }
        Serial.println();

        delay(50);  // 20 Hz
    }
}

#endif
