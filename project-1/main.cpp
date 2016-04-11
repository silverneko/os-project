#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <algorithm>
#include <functional>

using namespace std;

int main(int argc, char * argv[]) {
  char S[10];
  scanf("%s", S);
  int scheType = 0;
  if (strncmp("FIFO", S, 10) == 0) {
    scheType = FIFO;
  } else if (strncmp("RR", S, 10) == 0) {
    scheType = RR;
  } else if (strncmp("SJF", S, 10) == 0) {
    scheType = SJF;
  } else if (strncmp("PSJF", S, 10) == 0) {
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
  switch (scheType) {
    case FIFO:
      fifoSchedule(N, joblist);
      break;
    case RR:
      break;
    case SJF:
      break;
    case PSJF:
      break;
  }
  return 0;
}
