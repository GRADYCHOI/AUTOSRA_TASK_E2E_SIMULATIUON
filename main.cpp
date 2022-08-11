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
    int num;
    num = rand() % 10 + 20;
    cout << "Number of Runnable : " << num << endl;
    DAG dag;
    dag.GenerateRunnables(num);

    return 0;
}
