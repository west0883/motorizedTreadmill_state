#include "randomizing_functions_state.h"
#include <limits>

// Helper functions run at setup. 

// Uses stageBehaviors to create times and speeds for stageParameters
void RandomizeBehaviors(void)
{
  for (size_t i = 0; i <= ARRAY_SIZE(stageBehaviors) ; i++)
  {
    BehaviorType behavior = stageBehaviors[i].behaviorType;

    switch (behavior)
    {
       case BehaviorType::Rest:
       { 
         // Speed
         stageParameters[i].speed = 0; 

         // Duration
         uint32_t stage_duration = RandomDuration(RestTime[0], RestTime[1]);
         
         stageParameters[i].duration = stage_duration; 
                  
         break;
       }
       
       case BehaviorType::Movement1:
       {
         // Speed
         int number_of_speeds = ARRAY_SIZE(Speeds1);
         int j = RandomSpeeds(number_of_speeds);
         stageParameters[i].speed = Speeds1[j];
            
         // Duration
         uint32_t stage_duration = RandomDuration(MoveTime[0], MoveTime[1]);
         
         stageParameters[i].duration = stage_duration; 
        
         break;
       }

       case BehaviorType::Movement2:
       {
         // Speed
         int number_of_speeds = ARRAY_SIZE(Speeds2);
         int j = RandomSpeeds(number_of_speeds);
         stageParameters[i].speed = Speeds2[j];
         
         // Duration
         uint32_t stage_duration = RandomDuration(MoveTime[0], MoveTime[1]);
         
         stageParameters[i].duration = stage_duration; 
         
         break;
       }
    }
  }
 // Calculate the speed difference (for warning tone and easier use of probe trials).
   for (size_t i = 0; i < ARRAY_SIZE(stageBehaviors) ; i++)
  {
      // Place in parameter array
      stageParameters[i].speed_difference = stageParameters[i + 1].speed - stageParameters[i].speed;
  }
}

uint32_t RandomDuration(uint32_t MinTime, uint32_t MaxTime)
{
  // Find a random time within the range
  uint32_t stage_duration = random(MinTime, MaxTime + 1) * (1000);
  
  return stage_duration;
}

int RandomSpeeds(int number_of_speeds)
{
  size_t j = random(0, number_of_speeds);
  return j;
}

// Creates a function that randomizes the speed order. Make dependent on speedDiff value to determine what kind of change it is.
static void randomizeAccel(int count)
{
    // Only do this if user has said they want to randomize the accelerations.
    if (useAccels)
    {
        // Find the minimum speed 
        int min_speed = getMinSpeed(); 
        
        // Don't include index "0" because that is the initial rest period. 
        for (size_t i = 1; i <= count ; i++)
        {
           // Use previous stage's speed difference (accel set at start of new stage, when motor transition begins)
    
           // If less than the minimum speed, then assume it's a speed change
           if (abs(stageParameters[i - 1].speed_difference) < min_speed)
           {
            // Pick a random index within the speed array
            size_t j = random(0, ARRAY_SIZE(accelsSpeedChange));
        
            // Place in parameter array
            stageParameters[i].accel = accelsSpeedChange[j];
           }
    
           // Otherwise, is assumed to be a start or stop.
           else
           {
             // Pick a random index within the speed array
            size_t j = random(0, ARRAY_SIZE(accelsStartStop));
        
            // Place in parameter array
            stageParameters[i].accel = accelsStartStop[j];
           }
        }
    }
}

uint32_t GetTotalTime(int count)
{
  uint32_t totalTime = 0;

  for (size_t i = 1; i <= count ; i++)
  {
    totalTime += stageParameters[i].duration;
  }
  return totalTime;
}

// A function that writes out what goes to the serial monitor.
void Report(float targetSpeed, float accel, int activityTag, String message)
{
      
    CurrentTime=millis()-globalStartTime; 

    if (useTrialNumber)
    {
      Serial.print(trial_number);
      Serial.print(", ");
    }
    
    Serial.print(String(CurrentTime)); 
    Serial.print(", "); 
    Serial.print(String(targetSpeed));
    Serial.print(", "); 
    
    if (useAccels){
      Serial.print(String(accel));
      Serial.print(", "); 
    }
    
    Serial.print(activityTag); 
    Serial.print(", "); 
    Serial.println(message); 
}

void HeaderReport(int count, uint32_t totalTime)
{
  // Report trial number
  if (useTrialNumber) 
  {
    Serial.print("Trial ");
    Serial.println(trial_number); 
  }
  
  // Report total number of stages
  Serial.print("Total number of stages: ");
  Serial.println(count + 1);

  // Report total time of trial
  Serial.print("Total time: ");
  Serial.println(totalTime);

  // report speeds and times HeaderReport(int randomTime.count); 
  Serial.print("Time, Speed");

  if (useAccels)
  {
     Serial.print(", Acceleration");
  }
  
  if (useProbeTrials){
    Serial.print(", Probe type"); 
  }

  Serial.println();
 
  for (size_t i = 0; i <= count ; i++)
  {  
    Serial.print(stageParameters[i].duration);
    Serial.print(", ");
    
    Serial.print(stageParameters[i].speed);
    

    if (useAccels)
    {
      Serial.print(", ");
      Serial.print(stageParameters[i].accel);
    }

    if (useProbeTrials)
    {

      if (stageParameters[i].probe == Probe::NoWarning)
      {
        Serial.print(", "); 
        Serial.print("Probe: no warning cue");
      }
      
      else if (stageParameters[i].probe == Probe::NoChange)
      {
        Serial.print(", "); 
        Serial.print("Probe: no change in speed");
      }
      
      else
      {
        Serial.print(", "); 
        Serial.print("No probe");
      }
    }
    Serial.println();
  }  
}

int getMinSpeed(void)
{
    // A trick to find the maximum possible value for an int, so everything afterwards is always less than that
    int min_speed = std::numeric_limits<int>::max();

    // Go through first possible list of speeds
    for (std::size_t i = 0; i < ARRAY_SIZE(Speeds1); i++)
    {
        if ((Speeds1[i] > 0) && (Speeds1[i] < min_speed))
        {
            min_speed = Speeds1[i];
        }
    }
    
    // Go through second list of speeds.
    for (std::size_t i = 0; i < ARRAY_SIZE(Speeds2); i++)
    {
        if ((Speeds2[i] > 0) && (Speeds2[i] < min_speed))
        {
            min_speed = Speeds2[i];
        }
    }
    
    // If our array was broken, make sure we return a reasonable default
    if (min_speed == std::numeric_limits<int>::max())
    {
        min_speed = 2000;
    }
    
    return min_speed;
}
