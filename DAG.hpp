#include<iostream>
#include "TASK.hpp"
using namespace std;

class DAG //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    int num_task;
    int num_runnable;
    double hyper_period;

public:
    DAG();

    double reaction_time;
    double data_age;
    

    vector<int> task;

    void set_hyper_period();
    void get_hyper_period();
    void get_num_task();
    void get_num_runnable();

    ~DAG();

};