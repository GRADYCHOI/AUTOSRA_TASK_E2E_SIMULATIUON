#include<iostream>
#include<stdlib.h>
#include<random>
#include<time.h>
#include<cstdlib>
using namespace std;

int Decide_Input_run(int INPUT[], int n);
int Decide_Output_run(int INPUT[], int n);

class DAG //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    /* data*/    
public:
    DAG();
    int num = rand() % 50 + 100;
    int* runnable = new int[num]();
    int Input_num = Decide_Input_run(runnable, num);
    int Output_num = Decide_Output_run(runnable, num);
    //int display = display_arr(runnable, num);
    void display(void){
        for (int i = 0; i < num; i++) cout << runnable[i];
        cout << endl;
    }

    ~DAG(); 

};

DAG::DAG() {
    cout << "DAG START" << endl;
}

DAG::~DAG() {
    delete [] runnable;
    cout << "DAG END" << endl;
}

int Decide_Input_run(int INPUT[], int n) {
    int Input_count = rand() % 4 + 2;
    for (int i = 0; i <= Input_count; i++) INPUT[i] = 1;
    return Input_count;
}

int Decide_Output_run(int INPUT[], int n) {
    int Output_count = rand() % 4 + 2;
    for (int i = n; i >= n - Output_count; i--) INPUT[i] = 2;
    return Output_count;
}

int main() {
    srand(time(NULL));
    int ran = rand() % 10;
    DAG DAG1[ran];
    for (int i = 0; i < ran; i++) DAG1[i].display();

    return 0;
}