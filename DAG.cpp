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
        if (tmpPeriod < this->tasks[count]->GetPeriod()) {
            this->hyperPeriod = lcm(this->hyperPeriod, this->tasks[count]->GetPeriod());
            tmpPeriod = this->tasks[count]->GetPeriod();
        }
    }
}

void DAG::SetNumberOfTasks() {
    this->numOfTasks = this->tasks.size();
}

void DAG::SetNumberOfRunnables() {
    this->numOfRunnables = 0;

    for (int count = 0; count < this->numOfTasks; count++) {
        this->numOfRunnables += this->tasks[count]->GetNumberOfRunnables();
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
    int tasksnum = 0;
    cout << "Enter Number of Tasks : ";
    cin >> tasksnum;
    GenerateTasks(tasksnum);
    SetTaskPriority();
    //DoRandomTaskMapping();
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

int DAG::CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    precedence++;
    if (runnable->GetPrecedence() < precedence) runnable->SetPrecedence(precedence);
    if (runnable->GetStatus() == 2) for (auto &outputrunnable : runnable->getoutput()) CheckPrecedence(outputrunnable, precedence);
    return 0;
}

void DAG::SetRunnablePrecedence() {
    for (auto &runnable : runnables) {
        if (runnable->GetStatus() == 0) {
            int precedence = 1;
            runnable->SetPrecedence(precedence);
            for (auto &outputrunnable : runnable->getoutput()) {
                //cout << runnable->GetId() << " Input runnable's output runnable : " << outputrunnable->GetId() << endl;
                CheckPrecedence(outputrunnable, precedence);
            }
        }
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
    return this->tasks.size();
}

int DAG::GetNumberOfRunnables() {
    return this->runnables.size();
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
void DAG::AddTaskPtr(const std::shared_ptr<TASK>& task) {
    this->tasks.push_back(task);
}

void DAG::GenerateTasks(int numberOfTasks) {
    double tmpPeriod = -1.0;
    double tmpOffset = -1.0;
    this->numOfTasks = numOfTasks;
    bool flag = true;

    while (flag) {
        for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
            std::cout << taskIndex << " -th Task Period : ";
            std::cin >> tmpPeriod;
            std::cout << taskIndex << " -th Task Offset : ";
            std::cin >> tmpOffset;

            shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod, tmpOffset));
            this->AddTaskPtr(task);

            std::cout << "Task ID : " << task->GetId() << ", Period : " << task->GetPeriod() << std::endl;
        }
        if (CheckMappable()) {
            cout << "This Mappable!" << endl;
            flag = false;
        }
        else cout << "Increse Tasks Period!" << endl;
    }
}
void DAG::SetTaskPriority() {
    std::vector<std::pair<int, double>> tmpTaskArray; // ID, Period

    for (auto &task : this->tasks) {
        tmpTaskArray.push_back(std::make_pair(task->GetId(), task->GetPeriod()));
    }

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), &this->CompareTaskPeriod);

    for (auto &tmpTask : tmpTaskArray) {
        taskPriority.push_back(tmpTask.first);
    }

    //for (auto &task : taskPriority) cout << task << endl;
}

void DAG::SetRunnablePriority(int index) {
    std::vector<int> tmpRunnablePriority = this->runnablePriorities[index];
    tmpRunnablePriority.swap(this->runnablePriority);
}

void DAG::SetRunnablePriorities() {
    int numberOfRunnables = this->GetNumberOfRunnables();
    std::vector<std::vector<int>> abstractedRunnablePriorities;
    abstractedRunnablePriorities.reserve(numberOfRunnables);

    // Set Abstracted Runnable Priority Table
    for (auto &task : this->tasks) { // TODO : Task Priority에 따라 정렬
        int tmpPrecedence = -1;
        std::vector<std::pair<int, int>> tmpRunnableArray; // ID, Precedence

        for (auto &runnable : task->GetRunnables()) {
            tmpRunnableArray.push_back(std::make_pair(runnable->GetId(), this->runnablePrecedence[runnable->GetId()]));
        }

        std::sort(tmpRunnableArray.begin(), tmpRunnableArray.end(), &this->CompareRunnablePrecedence);

        for (auto &tmpRunnable : tmpRunnableArray) {
            if (tmpPrecedence != tmpRunnable.second) {
                abstractedRunnablePriorities[abstractedRunnablePriorities.size()].push_back(tmpRunnable.first);
                tmpPrecedence = tmpRunnable.second;
            } else {
                abstractedRunnablePriorities[(abstractedRunnablePriorities.size() - 1)].push_back(tmpRunnable.first);
            }
        }
    }

    // Set Expanded Runnable Priority Table
    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        this->ExpandRunnablePriorities(abstractedRunnablePriorities, 0, numberOfRunnables);
    }
}

void DAG::ExpandRunnablePriorities(std::vector<std::vector<int>> incompleteRunnablePriority, int pointer, int maxSize) {
    if (pointer == maxSize) {
        std::vector<int> tmpList;

        for (int runnableIndex = 0; runnableIndex < maxSize; runnableIndex++) {
            tmpList.push_back(incompleteRunnablePriority[runnableIndex][0]);
        }

        this->runnablePriorities.push_back(tmpList);
    } else {
        int numberOfSamePriority = incompleteRunnablePriority[pointer].size();

        if (numberOfSamePriority > 1) {
            for (int samePriorityRunnableIndex = 0; samePriorityRunnableIndex < numberOfSamePriority; samePriorityRunnableIndex++) {
                std::vector<std::vector<int>> tmpRunnablePriority = incompleteRunnablePriority;
                std::vector<int> tmpRunnable;

                tmpRunnable.push_back(tmpRunnablePriority[pointer][samePriorityRunnableIndex]);
                tmpRunnablePriority[pointer].erase(tmpRunnablePriority[pointer].begin() + samePriorityRunnableIndex);
                tmpRunnablePriority.insert(tmpRunnablePriority.begin() + pointer, tmpRunnable);
                
                this->ExpandRunnablePriorities(tmpRunnablePriority, ++pointer, maxSize);
            }
        } else {
            this->ExpandRunnablePriorities(incompleteRunnablePriority, ++pointer, maxSize);
        }
    }
}

int DAG::GetNumberOfSequenceCase() {
    return (int)runnablePriorities.size();
}
/*
void DAG::SetTasksPriority() {
    for (int i = 0; i < this->GetNumberOfTasks(); i++){
        int min = 0;
        min = tasks[i]->GetId();
        cout << "1" << endl;;
        for (int j = 0; j < this->GetNumberOfTasks(); j++) {
            cout << "2" << endl;
            if (i == j) continue;
            if (tasks[j]->GetPeriod() < tasks[min]->GetPeriod()) min = tasks[j]->GetId();
        }
        this->tasks2.push_back(tasks[min]);
        tasks.erase(tasks.begin() + (min - 1));
        //taskPriority.push_back(tasks[i]);
        cout << "3" << endl;
    }

    for (auto &task : tasks2) cout << task->GetId();
}*/

void DAG::DoRandomTaskMapping() {
    /*
    if (this->CheckMappable()) {
        for (auto &runnable : this->runnables) {
            bool mappingFlag = false;
            while (!mappingFlag) {
                for (auto &task : this->tasks) {
                    if ((this->GetUtilization() + (runnable->GetExecutionTime() / task->GetPeriod())) > 0.6) continue;

                    if ((std::rand() % 100) < 20) {
                        task->AddRunnable(runnable);
                        mappingFlag = true;
                        break;
                    }
                }

                if (!mappingFlag) {
                    if (this->GetUtilization() > 0.6) {
                        this->ClearTaskMapping();
                        this->DoRandomTaskMapping();
                        break;
                    }
                }
            }
        }
    } else {
        std::cout << "This Dag Can't Mapped within Utilization" << std::endl;
    }
    while(this->CheckMappable()) {
        for (auto &runnable : runnables) {
            bool mappingFlag = false;
            while(!mappingFlag) {
                for (auto &task : tasks) {
                    if (task)
                }
            }
        }
    }*/
    for (auto &runnable : runnables) {
        this->tasks[taskPriority[0]]->AddRunnable(runnable);
    }

}

bool DAG::CheckMappable() {
    double sumOfExecutionTimes = 0.0;
    double maxPeriod = 0.0;

    for (auto &runnable : this->runnables) {
        sumOfExecutionTimes += runnable->GetExecutionTime();
    }

    for (auto &task : this->tasks) {
        if (maxPeriod < task->GetPeriod()) {
            maxPeriod = task->GetPeriod();
        }
    }

    return ((sumOfExecutionTimes / maxPeriod) < 0.5) ? true : false;
}

double DAG::GetUtilization() {
    double tmpUtilization = 0.0;

    for (auto &task : this->tasks) {
        tmpUtilization += (task->GetExecutionTime() / task->GetPeriod());
    }

    return tmpUtilization;
}


void DAG::Simulate() {

}