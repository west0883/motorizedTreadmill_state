#include "mouse_runner.h"

/**
 * \brief Creates a mouse runner
 *
 * \param *stageParameters
 *      The array of stages we'll run
 * \param stageTotal
 *      The number of stages in the array
 * \param &motor
 *      The motor to use
 *
 * \return none
 */
MouseRunner::MouseRunner(const struct StageParameters *stageParameters, int stageTotal, Motor &motor):
    stageParameters(stageParameters),
    stageTotal(stageTotal),
    motor(motor)
{
    
}

/**
 * \brief Starts the mouse runner
 *
 * \param none
 *
 * \return none
 */
void MouseRunner::Start(void)
{
    // If we aren't waiting to start, ignore this
    if (this->state != State::Waiting)
    {
        return;
    }
    
    // Begin movement
    this->StartNextStage();
}

/**
 * \brief Stops the mouse runner
 *
 * \param none
 *
 * \return none
 */
void MouseRunner::Stop(void)
{
    // Stop our motor immediately
    this->motor.Stop();
    
    // Note we're done
    this->state = State::Done;
}

/**
 * \brief Starts running the mouse
 *
 * \param none
 *
 * \return none
 */
void MouseRunner::StartNextStage(void)
{
    switch (this->stageParameters[this->currentStage].type)
    {
        // We're moving the motor during this stage
        case MouseRunner::BehaviorType::Movement:
        {
            // Start our motor
            this->motor.Start(this->stageParameters[this->currentStage].speed);
            break;
        }
        
        // We're not moving during this stage
        case MouseRunner::BehaviorType::Rest:
        {
            // Stop our motor
            this->motor.Stop();
            break;
        }
    }

    // Note when we started running
    this->startTime = millis();

    // Note we're running
    this->state = State::Running;
}

/**
 * \brief Handles the mouse runner
 *
 * \param none
 *
 * \return none
 */
void MouseRunner::RunOnce(void)
{
    switch (this->state)
    {
        case State::Waiting:
        {
           if (digitalRead(A0) == HIGH)
           {
                Serial.println("Starting Mouse Runner");

                this->Start();
            }
            
            break;
        }

        case State::Running:
        {
            // If our current interval needs to run longer, let it
            if (MouseRunner::TimeElapsed(this->startTime) < this->stageParameters[this->currentStage].duration)
            {
                break;
            }

            // Note we're starting another stage
            this->currentStage++;

            // If we're out of stages, done!
            if (this->currentStage >= this->stageTotal)
            {
                this->Stop();
                break;
            }
            
            // Start our next stage
            this->StartNextStage();
            
            break;
        }
        
        case State::Done:
        {
            // Nothing to do anymore
            break;
        }
    }
}
