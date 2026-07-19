#ifndef CMD_TICKS_H
#define CMD_TICKS_H

void testMotorForTicksPerRevolution()
{
    if (Kinematics::LUNA_BASE == Kinematics::DIFFERENTIAL_DRIVE)
        total_motors = 2;

    const int PWM_FOR_TEST = 200;
    bool show_incremental  = false;

    Serial.println("Please ensure that the robot is ELEVATED and there are NO OBSTRUCTIONS to the wheels.");
    Serial.println("ticks test will run each motor at slow speed one motor at a time.");
    Serial.println("count the number of revolutions visually and make a note of final tick count for each motor.");
    Serial.println("ticks per rev = final tick count / number of revolutions counted");
    Serial.println("Press enter to continue to tick count test.");
    Serial.println("");
    delay(100);
    while (Serial.available()) Serial.read();
    while (!Serial.available()) {}  // wait for user to press enter
    while (Serial.available()) Serial.read();

    for (int i = 0; i < total_motors; i++)
    {
        Serial.print(labels[i]);
        unsigned long start_time = micros();

        while (true)
        {
            if (micros() - start_time >= SAMPLE_TIME * 1000000UL)
            {
                motors[i]->brake();
                start_time = micros();
                break;
            }
            float current_tick_count = encoders[i]->read();
            if (show_incremental)
            {
                Serial.print("current_tick_count:: "); Serial.println(current_tick_count);
            }
            else
            {
                Serial.print(".");
            }
            motors[i]->spin(PWM_FOR_TEST);
            delay(1000);
        }

        Serial.println("");
        Serial.print("final_tick_count for "); Serial.print(labels[i]); Serial.print(" = ");
        Serial.println(encoders[i]->read());
        Serial.println("=============");
    }
}

#endif
