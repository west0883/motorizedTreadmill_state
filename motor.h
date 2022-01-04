#pragma once

#include <limits.h>
#include <stddef.h>

#include <AccelStepper.h>

class Motor
{
    private:
        enum class State
        {
            Idle,
            Running,
            Decelerating,
            Stopping,
            Accelerating
        };
        
        enum State state = State::Idle;

        AccelStepper stepperMotor;

        // Which pin controls our sleeping
        static constexpr const size_t SleepPowerPin = 4;

        // Acceleration and deceleration of stepper motor
        static constexpr const float StepperAccell = 1000;

        // The arbitrary number of steps you want the motor to aim for 
        static constexpr const long nSteps = LONG_MAX;

        // The number of steps you want the motor to use for calculating deccel.
        static constexpr const long stopSteps = 500;

        // The next target speed
        float targetSpeed = 0;
        
    public:
        Motor(void);

        void Start(float speed);
        void Stop(void);
        
        void RunOnce(void);
};
