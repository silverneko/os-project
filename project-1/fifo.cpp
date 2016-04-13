#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <functional>

#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

using namespace std;

static int spawnProcess(const Job& job);

void fifoSchedule(int N, vector<Job> jobs) {
  // first sort by readyTime and executionTime
  sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
    if (a.readyTime == b.readyTime) {
      return a.executionTime < b.executionTime;
    } else {
      return a.readyTime < b.readyTime;
    }
  });

  cpu_set_t cpuMask;
  CPU_ZERO(&cpuMask);
  CPU_SET(0, &cpuMask);
  if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &cpuMask) != 0) {
    assert(0 && "sched_setaffinity() failed.");
  }
  int T = 0;
  for (int i = 0; i < N; ++i) {
    Job &job = jobs[i];
    while (job.readyTime > T) {
      waitTimeQuantum;
      ++T;
    }
    // put process into the FIFO queue as soon as it's ready
    struct sched_param param;
    param.sched_priority = 99-i;
    job.pid = spawnProcess(job);
    if (sched_setscheduler(job.pid, SCHED_FIFO, &param) != 0) {
      assert(0 && "sched_setscheduler() failed.");
    }
  }
  // zombie waiter
  for (int i = 0; i < N; ++i) {
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
    double startTime = getTime();
    printf("%s %d\n", job.name.c_str(), getpid());
    cpu_set_t cpuMask;
    CPU_ZERO(&cpuMask);
    CPU_SET(1, &cpuMask);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &cpuMask) != 0) {
      assert(0 && "sched_setaffinity() failed.");
    }
    int execTime = job.executionTime;
    while (execTime > 0) {
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
