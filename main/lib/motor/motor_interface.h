// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com



#ifndef MOTOR_INTERFACE
#define MOTOR_INTERFACE

class MotorInterface
{
    bool invert_;
    protected:
        virtual void forward(int pwm) = 0;
        virtual void reverse(int pwm) = 0;

    public:
        MotorInterface(int invert):
            invert_(invert)
        {
        }

        virtual void brake() = 0;
        void spin(int pwm)
        {
            if(invert_)
                pwm *= -1;

            if(pwm > 0)
                forward(pwm);
            else if(pwm < 0)
                reverse(pwm);
            else
                brake();
        }
};

#endif