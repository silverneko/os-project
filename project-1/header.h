#ifndef __OS_PROJECT_1_HEADER__
#define __OS_PROJECT_1_HEADER__

#include <string>
#include <vector>
#include <time.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#define __NR_mysyscall 327

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
#ifndef DEMO
  printf("%s\n", buffer);
#else
  syscall(__NR_mysyscall, buffer, len);
#endif
}

}

void fifoSchedule(int N, std::vector<Job> jobs);
void psjfSchedule(const std::vector<Job>& jobs);
void rrSchedule(int N, std::vector<Job> jobs);

#endif
