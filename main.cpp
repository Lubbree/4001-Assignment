#include <stdio.h>
#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include "header.h"

using namespace std;

void printJob(int time, job_t job, State old, State current){ //prints the important info about a job to console
    printf("Time: %d | pid: %d | Old State: %s | New State: %s\n", time, job.pid, printState(old), printState(current));
}

int main(int argc, const char** argv)
{
    //setting up the cpu
    int clock = 0; //ticks up every time the loop is completed
    vector<job_t> loaded; //new is a protected term using loaded instead
    deque<job_t> ready; //the ready queue
    runningJob_t running; //running is a varible as there can only be one running process at a time
    running.job.pid = -1; //pid = -1 means that there is no process running
    vector<waitingJob_t> waiting; //vectore to hold the waiting processes

    FILE* Data_in = fopen(argv[1], "r"); //takes in cvs file
    if (Data_in == NULL) {
        printf("No File/Data");
        return 1;
    }
    
    char line[255];

    while (fgets(line, sizeof(line), Data_in) != NULL) { //parse line by line into type job_t
        job_t temp;
        if(sscanf(line, "%d,%d,%d,%d,%d",
            &temp.pid,
            &temp.arrivalTime,
            &temp.totalCpuTime,
            &temp.ioFrequency,
            &temp.ioDuration)==5){
            loaded.push_back(temp);
        }
    }
    
    while(!loaded.empty() || !ready.empty() || running.job.pid != -1 || !waiting.empty()){ //exit when done

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