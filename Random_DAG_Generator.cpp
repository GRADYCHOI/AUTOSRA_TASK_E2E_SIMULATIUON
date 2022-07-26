#include<iostream>
#include<stdlib.h>
using namespace std;

class DAG //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    /* data*/    
public:
    int num;
    int Input() {
        cout << "Please Enter the number of Runnable Node : ";
        cin >> num;
        int runnable[num] = {0};
        int edge[num][num-1] = {0, };
        int Input_run[num] = {0};
        int Output_run[num] = {0}; 
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num-1; j++) printf("%d ", runnable[i]);
            printf("\n");
        }
        return num;
    } 
    //void GetEdge();
};

class Runnable //동작시간, 엣지
{
private:
    int ID;

public:
    //int execution_time = random 1~30; 
    //int edge_run[];

};

/*
void Runnable_Generator() {
    int run_num = random 1~100;
    Runnable run[run_num];

}
*/

/*
void DAG_Generator(DAG DAG1) {
    Runnable_Generator();
    Random_DAG_Generate();
    return DAG1;
}
*/
/*
void Random_DAG_Generate(~~~,~~~,~~~) {

}
*/
int main() {
    DAG DAG1;
    DAG1.Input();
    //DAG_Generator();
    cout << DAG1.num << endl;
}