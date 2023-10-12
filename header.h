#include <string>

struct job_t
{
    int pid;  //assumption -1 == no job
    int arrivalTime;
    int totalCpuTime;
    int ioFrequency;
    int ioDuration;
};

struct runningJob_t
{
    job_t job;
    int time = 0;
};

struct waitingJob_t
{
    job_t job;
    int time = 0;
};

enum State { New, Ready, Running, Waiting, Terminated };

std::string printState(State s){
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
