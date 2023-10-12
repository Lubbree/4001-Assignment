#include <stdio.h>
#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include "header.h"

using namespace std;

void printJob(int time, job_t job, State old, State current){ //prints the important info about a job to console
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
    //setting up the cpu
    int clock = 0; //ticks up every time the loop is completed
    vector<job_t> loaded; //new is a protected term using loaded instead
    deque<job_t> ready; //the ready queue
    runningJob_t running; //running is a varible as there can only be one running process at a time
    running.job.pid = -1; //pid = -1 means that there is no process running
    vector<waitingJob_t> waiting; //vectore to hold the waiting processes
    
    /** a internal test
    struct job_t j1;
    j1.pid = 1;
    j1.arrivalTime = 0;
    j1.totalCpuTime = 7;
    j1.ioFrequency = 2;
    j1.ioDuration = 3;

    struct job_t j2;
    j2.pid = 2;
    j2.arrivalTime = 2;
    j2.totalCpuTime = 4;
    j2.ioFrequency = 3;
    j2.ioDuration = 1;

    struct job_t j3;
    j3.pid = 3;
    j3.arrivalTime = 4;
    j3.totalCpuTime = 2;
    j3.ioFrequency = 1;
    j3.ioDuration = 3;

    loaded.push_back(j1);
    loaded.push_back(j2);
    loaded.push_back(j3);
    */

    while(!loaded.empty() && !ready.empty() && running.job.pid != -1 && !waiting.empty()){ //exit when done

        //from new into ready
        if (!loaded.empty()){
            vector<job_t>::iterator iter = loaded.begin();

            for(iter; iter < loaded.end();){
                if(iter->arrivalTime <= clock){ 
                    //add to ready
                    ready.push_back(*iter);
                    printJob(clock, *iter, New, Ready);
                    //remove from loaded(new)
                    loaded.erase(iter);
                }else{
                    iter++;
                }
            }
        }

        //move from waiting to ready
        if (!waiting.empty()){
            vector<waitingJob_t>::iterator iter = waiting.begin();

            for(iter; iter < waiting.end();){
                iter->time += 1;

                if(iter->job.ioDuration <= iter->time){
                    //add to ready
                    ready.push_back(iter->job);
                    printJob(clock, iter->job, Waiting, Ready);
                    //remove from waiting
                    waiting.erase(iter);
                }else{
                    iter++;
                }
            }
        }
        
        //handle running
        if (running.job.pid >= 0) {
                running.job.totalCpuTime--;
                running.time++;
            
            if(running.job.totalCpuTime <= 0){ 
                printJob(clock, running.job, Running, Terminated);
                //terminate job
                running.job.pid = -1;
                running.time = 0;
            } else if(running.job.ioFrequency <= running.time){
                //add job to waiting
                waitingJob_t temp;
                temp.job = running.job;
                waiting.push_back(temp);
                printJob(clock, running.job, Running, Waiting);
                //remove job from
                running.job.pid = -1;
                running.time = 0;
            }
        }

        //put an avaible process into running
        if (running.job.pid < 0 && !ready.empty()){
            //add job to running
            running.job = ready.front();
            printJob(clock, running.job, Ready, Running);
            //remove job from ready
            ready.pop_front();
        }
        

        clock++;
    }

    return 0;
}