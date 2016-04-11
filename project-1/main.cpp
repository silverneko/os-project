#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <functional>
#include <vector>

#include <unistd.h>

#define timeQuantum { volatile unsigned long i; for(i=0;i<1000000UL;i++);}

enum {
  FIFO,
  RR,
  SJF,
  PSJF
};

class Job {
public:
  string name;
  int readyTime;
  int executionTime;
  Job() {}
  Job(const string &n, int r, int e) : name(n), readyTime(r), executionTime(e) {}
  bool operator < (const Job& j) const {
    return readyTime < j.readyTime;
  }
};

int main(int argc, char * argv[]) {
  char S[10];
  scanf("%s", S);
  int scheType = 0;
  if (strncmp("FIFO", S, 10)) {
    scheType = FIFO;
  } else if (strncmp("RR", S, 10)) {
    scheType = RR;
  } else if (strncmp("SJF", S, 10)) {
    scheType = SJF;
  } else if (strncmp("PSJF", S, 10)) {
    scheType = PSJF;
  } else {
    assert(0 && "Match pattern exhausted. Should never reach here.");
  }
  int N;
  scanf("%d", &N);
  vector<Job> joblist;
  for (int i = 0; i < N; ++i) {
    char buffer[40];
    int r, t;
    scanf("%s %d %d", buffer, &r, &t);
    joblist.emplace_back(buffer, r, t);
  }
  sort(joblist.begin(), joblist.end());
  return 0;
}
