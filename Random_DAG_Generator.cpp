#include<iostream>
#include<stdlib.h>
#include<random>
#include<time.h>
#include<cstdlib>
using namespace std;

int Decide_Input_run(int INPUT[], int n);
int Decide_Output_run(int INPUT[], int n, int input);

class DAG //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    /* data*/    
public:
    int num;
    int runnable[];
    DAG();

    void Input() {
        cout << "Runnable number : " << num << endl;
        for (int i = 0; i <= num; i++) runnable[i] = 0;
        int Input_num = Decide_Input_run(runnable, num);
        int Output_num = Decide_Output_run(runnable, num, Input_num);
        for (int i = 0; i < num; i++) {
            printf("%d ", runnable[i]);
        }
    }
    //int edge[num][num-1] = {0, };
    //void GetEdge();
    ~DAG();
};
DAG::DAG() {
    cout << "Please Enter the number of Runnable Node(50 ~ 500) : ";
    cin >> num;
    int runnable[num] = {0};
}

DAG::~DAG() {
    cout << endl << "end" << endl;
}
class Runnable //동작시간, 엣지
{
private:
    int ID;

public:
    //int execution_time = random 1~30; 
    //int edge_run[];

};

int Decide_Input_run(int INPUT[], int n) {
    int Input_count = rand() % 4 + 2;
    for (int i = 0; i <= Input_count; i++) INPUT[i] = 1;
    return Input_count;
}

int Decide_Output_run(int INPUT[], int n, int input) {
    int Output_count = rand() % 4 + 2;
    for (int i = n; i >= n - Output_count; i--) INPUT[i] = 2;
    return Output_count;
}
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
    srand(time(NULL));
    DAG DAG1;
    DAG1.Input();
    //DAG_Generator();

    return 0;
}