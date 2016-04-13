#ifndef __OS_PROJECT_1_HEADER__
#define __OS_PROJECT_1_HEADER__

#include <string>
#include <vector>

#include <sys/time.h>
#include <time.h>

#define waitTimeQuantum { volatile unsigned long i; for(i=0;i<1000000UL;i++);}

enum {
  FIFO,
  RR,
  SJF,
  PSJF
};

class Job {
public:
  std::string name;
  int readyTime;
  int executionTime;
  int pid;
  Job() {}
  Job(const std::string &n, int r, int e) : name(n), readyTime(r), executionTime(e) {}
};

namespace {
double getTime() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void logger(const char buffer[], int len) {
  // TODO output with printk
  printf("%s\n", buffer);
}

}

void fifoSchedule(int N, std::vector<Job> jobs);

void rrSchedule(int N, std::vector<Job> jobs);

#endif
