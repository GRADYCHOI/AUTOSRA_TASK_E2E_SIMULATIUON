#include "DAG.hpp"
#include<iostream>

using namespace std;

int lcm(int a,int b) {
    int i,j;
    int temp; //공배수를 저장할 변수
    j = (a>b)?a:b; //둘중 큰수를 구함
    for(i=j;;i++) {
        if(i%a==0 && i%b==0){
            temp = i;
            break;
        }
    }
    return temp;
}

DAG::DAG() {}

DAG::~DAG() {}

void DAG::SetParameters() {
    this->SetNumberOfTasks();
    this->SetNumberOfRunnables();
    this->SetHyperPeriod();
}

void DAG::SetHyperPeriod() {
    double tmpPeriod = 0.0;

    for(int count = 0; count < this->numOfTasks; count++) {
        if (tmpPeriod < this->task[count]->GetPeriod()) {
            this->hyperPeriod = lcm(this->hyperPeriod, this->task[count]->GetPeriod());
            tmpPeriod = this->task[count]->GetPeriod();
        }
    }
}

void DAG::SetNumberOfTasks() {
    this->numOfTasks = this->task.size();
}

void DAG::SetNumberOfRunnables() {
    this->numOfRunnables = 0;

    for (int count = 0; count < this->numOfTasks; count++) {
        this->numOfRunnables += this->task[count]->GetNumberOfRunnables();
    }
}

void DAG::GenerateRunnables(int num, int in, int out) {
    for (int i = 0; i <= num; i++) {
        shared_ptr<RUNNABLE> runnable(new RUNNABLE(i, (double)(rand() % 100) / 1000));
        this->runnables.push_back(runnable);
    }
}

void DAG::RandomEdge(int num, int in) { //Runnable edge random generation
    for (int j = 0; j < num; j++) {
        for (int k = j+1; k <= num; k++) {
            if ((rand() % 100) < 30) {
                //cout << j << " -> " << k << endl;
                //this->runnables[j]->LinkInputRunnable(this->runnables[k]->GetSharedPtr());
                //this->runnables[j]->LinkOutputRunnable(this->runnables[k]->GetSharedPtr());
                this->runnables[j]->AddOutputRun(this->runnables[k]->GetSharedPtr());
            }
        }
    }
}

void DAG::DisplayRunnablesPtr(){
    cout << runnables.size() << " " << runnables.capacity() << endl;
    for (const auto &runnable : runnables) {
        cout << runnable->GetExecutionTime()  << " " << runnable->GetId() << " ,  ";
        runnable->DisplayRunnable();
        cout << endl;
    }
}

int DAG::GetNumberOfTasks() {
    return this->numOfTasks;
}

int DAG::GetNumberOfRunnables() {
    return this->numOfRunnables;
}

double DAG::GetHyperPeriod() {
    return this->hyperPeriod;
}

double DAG::GetReactionTime() {
    return this->reactionTime;
}

double DAG::GetDataAge() {
    return this->dataAge;
}
/*
void DAG::AddTask(Task& task) {
    this->task.push_back(task);

    this->SetParamters();
}*/

void DAG::Simulate() {

}