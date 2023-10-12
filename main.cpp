#include <stdio.h>
#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include <format>
#include "header.h"

using namespace std;

void printJob(int time, job_t job, State old, State current){
    //return "time: " + time + " pid " + job.pid + " Old State: " + printState(old) + " New State " + printState(current) + " \n";
    //return format("Time: {} | pid: {} | Old State: {} | New State: {} \n", time, job.pid, printState(old), printState(current));
    //printf("Time: %d | pid: %d | Old State: %s | New State: %s \n", time, job.pid, printState(old), printState(current));
    //cout << "Time: " << time << " pid: " << job.pid << " Old State: " << printState(old) << " New State " << printState(current) << "\n";
    cout << "Time: ";
    cout << time;
    cout << " | pid: ";
    cout << job.pid;
    cout << " | Old State: ";
    cout << printState(old);
    cout << " | New State: ";
    cout << printState(current) << "\n";
}

int main()
{
    int clock = 0;

    vector<job_t> loaded; //new is a protected term hopefully you can just add stuff into the vector
    deque<job_t> ready;
    runningJob_t running;
    running.job.pid = -1;
    vector<waitingJob_t> waiting;
    
    struct job_t j1;
    j1.pid = 0;
    j1.arrivalTime = 1;

    loaded.push_back(j1);

    while(!loaded.empty() && !ready.empty() && running.job.pid != -1 && !waiting.empty() || clock < 5){ //exit when done

        //from new into ready
        if (!loaded.empty()){
            vector<job_t>::iterator iter = loaded.begin();

            for(iter; iter < loaded.end(); iter++){
                if(iter->arrivalTime <= clock){
                    ready.push_back(*iter);
                    loaded.erase(iter);
                    printJob(clock, *iter, New, Ready);
                }
            }
        }

        //move from waiting to ready
        if (!waiting.empty()){
            vector<waitingJob_t>::iterator iter = waiting.begin();

            for(iter; iter < waiting.end(); iter++){
                if(iter->job.ioDuration <= iter->time){
                    ready.push_back(iter->job);
                    waiting.erase(iter);
                    printJob(clock, iter->job, New, Ready);
                }
                iter->time += 1;
            }
        }
        
        //handle running
        if (running.job.pid >= 0) {
            if (running.job.ioFrequency <= running.time){ //io
                //add job to waiting
                waitingJob_t temp;
                temp.job = running.job;
                waiting.push_back(temp);
                printJob(clock, running.job, Running, Waiting);
                //remove job from
                running.job.pid = -1;
                running.time = 0;
            }else if (running.job.totalCpuTime <= 0){ //Terminate Job
                printJob(clock, running.job, Running, Terminated);
                running.job.pid = -1;
                running.time = 0;
            }else {
                running.job.totalCpuTime--;
                running.time++;
            }
        }

        //put an avaible process into running
        if (running.job.pid < 0 && !ready.empty()){
            running.job = ready.front();
            ready.pop_front();
        }
        

        clock++;
    }

    cout << loaded.empty();

    return 0;
}