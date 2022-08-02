#include<iostream>
#include "RUNNABLE.hpp"

using namespace std;
class TASK 
{ 
private:
    double period;
    double offset;
    double execution_time;
    int numOfRunnables;
    int priority;

public:
    TASK();
    ~TASK();
    
    vector<int> runnable;

    int GetNumRunnable() {
        return this->numOfRunnables;
    };
    double GetPeriod() {
        return this->period;
    };
    void set_execution_time();


};