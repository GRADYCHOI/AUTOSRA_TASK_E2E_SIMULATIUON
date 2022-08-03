#include<iostream>
#include<stdlib.h>
#include<random>
#include<time.h>
#include<cstdlib>
#include<ctime>
#include<vector>
#include"DAG.hpp"
using namespace std;

int main() {
    srand(time(NULL));
    //int num = 0;
    int num, input, output;
    num = rand() % 20 + 40;
    cout << "Number of Runnable : " << num << endl;
    cout << "Input Runnable num : ";
    cin >> input;
    cout << "Output Runnable num : ";
    cin >> output;
    DAG dag;
    dag.DisplayRunnablesPtr();
    dag.GenerateRunnables(num, input, output);
    //cin >> num;
    /* 
    RUNNABLE runnable[num];
    srand(time(NULL));
    for (int i = 0; i <= num; i++) {
        //runnable[i].id = i;
        double exe = (double)(rand() % 100) / 100;
    }*/
    

    return 0;
}
