#pragma once

// Initialize a structure that will hold the output of randomizeTime
struct time_outputs {
  uint32_t cumulativeTime;
  int count;
};


static void randomizeAccel(time_outputs randomTime);
void Report(float targetSpeed, int activityTag, String message);
void HeaderReport(int count); 
int getMinSpeed(void);
