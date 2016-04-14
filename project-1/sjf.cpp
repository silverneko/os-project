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


//SJF by Sean

#define waitTimeQuantum { volatile unsigned long i; for(i=0;i<1000000UL;i++);}

//prototype
/*
class Job {
public:
  std::string name;
  int readyTime;
  int executionTime;
  int pid;
  Job() {}
  Job(const std::string &n, int r, int e) : name(n), readyTime(r), executionTime(e) {}
};
*/

enum STATE{
	STATE_WAITING,
	STATE_READY,
	STATE_RUNNING,
	STATE_PAUSE,
	STATE_FINISH
}

void sjfSchedule(int n, vector<Job> job){
	//shortest job first:
	//every time a job ends, execute the next shortest job.
	//never preempt: no need to set schuduler

	//ver1.0(now)):
	//use realtime scheduling
	//calculate the priority when running

	//TODO:
	//ver2.0:
	//optimize: calculate everything before started
	//use a buffer to store the time for every command


	int finishJob = 0;
	int timeNow = 0;
	int runningTime[n];
	int jobOrder[n];
	int jobState[n];
	for(int i=0;i<n;i++){
		jobOrder[i] = 0;
		jobState[i] = STATE_WAITING;	
		runningTime[i] = 0;
	}
	while(finishJob<n){
		for(int i=0;i<n;i++){
			switch(jobState[i]){
				case STATE_WAITING:
					//job has not been forked
					if(timeNow==job[i].readyTime){
						//fork

						jobState[i] = STATE_READY;
					}
					break;
				case STATE_READY:
					//job created, but put to wait
					break;
				case STATE_RUNNING:
					//job started
					break;
				/*
					//job has been paused
					no need for non preemptive SJF.
				case STATE_PAUSE:
					break;
				*/
				case STATE_FINISH:
					//job finished
					break;
				default:
				//fprintf(stderr, "WTF\n");
			}
		}


		//next time
		waitTimeQuantum;
		time++;
	}
	
	
	


	
	
	
	
}
