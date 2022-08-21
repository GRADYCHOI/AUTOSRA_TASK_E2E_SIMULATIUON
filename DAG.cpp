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

void DAG::GenerateRunnables(int num) {
    for (int i = 0; i < num; i++) {
        shared_ptr<RUNNABLE> runnable(new RUNNABLE(i, (double)(rand() % 100) / 1000));
        this->runnables.push_back(runnable);
        cout << "Runnable ID : " << runnable->GetId() << ", Execution Time : " << runnable->GetExecutionTime() << endl;
    }
    RandomEdge();
    SetRunnablePrecedence();
    DisplayRunnablesPtr();
}

void DAG::RandomEdge() { 
    int rate;
    cout << "Enter Edge rate(20~40) : ";
    cin >> rate;
    for (int j = 0; j < (int)this->runnables.size(); j++) {
        for (int k = j+1; k < (int)this->runnables.size(); k++) {
            if ((rand() % 100) < rate) {
                //cout << j << " -> " << k << endl;
                this->runnables[j]->LinkOutputRunnable(this->runnables[k]->GetSharedPtr());
            }
        }
    }
}

int CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    precedence++;
    if (runnable->GetStatus() == 0) {
         runnable->SetPrecedence(precedence);
         cout << "input runnable complete" << endl;
         return 0;
    }
    else if (runnable->GetStatus() == 1) {
        if (runnable->GetPrecedence() < precedence) {
            runnable->SetPrecedence(precedence);
            cout << "output runnable complete" << endl;
        } 
    }
    else {
        for (auto &OutputRunnable : runnable->getoutput()) {
            runnable->SetPrecedence(precedence);
            CheckPrecedence(OutputRunnable, precedence); // output runnable precedence가 지금  precedence보다 낮으면 실행하도록 바꾸라!
        }
    }
    cout  << runnable->GetId() << "last" << precedence << endl;
    //runnable->SetPrecedence(precedence);
    return 0;
}

void DAG::SetRunnablePrecedence() {
    
    for (auto &runnable : runnables) {
        int precedence = 0;
        CheckPrecedence(runnable, precedence);
    }
}

void DAG::DisplayRunnablesPtr(){

    cout << "Display Start" << endl;
    cout << runnables.size() << " " << runnables.capacity() << endl;
    for (const auto &runnable : runnables) {
        cout << "Runnable ID : " <<  runnable->GetId() << " ,  ";
        cout << "Execution Time : " << runnable->GetExecutionTime() << " , ";
        cout << "Precedence : " << runnable->GetPrecedence() << " , ";
        if (runnable->GetStatus() == 0) cout << "INPUT , ";
        else if (runnable->GetStatus() == 1) cout << "OUTPUT , ";
        else if (runnable->GetStatus() == 2) cout << "MIDDLE , ";
        cout << runnable->GetId() << " -> ";
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