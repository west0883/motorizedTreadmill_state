#pragma once

enum class BehaviorType
{
  Rest,
  Movement1,
  Movement2,        
};

struct StageBehaviors
{
  BehaviorType behaviorType;
};

// Initialize a structure that will hold the output of randomizeTime
struct time_outputs {
  uint32_t cumulativeTime;
  int count;
};


static void RandomizeBehaviors(void);
uint32_t RandomDuration(uint32_t MinTime, uint32_t MaxTime);
int RandomSpeeds(int* Speeds[]);
static void randomizeAccel(int count);
uint32_t GetTotalTime(int count);
void Report(float targetSpeed, int activityTag, String message);
void HeaderReport(int count, uint32_t totalTime); 
int getMinSpeed(void);
