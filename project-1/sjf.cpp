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

class CmpExe{
public: 
	bool operator()(Job *a, Job *b){
		return a->executionTime > b->executionTime;
	}
};

class CmpReady{
public:
	bool operator()(Job *a, Job *b){
		return a->readyTime > b->readyTime;
	}

};

void setCpu(int pid, int cpuId){
	cpu_set_t cpuMask;
    CPU_ZERO(&cpuMask);
    CPU_SET(cpuId, &cpuMask);
    sched_setaffinity(pid, sizeof(cpu_set_t), &cpuMask);
}

void idle(int pid){
	sched_param param;
	sched_getparam(pid, &param);
	param.sched_priority = 0;
	sched_setscheduler(pid, SCHED_IDLE, &param);
}

void wakeup(int pid){
	sched_param param;
	sched_getparam(pid, &param);
	param.sched_priority = 1;
	sched_setscheduler(pid, SCHED_FIFO, &param);
}

void setPriority(int pid, int priority){
	sched_param param;
	sched_getparam(pid, &param);
	param.sched_priority = priority;
	sched_setscheduler(pid, SCHED_FIFO, &param);
}

void sjfSchedule(int n, vector<Job> job){
	//shortest job first:
	//every time a job ends, execute the next shortest job.
	//never preempt: no need to set schuduler

	//implementation
	//uses 2 queues: readyQueue and createQueue
	//they are sorted according to the readyTime and executionTime, respectively
	//in the beginning, all process are in the createQueue
	//when the readyTime has come, a process is put to the readyQueue
	//when no process is running, the first process in the readyQueue will start to execute

	int timeNow = 0;
	int jobFinish = 0;
	Job *jobNow = NULL;
	priority_queue<Job*, vector<Job*>, CmpReady> createQueue;
	priority_queue<Job*, vector<Job*>, CmpExe> readyQueue;
	//put all processes into createQueue
	for(int i=0;i<n;i++){
		createQueue.push(&job[i]);
	}
	wakeup(0);
	setCpu(getpid(), 0);
	//schudule all jobs
	while(jobFinish<n){
	
		while(!createQueue.empty()){
			Job *nextJob = createQueue.top();
			//spawn processes, set priority to lowest to idle them
			if(nextJob->readyTime<=timeNow){
				createQueue.pop();
				readyQueue.push(nextJob);
				double beginTime = getTime();
				int workTime = nextJob->executionTime;
				int pid = fork();
				if(pid!=0){
					nextJob->pid = pid;
					//printf("spawn %s\n", nextJob->name.c_str());
					setCpu(pid, 1);
					idle(pid);
				}
				else{
					printf("%s %d\n", nextJob->name.c_str() , getpid());
					for(int t=0;t<workTime;t++){
						//printf("%s %d\n", nextJob->name.c_str() , t);
						waitTimeQuantum;
					}
					//TODO: print message to kernel
					double endTime = getTime();
    				char buffer[512];
    				sprintf(buffer, "[Project1] %d %.9f %.9f", getpid(), beginTime, endTime);
    				logger(buffer, strlen(buffer));
					exit(0);
				}
			}
			else{
				break;
			}
		}//end while createQueue

		//process readyQueue
		if(jobNow==NULL){
			if(!readyQueue.empty()){
				jobNow = readyQueue.top();
				readyQueue.pop();
				//printf("wakeup %s will execute %d\n", jobNow->name.c_str(), jobNow->executionTime);
				wakeup(jobNow->pid);
			}
		}

		if(jobNow!=NULL){
			if(jobNow->runningTime>=jobNow->executionTime){
				waitpid(jobNow->pid, NULL, 0);
				jobNow = NULL;
				jobFinish++;
			}
			else{
				jobNow->runningTime++;
			}
		}


		//next quantum
		//printf("main %d\n", timeNow);
		waitTimeQuantum;
		timeNow++;
	}
}
	

