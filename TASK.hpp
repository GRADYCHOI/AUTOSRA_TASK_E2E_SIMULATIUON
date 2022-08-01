#include<iostream>
#include "RUNNABLE.hpp"

using namespace std;
class TASK //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    double period;
    double offset;
    double execution_time;
    int num_runnable;
    int priority;

public:
    TASK();
    
    vector<int> runnable;

    int get_num_runnable();
    void set_execution_time();

    ~TASK();

};