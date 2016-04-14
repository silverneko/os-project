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

void setPriority(int pid, int priority){
	schud_param param;
	sched_getparam(pid, &param);
	param.sched_priority = priority;
	sched_setparam(pid, &param);
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
	int jobNow = -1;
	int jobOrder[n];
	int jobState[n];
	for(int i=0;i<n;i++){
		jobOrder[i] = 0;
		jobState[i] = STATE_WAITING;	
		runningTime[i] = 0;
	}

	while(finishJob<n){
		//if any job finishes
		if(jobNow==-1){
			
		}
		int nextJob = -1;
		int nextTime = 1000000;

		for(int i=0;i<n;i++){
			switch(jobState[i]){
				case STATE_WAITING:
					//job has not been forked
					if(timeNow>=job[i].readyTime){
						//fork
						int pid = fork();
						if(pid!=0){
							job[i].pid = pid;
							jobState[i] = STATE_READY;
							setPriority(pid, 99);
							if(job[i].executionTime<nextTime){
								nextJob = i;
								nextTime = job[i].executionTIme;
							}
						}
						else{
							for(int i=0;i<job[i].executionTime;i++){
								waitTimeQuantum;
							}
							//TODO: print message to kernel
							return 0;
						}
					}
					break;
				case STATE_READY:
					//job created, but put to wait
					if(job[i].executionTime<nextTime){
						nextJob = i;
						nextTime = job[i].executionTIme;
					}
					break;
				case STATE_RUNNING:
					//job started
					if(runningTime[i]>=job[i].executionTime){
						jobNow = -1;
						jobState[i] = STATE_FINISH;
						jobFinish++;
					}
					else{
						runningTime[i]++;
					}
					break;
					/*
					//job has been paused
					//no need for non preemptive SJF.
				case STATE_PAUSE:
					break;
					*/
				case STATE_FINISH:
					//job finished
					break;
				default:
					//fprintf(stderr, "WTF\n");
			}
			//if no job is running and there are jobs ready
			if(jobNow==-1&&nextJob!=-1){
				jobNow = nextJob;
				jobState[nextJob] = STATE_RUNNING;
				setPriority(job[nextJob].pid, 0);
			}
		}

		//next quantum
		waitTimeQuantum;
		timeNow++;
	}
	
	
	


	
	
	
	
}
