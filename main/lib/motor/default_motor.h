// Copyright (c) 2026 Apiluck Noppkun. All rights reserved.
// This source code is provided for personal use only.
// Redistribution, resale, or commercial use without written
// permission from the author is strictly prohibited.
// Contact: apiluck.banh@gmail.com


#ifndef DEFAULT_MOTOR
#define DEFAULT_MOTOR

#include <Arduino.h>
#include "config.h"
#if defined(PICO)
inline void analogWriteFrequency(double frequency)
{
  analogWriteFreq(frequency);
}
inline void analogWriteFrequency(uint8_t pin, double frequency)
{
  analogWriteFreq(frequency);
}
#else
// ESP32 Arduino 3.x dropped the pin argument — provide two-arg wrapper for call-site compatibility
inline void analogWriteFrequency(uint8_t pin, double frequency)
{
  analogWriteFrequency((uint32_t)frequency);
}
#endif

#include "motor_interface.h"


class CytronMD10C: public MotorInterface
{
    private:
        int in_pin_;
        int pwm_pin_;

    protected:
        void forward(int pwm) override
        {
            if (in_pin_ < 0) return;
            digitalWrite(in_pin_, HIGH);
            analogWrite(pwm_pin_, abs(pwm));
        }

        void reverse(int pwm) override
        {
            if (in_pin_ < 0) return;
            digitalWrite(in_pin_, LOW);
            analogWrite(pwm_pin_, abs(pwm));
        }

    public:
        CytronMD10C(float pwm_frequency, int pwm_bits, bool invert, int pwm_pin, int in_pin, int unused=-1):
            MotorInterface(invert),
            in_pin_(in_pin),
            pwm_pin_(pwm_pin)
        {
            if (in_pin_ < 0) return;
            pinMode(in_pin_, OUTPUT);
            pinMode(pwm_pin_, OUTPUT);

            if (pwm_frequency > 0)
                analogWriteFrequency(pwm_pin_, pwm_frequency);
            analogWriteResolution(pwm_bits);

            analogWrite(pwm_pin_, 0);
        }

        void brake() override
        {
            if (in_pin_ < 0) return;
            analogWrite(pwm_pin_, 0);
        }
};

// ── LUNA_MOTOR_DRIVE : dual PWM on IN_A / IN_B ────────────────
class LUNA_MOTOR_DRIVE: public MotorInterface
{
    private:
        int in_a_pin_;
        int in_b_pin_;
        int pwm_max_;

    protected:
        void forward(int pwm) override
        {
	    if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            analogWrite(in_a_pin_, pwm_max_ - abs(pwm));
            analogWrite(in_b_pin_, pwm_max_); // short brake
#else
            analogWrite(in_a_pin_, 0);
            analogWrite(in_b_pin_, abs(pwm));
#endif
        }

        void reverse(int pwm) override
        {
	    if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            analogWrite(in_b_pin_, pwm_max_ - abs(pwm));
            analogWrite(in_a_pin_, pwm_max_); // short brake
#else
            analogWrite(in_b_pin_, 0);
            analogWrite(in_a_pin_, abs(pwm));
#endif
        }

    public:
        LUNA_MOTOR_DRIVE(float pwm_frequency, int pwm_bits, bool invert, int unused, int in_a_pin, int in_b_pin):
            MotorInterface(invert),
            in_a_pin_(in_a_pin),
            in_b_pin_(in_b_pin)
        {
	    if (in_a_pin_ < 0) return;
            pwm_max_ = (1 << pwm_bits) - 1;
            pinMode(in_a_pin_, OUTPUT);
            pinMode(in_b_pin_, OUTPUT);

            if(pwm_frequency > 0)
            {
                analogWriteFrequency(in_a_pin_, pwm_frequency);
                analogWriteFrequency(in_b_pin_, pwm_frequency);

            }
            analogWriteResolution(pwm_bits);

            //ensure that the motor is in neutral state during bootup
            analogWrite(in_a_pin_, 0);
            analogWrite(in_b_pin_, 0);
        }

        LUNA_MOTOR_DRIVE(float pwm_frequency, int pwm_bits, bool invert, int in_a_pin, int in_b_pin):
            MotorInterface(invert),
            in_a_pin_(in_a_pin),
            in_b_pin_(in_b_pin)
        {
	    if (in_a_pin_ < 0) return;
            pwm_max_ = (1 << pwm_bits) - 1;
            pinMode(in_a_pin_, OUTPUT);
            pinMode(in_b_pin_, OUTPUT);

            if(pwm_frequency > 0)
            {
                analogWriteFrequency(in_a_pin_, pwm_frequency);
                analogWriteFrequency(in_b_pin_, pwm_frequency);

            }
            analogWriteResolution(pwm_bits);

            //ensure that the motor is in neutral state during bootup
            analogWrite(in_a_pin_, 0);
            analogWrite(in_b_pin_, 0);
        }

        void brake() override
        {
	    if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            analogWrite(in_a_pin_, pwm_max_);
            analogWrite(in_b_pin_, pwm_max_); // short brake
#else
            analogWrite(in_b_pin_, 0);
            analogWrite(in_a_pin_, 0);
#endif
        }
};

#endif
