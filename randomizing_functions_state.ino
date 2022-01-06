#include "randomizing_functions_state.h"
#include <limits>

// Helper functions run at setup. 

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

void HeaderReport(int count)
{
  // Report trial number
  if (useTrialNumber) 
  {
    Serial.print("Trial ");
    Serial.println(trial_number); 
  }
  
  // Report total number of stages
  Serial.print("Total number of stages: ");
  Serial.println(count);

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
