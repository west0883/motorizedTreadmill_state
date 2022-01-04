#pragma once

#include <stddef.h>
#include <stdint.h>

#include "motor.h"

class MouseRunner
{
    public:
        enum class BehaviorType
        {
            Rest,
            Movement
        };
        
        struct StageParameters
        {
            // What type of stage this is
            BehaviorType type;
            
            // How long we'll run the mouse for
            uint32_t duration;

            // How fast the motor will run this interval
            float speed;
        };

    private:
        enum class State
        {
            // We're waiting for the initial starting signal
            Waiting,

            // We're running the mouse
            Running,

            // We're done running the mouse
            Done,
        };
        
        enum State state = State::Waiting;

        // Our stages
        const struct StageParameters *stageParameters;
        int stageTotal;

        // When we started our last stage
        uint32_t startTime;

        // The stage we're currently handling
        int currentStage = 0;

        // Our motor
        Motor &motor;

    private:
        /**
         * \brief Gets the time since a start time
         *
         * \param startTime
         *      The start time
         *
         * \return uint32_t
         *      The time since
         */
        static inline uint32_t TimeElapsed(uint32_t startTime)
        {
            return (uint32_t)millis() - startTime;
        }

        void StartNextStage(void);
        
    public:
        MouseRunner(const struct StageParameters *stageParameters, int stageTotal, Motor &motor);

        void Start(void);
        void Stop(void);
        
        void RunOnce(void);
};
