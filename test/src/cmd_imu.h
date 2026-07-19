#ifndef CMD_IMU_H
#define CMD_IMU_H

void testIMU()
{
    Serial.println("Initializing IMU...");
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!imu.init())
    {
        Serial.println("ERROR: IMU init failed. Check wiring (SDA=" + String(SDA_PIN) + " SCL=" + String(SCL_PIN) + ").");
        return;
    }

    Serial.println("IMU OK. Calibrating gyro (keep robot still)...");
    Serial.println("Streaming IMU data. Press any key to stop.");
    Serial.println("");

    delay(100);
    while (Serial.available()) Serial.read();

    Serial.println("Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z");

    while (true)
    {
        if (Serial.available())
        {
            Serial.read();
            Serial.println("STOP - user interrupt");
            break;
        }

        sensor_msgs__msg__Imu data = imu.getData();

        Serial.print(data.linear_acceleration.x, 4); Serial.print(",");
        Serial.print(data.linear_acceleration.y, 4); Serial.print(",");
        Serial.print(data.linear_acceleration.z, 4); Serial.print(",");
        Serial.print(data.angular_velocity.x, 4);    Serial.print(",");
        Serial.print(data.angular_velocity.y, 4);    Serial.print(",");
        Serial.println(data.angular_velocity.z, 4);

        delay(50);  // 20 Hz
    }
}

#endif
