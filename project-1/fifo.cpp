#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <functional>

#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int spawnProcess(const Job& job);

void fifoSchedule(int N, vector<Job> jobs) {
  sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
    if (a.readyTime == b.readyTime) {
      return a.executionTime < b.executionTime;
    } else {
      return a.readyTime < b.readyTime;
    }
  });

  int T = 0;
  for (int i = 0; i < N; ++i) {
    Job &job = jobs[i];
    while (job.readyTime > T) {
      waitTimeQuantum;
      ++T;
    }
    job.pid = spawnProcess(job);
    wait(NULL);
  }
}

int spawnProcess(const Job& job) {
  int pid;
  if (pid = fork()) {
    // Parent
    assert(pid != -1 && "fork() failed.");
    return pid;
  } else {
    // Child
    printf("%s %d\n", job.name.c_str(), getpid());
    int execTime = job.executionTime;
    double startTime = getTime();
    while(execTime > 0) {
      waitTimeQuantum;
      --execTime;
    }
    double endTime = getTime();
    char buffer[500];
    int len = snprintf(buffer, 500, "[Project1] %d %.9f %.9f", getpid(), startTime, endTime);
    logger(buffer, len);
    exit(0);
  }
  // Should never reach here
  exit(0);
}
