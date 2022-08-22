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
void DAG::CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    int runnableId = runnable->GetId();
    this->runnablePrecedence[runnableId] = (this->runnablePrecedence[runnableId] > precedence) ? this->runnablePrecedence[runnableId] : precedence;

    if (runnable->GetStatus() != 1) { // Output Runnable
        for (auto &outputRunnable : runnable->getoutput()) {
            this->CheckPrecedence(outputRunnable, ++precedence);
        }
    }
}

void DAG::SetRunnablePrecedence() {
    // runnablePrecedence initialize
    std::vector<int> tmpRunnablePrecedence(this->GetNumberOfRunnables(), -1);
    tmpRunnablePrecedence.swap(this->runnablePrecedence);

    int numberOfInputRunnables = this->GetNumberOfInputRunnables();

    for (int inputRunnableIndex = 0; inputRunnableIndex < numberOfInputRunnables; inputRunnableIndex++) {
        this->CheckPrecedence(this->runnables[this->inputRunnables[inputRunnableIndex]], 0);
    }
}
/*
int CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    precedence++;
    if (runnable->GetInputRunnable() == 0) {
         runnable->SetPrecedence(precedence);
         cout << "input runnable complete" << endl;
         return 0;
    }
    if (runnable->GetOutputRunnable() == 0) { //이미 precedence가 셋팅되어있으면 안하는식으로 수정
        runnable->SetPrecedence(precedence);            
        cout << runnable->GetId() <<" output runnable complete" << endl;
    }
    else {
        for (auto &OutputRunnable : runnable->getoutput()) {
            CheckPrecedence(OutputRunnable, precedence);

        }
     
    }
    runnable->SetPrecedence(precedence);
}

void DAG::SetRunnablePrecedence() {
    
    for (auto &runnable : runnables) {
        int precedence = 0;
        if runnable->Get
        CheckPrecedence(runnable, precedence);
        cout << precedence << endl;
    }
}
*/

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
    return this->runnables.size();
}

void DAG::SetInputRunnableList() {
    // vector capacity control : swap technique
    std::vector<int> tmpList;

    for (auto &runnable : this->runnables) {
        if (runnable->GetStatus() == 0) {
            tmpList.push_back(runnable->GetId());
        }
    }

    tmpList.swap(this->inputRunnables);
}

void DAG::SetOutputRunnableList() {
    std::vector<int> tmpList;

    for (auto &runnable : this->runnables) {
        if (runnable->GetStatus() == 1) {
            tmpList.push_back(runnable->GetId());
        }
    }

    tmpList.swap(this->outputRunnables);
}

int DAG::GetNumberOfInputRunnables() {
    return (int)this->inputRunnables.size();
}

int DAG::GetNumberOfOutputRunnables() {
    return (int)this->outputRunnables.size();
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