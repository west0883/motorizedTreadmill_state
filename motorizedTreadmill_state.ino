// Sarah West
// 3/9/21
// code edited from https://github.com/troscianko/optomotor/blob/master/Arduino_Code/Optomotor_Controller_Simple_IR.ino 
// be sure you have the AccelStepper library installed

#include <stdint.h>

#include <AccelStepper.h>
#include "randomizing_functions.h"
#include "tone.h"
#include "warningTone.h"

#include "motor.h"
#include "mouse_runner.h"


uint32_t StartTime;
uint32_t CurrentTime;

// write out what speeds you want to include; ***YOU CAN EDIT THIS***
static constexpr int Speeds1[] = {800};
static constexpr int Speeds2[] = {}; 

// Time range for rest and movement (in seconds)
static constexpr in RestTime[] = {5, 30};
static constexpr in MoveTime[] = {5, 15};

// write out the parameter matrix; ***YOU CAN EDIT THIS*** 
static struct MouseRunner::StageParameters stageParameters[] = {
    {MouseRunner::BehaviorType::Rest,     },
    {MouseRunner::BehaviorType::Movement1 },
    {MouseRunner::BehaviorType::Rest      },
    {MouseRunner::BehaviorType::Movement1 },
    {MouseRunner::BehaviorType::Rest      },
    {MouseRunner::BehaviorType::Movement1 },
    {MouseRunner::BehaviorType::Rest      },
    {MouseRunner::BehaviorType::Movement1 },
    {MouseRunner::BehaviorType::Rest      },
    {MouseRunner::BehaviorType::Movement1 },
    {MouseRunner::BehaviorType::Rest      }
};

// Make a flag for if an input trigger (from Spike2) should be used.
static const bool useTrigger = false; 

// Make a flag for if maintaining tones should be used; 
static const bool useMaintaining = false; 

// Make a flag for if probe trials should be used.
static const bool useProbeTrials = false;

// Probability of those probe trials, if used (a fraction of 1, 1 = 100% of the time); 
static const double probability = 0.20; 

// Number of posssible entries in stageParameters.
static const int possible_stages = 25;

// Initialize the time stage parameters array with a lot of possible entries
struct MouseRunner::StageParameters stageParameters[possible_stages];

// DON'T EDIT:

// Make a tag that motor and warningTone will use to make understanding what's happening in the serial monitor easier to understand later.
// Explained further in comments at bottom.
int activityTag;

// Function that finds the size of an array.
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

// Predefine your times (do it here, and with "static" so they're accessible throughout program).
static uint32_t globalStartTime; 
static uint32_t CurrentTime;

// Declare your objects.
static Motor motor;
static WarningTone warningTone(useMaintaining);
static MouseRunner mouseRunner(stageParameters, motor, warningTone);

void setup(void)
{
  pinMode(A0, INPUT);  // for the trigger
  Serial.begin(115200);

  // Create a random seed for random sequence generator.
  randomSeed(analogRead(A1) * analogRead(A3) * analogRead(A5) * analogRead(A8) * analogRead(A10));

  // Run the starting random functions and displays.
  mouseRunner.StartNewTrial();
}

void loop(void)
{
  mouseRunner.RunOnce();
  motor.RunOnce();
}
