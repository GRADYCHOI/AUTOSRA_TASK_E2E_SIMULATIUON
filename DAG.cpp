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

void DAG::AddRunnablePtr(const std::shared_ptr<RUNNABLE>& runnable) {
    this->runnables.push_back(runnable);
}

void DAG::GenerateRunnables(int num, int in, int out) {
    int status = -1;
    for (int i = 1; i <= num; i++) {
        shared_ptr<RUNNABLE> runnable(new RUNNABLE(i, (double)(rand() % 100) / 1000));
        AddRunnablePtr(runnable);
        //if (i <= in) runnable->SetStatus(0);
        //else if (i > num - out) runnable->SetStatus(1);
        //else runnable->SetStatus(2);    
    }
    /*
    for (int j = 1; j <= num; j++) {
        for (int k = j+1; k <= num; k++) {
            if (j <= in) {
                if ((rand() % 100) < 40) {
                    //j runnable link -> k runnable
                }
            }
            else {
                if ((rand() % 100) < 30) {
                    //j runnable link -> k runnable
                }
            }
        }
    }*/

    /*for (const auto &runnable : runnables) {
        if (runnable->GetStatus() == 0) {
            runnable->LinkOutputRunnable(runnables[(runnable->GetId() + in)]);
        }
        else if (runnable->GetStatus() == 2) {
            runnable->LinkOutputRunnable(runnables[(runnable->GetId() + in)]);
        }
        //else {
       
        //}
        
    }*/
}
void DAG::RandomEdge() { //Runnable edge random generation

}

void DAG::DisplayRunnablesPtr(){
    cout << runnables.size() << " " << runnables.capacity() << endl;
    for (const auto &runnable : runnables) {
        cout << runnable->GetExecutionTime()  << " " << runnable->GetId() << endl;
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