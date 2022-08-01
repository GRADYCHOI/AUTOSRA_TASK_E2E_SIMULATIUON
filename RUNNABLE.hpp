#include<iostream>
#include<stdlib.h>
#include<vector>
using namespace std;

class RUNNABLE 
{ 
private:
    double execution_time;
    int precedence = -1;
    int status = -1; // 0 = input runnable, 1 = middle runnable, 2 = output runnable
    vector<int> input_runnable;
    vector<int> output_runnable;

public:
    int ID;
    RUNNABLE();
    RUNNABLE(double time, int status);

    void get_status();
    void set_precedence(int number);
    void get_execution_time(double time);

    ~RUNNABLE();
};
