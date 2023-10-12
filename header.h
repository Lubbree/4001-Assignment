#include <string>

struct job_t //used to hold the information of a job (process)
{
    int pid;  //assumption -1 == no job
    int arrivalTime;
    int totalCpuTime;
    int ioFrequency;
    int ioDuration;
};

struct runningJob_t //holds the job that is running and a timer
{
    job_t job; //job that is running
    int time = 0; //how long it has been running
};

struct waitingJob_t //holds a job that is waiting and a timer
{
    job_t job; //job that is waiting
    int time = 0; //how long it has been waiting
};

enum State { New, Ready, Running, Waiting, Terminated }; //the differnet states a job can be in

std::string printState(State s){ //turns a state enum into a string
    switch (s)
    {
    case New:
        return "New";
    case Ready:
        return "Ready";
    case Running:
        return "Running";
    case Waiting:
        return "Waiting";
    case Terminated:
        return "Terminated";
    default:
       return "!State Not Found!";
    }
}
