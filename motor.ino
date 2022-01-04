#include "motor.h"
/**
 * \brief Creates a motor
 *
 * \param none
 *
 * \return none
 */
Motor::Motor(void):
    stepperMotor(AccelStepper::DRIVER, 3, 2)
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(Motor::SleepPowerPin, OUTPUT);
    
    digitalWrite(Motor::SleepPowerPin, LOW);
    
    this->stepperMotor.setAcceleration(Motor::StepperAccell);
    this->stepperMotor.setCurrentPosition(0);
}

/**
 * \brief Starts the motor
 *
 * \param speed
 *      How fast to run the motor
 *
 * \return none
 */
void Motor::Start(float speed)
{
    this->targetSpeed = speed;
    
    digitalWrite(Motor::SleepPowerPin, HIGH);

    // If we'll be accelerating, just set the new max speed, as the motor driver
    // will manage accelerating
    if (this->targetSpeed >= this->stepperMotor.speed())
    {
        CurrentTime=millis()-StartTime; 
        Serial.print(String(CurrentTime)); 
        Serial.print(", "); 
        Serial.print(String(this->targetSpeed));
        Serial.print(", "); 
        Serial.println("Motor: accelerating"); 
        
        this->stepperMotor.setMaxSpeed(this->targetSpeed);

        this->stepperMotor.move(Motor::nSteps);

        this->state = State::Accelerating;

    }
    // Else, if we'll be decelerating, we'll need to coast the motor down from its
    // current speed manually using stop()
    else
    {
        CurrentTime=millis()-StartTime; 
        Serial.print(String(CurrentTime)); 
        Serial.print(", "); 
        Serial.print(String(this->targetSpeed));
        Serial.print(", "); 
        Serial.println("Motor: decelerating");
        this->stepperMotor.stop();

        this->state = State::Decelerating;
    }
}

/**
 * \brief Stops the motor
 *
 * \param none
 *
 * \return none
 */
void Motor::Stop(void)
{
    this->targetSpeed = 0;

    CurrentTime=millis()-StartTime; 
    Serial.print(String(CurrentTime)); 
    Serial.print(", "); 
    Serial.print(String(this->targetSpeed));
    Serial.print(", "); 
    Serial.println("Motor: stopping");

    // Stop our motor
    this->stepperMotor.stop();
  
    this->state = State::Stopping;
}

/**
 * \brief Handles controlling the motor
 *
 * \param none
 *
 * \return none
 */
void Motor::RunOnce(void)
{
    switch (this->state)
    {
        case State::Idle:
        {
            // Nothing else to do
            return;
        }
        
        case State::Stopping:
        {
            // When motor has reached final position
            if (this->stepperMotor.distanceToGo() == 0)
            {
                  CurrentTime=millis()-StartTime; 
                  Serial.print(String(CurrentTime)); 
                  Serial.print(", "); 
                  Serial.print(String(this->targetSpeed));
                  Serial.print(", "); 
                  Serial.println("Motor: finished stopping");
                
                // Turn off the motor's power
                digitalWrite(Motor::SleepPowerPin, LOW);
                
                // go to Idle state 
                this->state = State::Idle;
            }
            
            break;
        }

        case State::Accelerating:
        {
            // If the motor has reached the target speed
            if (this->stepperMotor.speed() >= this->targetSpeed)
            {
                CurrentTime=millis()-StartTime; 
                Serial.print(String(CurrentTime)); 
                Serial.print(", "); 
                Serial.print(String(this->targetSpeed));
                Serial.print(", "); 
                Serial.println("Motor: reached target speed");      
                           
                // the AccelStepper library doesn't need to reset the set speed after accelerating, like decelerating does

                this->state = State::Running;
            }

            break;
        }
        
        case State::Decelerating:
        {
            // If we've coasted enough and we're ready to start moving again, do so
            if (this->stepperMotor.speed() <= this->targetSpeed)
            {
                CurrentTime=millis()-StartTime; 
                Serial.print(String(CurrentTime)); 
                Serial.print(", "); 
                Serial.print(String(this->targetSpeed));
                Serial.print(", "); 
                Serial.println("Motor: reached target speed");      
                           
                this->stepperMotor.setMaxSpeed(this->targetSpeed);

                this->stepperMotor.move(Motor::nSteps);

                this->state = State::Running;
            }

            break;
        }

        case State::Running:
        {   
    
            break;
        }
    }
    
    // This holds in its 'memory' where the motor is supposed to go and checks if the
    // stepper is due for another step; runs for every iteration of the loop
    this->stepperMotor.run();
}
