// Sarah West
// 3/9/21
// code edited from https://github.com/troscianko/optomotor/blob/master/Arduino_Code/Optomotor_Controller_Simple_IR.ino 
// be sure you have the AccelStepper library installed

#include <stdint.h>

#include <AccelStepper.h>

#include "motor.h"
#include "mouse_runner.h"

uint32_t StartTime;
uint32_t CurrentTime;

// write out what speeds you want to include; ***YOU CAN EDIT THIS***
static constexpr int allSpeeds[] = {800, 1200, 1600};

// write out the parameter matrix; ***YOU CAN EDIT THIS*** 
static struct MouseRunner::StageParameters stageParameters[] = {
    // rest or move?, time (in ms)
    {MouseRunner::BehaviorType::Rest,       2000},
    {MouseRunner::BehaviorType::Movement,   30000},
    {MouseRunner::BehaviorType::Rest,       30000},
    {MouseRunner::BehaviorType::Movement,   30000},
    {MouseRunner::BehaviorType::Rest,       30000},
    {MouseRunner::BehaviorType::Movement,   30000},
    {MouseRunner::BehaviorType::Rest,       30000},
    {MouseRunner::BehaviorType::Movement,   30000},
    {MouseRunner::BehaviorType::Rest,       30000},
    {MouseRunner::BehaviorType::Movement,   30000},
    {MouseRunner::BehaviorType::Rest,       30000}
};

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static Motor motor;
static MouseRunner mouseRunner(stageParameters, ARRAY_SIZE(stageParameters), motor);

//         M0   M1    M2          RESOLUTION
//------------------------------------------------
//        LOW   LOW   LOW        FULL  STEP   1 Revolution  200  steps
//        HIGH  LOW   LOW        HALF  STEP   1 Revolution  400  steps
//        LOW   HIGH  LOW        1/4   STEP   1 Rrevolution  800  steps
//        HIGH  HIGH  LOW        1/8   STEP   1 Rrevolution  1600 steps
//        LOW   LOW   HIGH       1/16  STEP   1 Revolution  3200 steps
//        HIGH  LOW   HIGH       1/32  STEP   1 Revolution  6400 steps

static void randomize(void)
{
    // Randomize the speed order
    //
    // Creates a random seed for random sequence generator.
    randomSeed(analogRead(1));
    
    for (size_t i = 0; i < ARRAY_SIZE(stageParameters); i++)
    {
        switch (stageParameters[i].type)
        {
            case MouseRunner::BehaviorType::Rest:
            {
                break;
            }

            case MouseRunner::BehaviorType::Movement:
            {
                // Pick a random index within the speed array
                size_t j = random(0, ARRAY_SIZE(allSpeeds));
    
                stageParameters[i].speed = allSpeeds[j];
                break;
            }
        }
    }
}

void setup(void)
{
    Serial.begin(115200);

    randomize();
  
    // report speed order
    for (size_t i = 0; i < ARRAY_SIZE(stageParameters); i++)
    {
        Serial.print(stageParameters[i].speed);
        Serial.print(", ");
    }

    pinMode(A0, INPUT);  // for the trigger

    Serial.println("Waiting for Trigger"); 
}

void loop(void)
{
    mouseRunner.RunOnce();
    motor.RunOnce();
}
