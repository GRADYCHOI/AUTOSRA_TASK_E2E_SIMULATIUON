#include<iostream>
#include<stdlib.h>
#include<random>
#include<time.h>
#include<cstdlib>
#include<ctime>
#include<vector>
//#include"DAG.hpp"
#include"RUNNABLE.hpp"
using namespace std;

int main() {
    srand(time(NULL));
    //int num = 0;
    int num;
    num = rand() % 50 + 50;
    cout << "Number of Runnable : " << num << endl;
    //cin >> num;
    
    RUNNABLE runnable[num];
    srand(time(NULL));
    for (int i = 0; i <= num; i++) {
        runnable[i].ID = i;
        //double exe = 0.1;
        double exe = (double)(rand() % 100) / 100;
        runnable[i].get_execution_time(exe);
    }
    

    return 0;
}
