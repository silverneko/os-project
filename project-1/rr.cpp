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
#include <queue>

using namespace std;

static int spawnProcess(const Job& job);
static void putToSleep(int pid);
static void wakeUp(int pid);

void rrSchedule(int N, vector<Job> jobs) {
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

  queue<int> readyQueue;
  int T = 0;
  int upTime = 0;
  int running = 0;
  auto it = jobs.begin();
  int jobsLeft = N;
  while (jobsLeft > 0) {
    while (it != jobs.end()) {
      if (it->readyTime <= T) {
        // spawn a process and put it to the end of the ready queue
        int pid = spawnProcess(*it);
        putToSleep(pid);
        readyQueue.push(pid);
        ++it;
      } else {
        break;
      }
    }
    if (running) {
      int pid = running;
      if (waitpid(pid, NULL, WNOHANG) == 0) {
        // pid is still "alive"
        if (upTime >= 500) {
          // time expired
          putToSleep(pid);
          readyQueue.push(pid);
          running = 0;
        }
      } else {
        // pid has exited
        running = 0;
        --jobsLeft;
      }
    }
    if (!running) {
      // Nobody is using the cpu
      // wake a process up
      if (!readyQueue.empty()) {
        int pid = readyQueue.front();
        readyQueue.pop();
        running = pid;
        upTime = 0;
        wakeUp(pid);
      }
    }
    waitTimeQuantum;
    ++T;
    ++upTime;
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

void putToSleep(int pid) {
  struct sched_param param;
  param.sched_priority = 0;
  if (sched_setscheduler(pid, SCHED_IDLE, &param) != 0) {
    assert(0 && "sched_setscheduler() failed.");
  }
}

void wakeUp(int pid) {
  struct sched_param param;
  param.sched_priority = 99;
  if (sched_setscheduler(pid, SCHED_FIFO, &param) != 0) {
    assert(0 && "sched_setscheduler() failed.");
  }
}
