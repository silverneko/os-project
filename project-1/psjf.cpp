#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

namespace leo{
	char message[50];
	void error(const char *message){
		printf("%s", message);
		exit(0);
	}

	class Process{
	public:
		Process(const std::string &n, int r, int e){
		// FOR GENERAL-PROCESS
			name = n;
			pid = -1;
			readyTime = r;
			executionTime = e;
			remainTime = e;
			priority = -1;
			next = NULL;
		}
		Process(int remain, int prior){
		// FOR PSEUDO-PROCESS
			name = "";
			pid = -2;
			readyTime = -1;
			executionTime = -1;
			remainTime = remain;
			priority = prior;
			next = NULL;
		}
		bool operator<(const Process& another) const {
			return readyTime < another.readyTime;
		}

		string name;
		int pid;
		int readyTime;
		int executionTime;

		int remainTime;
		int priority;

		Process *next;
	};

	class CpuQueue{
	public:
		CpuQueue(){
			head = new Process(0, 99);
			tail = new Process(1000000000, 0);
			head->next = tail;
			capacity = 0;
			highestPriority = 0;
		}
		Process* top(){
		// RETURN THE POINTER TO THE HIGHEST PRIORITY PROCESS IN QUEUE
		// RETURN NULL IF THE QUEUE IS EMPTY
			if(capacity == 0)
				return NULL;
			return head->next;
		}
		int pop(){
		// RETURN -1 IF THE QUEUE IS EMPTY
		// RETURN POSITIVE INTEGER INDICATING THE NUMBER OF PROCESSES NOW IN QUEUE
			if(capacity == 0)
				return -1;
			
			head->next = head->next->next;
			capacity--;
			return capacity;
		}
		int insertAndPrioritize(Process *inserting){
		// LOCATING CORRECT POSITION IN LIST
			Process *higher = head;
			Process *lower = head->next;
			while(inserting->remainTime >= lower->remainTime){
				higher = higher->next;
				lower = lower->next;
			}
			
		// SETTING PRIORITY
			inserting->priority = (higher->priority + lower->priority)/2;
			
		// LINKING NEW PROCESS TO LIST
			inserting->next = lower;
			higher->next = inserting;
			capacity++;

			return inserting->priority;
		}
		int simulate(int time){
		// IN THIS TIME INTERVAL
		// RETURN -1 IF CPU KEEP IDLED
		// RETURN 0 IF ALL PROCESSES KEEP RUNNING
		// RETRUN POSITIVE INTEGER N IF N PROCESSES WILL TERMINATED
			if(capacity == 0)
				return -1;

			Process *now = top();
			int terminated = 0;
			while(now->remainTime <= time){
				time -= now->remainTime;
				terminated++;
				pop();
				
				now = top();
				if(now == NULL)
					break;
			}
			if(now != NULL)
				now->remainTime -= time;
			
			return terminated;
		}
		void print(){
			if(capacity == 0)
				cout << "EMPTY";
			else{
				Process *now = head->next;
				for(; now!=tail; now=now->next)
					cout << now->name << " ";
			}
		}

		Process *head;
		Process *tail;
		int capacity;
		int highestPriority;
	};

	void assignCPU(int pid, int cpu);
	void assignPriority(int pid, int priority);
	void newProcess(Process *process);
}
using namespace leo;





void psjfSchedule(const std::vector<Job>& jobs)
{
	assignCPU(0, 0);
	assignPriority(0, 99);
	setbuf(stdout, NULL);

	std::vector<Process> processes;
	for(int i=0; i<jobs.size(); i++)
		processes.emplace_back(jobs[i].name, jobs[i].readyTime, jobs[i].executionTime);

// SORT BY READY TIME
	std::sort(processes.begin(), processes.end());

	int T = 0, pivot = 0;
	CpuQueue queue;

// NEW PROCESS ARRIVAL DRIVEN LOOP
	while(true){

	// SUSPEND ALL CHILD PROCESSES WHEN SCHEDULING
		assignCPU(0, 1);
		
		for(; processes[pivot].readyTime == T; pivot++){
			Process *now = &processes[pivot];
			queue.insertAndPrioritize(now);
			newProcess(now);
			cout << now->name << " " << now->pid << endl;
		}
	// ALL CHILD PROCESSES ARRIVED AND FORKED
		if(pivot == processes.size())
			break;
		
		int nextArrival = processes[pivot].readyTime;
		int terminated = queue.simulate(nextArrival-T);

		assignCPU(0, 0);
	// SCHEDULING OVER, CHILD PROCESSES START RUNNING
		
	// ON CPU0 SIMULATE THE TIMING ON CPU1, SYNC WITH BLOCKING WAIT
		for(; T<nextArrival; T++){
			waitTimeQuantum;	
		}
		for(int i=0; i<terminated; i++)
			wait(NULL);
	}
	int terminated = queue.simulate(1000000000);

// NO MORE NEW PROCESS ARRIVAL
// FINISH THE REMAINING CHILD PROCESSES IN QUEUE	
	assignCPU(0, 0);
	for(int i=0; i<terminated; i++)
		wait(NULL);

	return;
}





namespace leo{
	void assignCPU(int pid, int cpu){
		cpu_set_t cpuMask;
	  	CPU_ZERO(&cpuMask);
	  	CPU_SET(cpu, &cpuMask);
		if( sched_setaffinity(pid, sizeof(cpu_set_t), &cpuMask)!=0 ){
			sprintf(message, "[%d] sched_setaffinity failed\n", getpid());
			error(message);
		}
	}
	void assignPriority(int pid, int priority){
		struct sched_param param;
		param.sched_priority = priority;
		if( sched_setscheduler(pid, SCHED_FIFO, &param)!=0 ){
			sprintf(message, "[%d] sched_setscheduler failed\n", getpid());
			error(message);
		}
	}

	void newProcess(Process *process){
		if( process->pid = fork() ){
			if(process->pid == -1)
				error("fork failed\n");
			return;
		}
		else{
			assignPriority(0, process->priority);

			double startTime = getTime();
			int time = process->remainTime;
			for(int i=0; i<time; i++){
				waitTimeQuantum;
			}
			double endTime = getTime();
			
			char buffer[500];
    		int len = snprintf(buffer, 500, "[Project1] %d %.9f %.9f", getpid(), startTime, endTime);
    		logger(buffer, len);
			
			exit(0);
		}
	}
}
