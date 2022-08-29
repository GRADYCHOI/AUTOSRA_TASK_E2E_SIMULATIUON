#include "DAG.hpp"


using namespace std;

double LCM(double a, double b) {
    int tmp1 = static_cast<int>(a * 1000);
    int tmp2 = static_cast<int>(b * 1000);

    while (tmp2 != 0) {
    	int tmp3 = tmp1 % tmp2;
        tmp1 = tmp2;
        tmp2 = tmp3;
    }

    return static_cast<double>(a * (b / (double)(tmp1 / 1000)));
}

DAG::~DAG() {
    tasks.clear();
    runnables.clear();
}

std::vector<int> DAG::GetRunnablePriority() {
    std::vector<std::pair<int, int>> tmpVector;
    std::vector<int> result;
    int index = 0;

    for (auto &runnableId : this->runnablePriority) {
        tmpVector.push_back(std::make_pair(index, runnableId));
        index++;
    }

    std::sort(tmpVector.begin(), tmpVector.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

    for (auto &priorityMap : tmpVector) {
        result.push_back(priorityMap.first);
    }

    return result;
}

std::vector<int> DAG::GetRunnableSequence(int index) {
    return this->runnablePriorities[index];
}

std::vector<int> DAG::GetTaskPriority() {
    std::vector<std::pair<int, int>> tmpVector;
    std::vector<int> result;
    int index = 0;

    for (auto &taskId : this->taskPriority) {
        tmpVector.push_back(std::make_pair(index, taskId));
        index++;
    }

    std::sort(tmpVector.begin(), tmpVector.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

    for (auto &priorityMap : tmpVector) {
        result.push_back(priorityMap.first);
    }

    return result;
}

std::vector<std::shared_ptr<TASK>> DAG::GetOrderOfPriorityTasks() {
    std::vector<std::shared_ptr<TASK>> orderOfPriorityTasks;

    for (auto &taskId : taskPriority) {
        orderOfPriorityTasks.push_back(this->tasks[taskId]);
    }

    return orderOfPriorityTasks;
}

std::vector<std::shared_ptr<RUNNABLE>> DAG::GetOrderOfPriorityRunnables() {
    std::vector<std::shared_ptr<RUNNABLE>> orderOfPriorityRunnables;

    for (auto &runnableId : runnablePriority) {
        orderOfPriorityRunnables.push_back(this->runnables[runnableId]);
    }

    return orderOfPriorityRunnables;
}

int DAG::GetMaxCycle() {
    int minPeriod = INT_MAX;
    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        if (minPeriod > this->tasks[taskIndex]->GetPeriod()) {
            minPeriod = this->tasks[taskIndex]->GetPeriod();
        } 
    }

    return static_cast<int>(this->GetHyperPeriod() / minPeriod);
}

double DAG::GetHyperPeriod() {
    double tmpPeriod = this->GetOrderOfPriorityTasks()[0]->GetPeriod();

    for(auto& orderOfPriorityTasks : this->GetOrderOfPriorityTasks()) {
        tmpPeriod = LCM(tmpPeriod, orderOfPriorityTasks->GetPeriod());
    }

    return tmpPeriod;
}

void DAG::SetInputRunnableList() {
    // inputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->inputRunnables);

    //std::copy_if(this->runnables.begin(), this->runnables.end(), std::back_inserter(this->inputRunnables), [](std::shared_ptr<RUNNABLE> runnable) { return runnable->GetId() == 0; });
    for (auto &run : runnables) if (run->GetStatus() == 0) inputRunnables.push_back(run);
}

void DAG::SetOutputRunnableList() {
    // outputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->outputRunnables);

    //std::copy_if(this->runnables.begin(), this->runnables.end(), std::back_inserter(this->outputRunnables), [](std::shared_ptr<RUNNABLE> runnable) { return runnable->GetId() == 1; });
    for (auto &run : runnables) if (run->GetStatus() == 1) outputRunnables.push_back(run);
}

void DAG::GenerateRunnables(int numberOfRunnables) {
    std::cout << "Generate Runnables Start" << std::endl;

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<RUNNABLE> runnable(new RUNNABLE(runnableIndex, runnableIndex, (double)(std::rand() % 100) / 1000));
        this->runnables.push_back(runnable);
        //std::cout << "Runnable ID : " << runnable->GetId() << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;
    }

    this->RandomEdge();
    this->SetInputRunnableList();
    this->SetOutputRunnableList();
    for (auto &run : inputRunnables) std::cout << run->GetId() << " " << run->GetStatus() << std::endl;
}

void DAG::RandomEdge() { //Runnable edge random generation
    int rate = -1;

    std::cout << "Enter Edge rate(20~40) : ";
    std::cin >> rate; 

    for (auto &runnable : this->runnables) {
        for (auto &outputRunnable : this->runnables) {
            if ((rand() % 100) < rate && runnable->GetId() < outputRunnable->GetId()) {
                runnable->LinkOutputRunnable(outputRunnable->GetSharedPtr());
            }
        }
    }
}

void DAG::GenerateTasks(int numberOfTasks) {
    double tmpPeriod = -1.0;
    double tmpOffset = -1.0;
    bool flag = true;

    while(flag) {
        for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
            std::cout << taskIndex << " -th Task Period : ";
            std::cin >> tmpPeriod;
            std::cout << taskIndex << " -th Task Offset : ";
            std::cin >> tmpOffset;
            std::shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod, tmpOffset));
            this->tasks.push_back(task);
            std::cout << "Task ID : " << tasks[taskIndex]->GetId() << ", Period : " << tasks[taskIndex]->GetPeriod() << std::endl;
        }

        if (CheckMappable()) {
            std::cout << "This Mappable!" << std::endl;
            flag = false;
        }
        else {
            std::cout << "Increse Tasks Period!" << std::endl;
            this->ClearTaskMapping();
            std::vector<std::shared_ptr<TASK>>().swap(this->tasks);
        }
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

void DAG::ClearTaskMapping() {
    for (auto &task : this->tasks) {
        task->ClearMapping();
    }
}

double DAG::GetUtilization() {
    double tmpUtilization = 0.0;

    for (auto &task : this->tasks) {
        tmpUtilization += (task->GetExecutionTime() / task->GetPeriod());
    }

    return tmpUtilization;
}

// TODO : 인 형 확인
int DAG::CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    precedence++;
    //int runnableId = runnable->GetId();
    //this->runnablePrecedence[runnableId] = (this->runnablePrecedence[runnableId] > precedence) ? this->runnablePrecedence[runnableId] : precedence;
    if (runnablePrecedence[runnable->GetId()] < precedence) runnablePrecedence[runnable->GetId()] = precedence;
    //std::cout << runnable->GetId() << " " << runnable->GetStatus() << std::endl;
    if (runnable->GetStatus() == 2) {
        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            if (runnablePrecedence[outputRunnable->GetId()] < precedence+1) CheckPrecedence(outputRunnable, precedence);
        }
    }

    return 0;
}

void DAG::SetRunnablePrecedence() {
    // runnablePrecedence initialize
    std::vector<int> tmpRunnablePrecedence(this->GetNumberOfRunnables(), 0);
    tmpRunnablePrecedence.swap(this->runnablePrecedence);

    for (auto &inputRunnable : this->inputRunnables) {
        int precedence = 1;
        this->runnablePrecedence[inputRunnable->GetId()] = precedence;
        for (auto &outputRunnable : inputRunnable->GetOutputRunnables()) {
            CheckPrecedence(outputRunnable, 0);
        }
            
    }
    for (auto &run : runnables)std::clog << run->GetId() << " " << run->GetStatus() << " " << runnablePrecedence[run->GetId()] << std::endl;
}

void DAG::SetTaskPriority() {
    std::vector<std::pair<int, double>> tmpTaskArray; // ID, Period

    for (auto &task : this->tasks) {
        tmpTaskArray.push_back(std::make_pair(task->GetId(), task->GetPeriod()));
    }

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), [](std::pair<int, double> a, std::pair<int, double> b) { return a.second < b.second; });

    for (auto &tmpTask : tmpTaskArray) {
        this->taskPriority.push_back(tmpTask.first);
    }
}

void DAG::SetRunnablePriority(int index) {
    std::vector<int> tmpRunnablePriority = this->runnablePriorities[index];
    tmpRunnablePriority.swap(this->runnablePriority);

    this->currentSequenceIndex = index;
}

void DAG::SetRunnablePriorities() {
    std::vector<std::vector<int>> abstractedRunnablePriorities;

    // Set Abstracted Runnable Priority Table
    for (auto &task : GetOrderOfPriorityTasks()) { // TODO : Task Priority에 따라 정렬
        int tmpPrecedence = -1;
        std::vector<std::pair<int, int>> tmpRunnableArray; // ID, Precedence

        for (auto &runnable : task->GetRunnables()) {
            tmpRunnableArray.push_back(std::make_pair(runnable->GetId(), this->runnablePrecedence[runnable->GetId()]));
        }

        std::sort(tmpRunnableArray.begin(), tmpRunnableArray.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

        for (auto &tmpRunnable : tmpRunnableArray) {
            if (tmpPrecedence != tmpRunnable.second) {
                std::vector<int> tmpVector = {tmpRunnable.first};
                abstractedRunnablePriorities.push_back(tmpVector);
                tmpPrecedence = tmpRunnable.second;
            } else {
                abstractedRunnablePriorities.back().push_back(tmpRunnable.first);
            }
        }
    }

    // Set Expanded Runnable Priority Table
    this->ExpandRunnablePriorities(abstractedRunnablePriorities, 0, this->GetNumberOfRunnables());
}

void DAG::ExpandRunnablePriorities(std::vector<std::vector<int>>& incompleteRunnablePriority, int iterator, int maxSize) {
    if (iterator == maxSize) {
        std::vector<int> tmpList;

        for (auto &tmpRunnablePriority : incompleteRunnablePriority) {
            tmpList.push_back(tmpRunnablePriority[0]);
        }
        this->runnablePriorities.push_back(tmpList);
    } else {
        if (incompleteRunnablePriority[iterator].size() > 1) {
            std::vector<int> tmpRunnable;

            tmpRunnable.push_back(incompleteRunnablePriority[iterator][0]);
            incompleteRunnablePriority[iterator].erase(std::find(incompleteRunnablePriority[iterator].begin(), incompleteRunnablePriority[iterator].end(), incompleteRunnablePriority[iterator][0]));
            incompleteRunnablePriority.insert(incompleteRunnablePriority.begin() + iterator, tmpRunnable);
            
            this->ExpandRunnablePriorities(incompleteRunnablePriority, (iterator + 1), maxSize);
        } else {
            this->ExpandRunnablePriorities(incompleteRunnablePriority, (iterator + 1), maxSize);
        }
    }
}

void DAG::DisplayRunnables() {
    std::cout << "[Debugging] Display Start" << std::endl;
    std::cout << " - Runnable Vector Size : " << std::cout.width(5) << this->runnables.size() << std::endl;
    std::cout << " - Runnable Vector capacity : " << std::cout.width(5) << this->runnables.capacity() << std::endl;

    for (const auto &runnable : this->runnables) {
        std::cout << "[" <<  runnable->GetId() << "th Runnable]" << std::endl;
        std::cout << " - Execution Time : " << std::cout.width(5) << runnable->GetExecutionTime() << " , ";
        std::cout << "Precedence : " << std::cout.width(5) << this->runnablePrecedence[runnable->GetId()] << " , ";
        if (runnable->GetStatus() == 0) std::cout << "Status : INPUT  , ";
        else if (runnable->GetStatus() == 1) std::cout << "Status : OUTPUT , ";
        else if (runnable->GetStatus() == 2) std::cout << "Status : MIDDLE , ";
        std::cout << "Output Runnable : ";
        runnable->DisplayOutputRunnables();
    }
}

void DAG::SaveDag(std::string thisTime) {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value dagObject(rapidjson::kObjectType);
    rapidjson::Value runnableArray(rapidjson::kArrayType);
    rapidjson::Value taskArray(rapidjson::kArrayType);

    for (auto &runnable : this->runnables) {
        rapidjson::Value runnableObject(rapidjson::kObjectType);
        rapidjson::Value inputRunnableArray(rapidjson::kArrayType);
        rapidjson::Value outputRunnableArray(rapidjson::kArrayType);

        runnableObject.AddMember("ID", runnable->GetRealId(), allocator);
        runnableObject.AddMember("Execution Time", runnable->GetExecutionTime(), allocator);
        runnableObject.AddMember("Status", runnable->GetStatus(), allocator);

        for (auto &inputRunnable : runnable->GetInputRunnables()) {
            inputRunnableArray.PushBack(inputRunnable->GetRealId(), allocator);
        }
        runnableObject.AddMember("Input Runnables's ID", inputRunnableArray, allocator);

        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            outputRunnableArray.PushBack(outputRunnable->GetRealId(), allocator);
        }
        runnableObject.AddMember("Output Runnable's ID", outputRunnableArray, allocator);

        runnableArray.PushBack(runnableObject, allocator);
    }
    dagObject.AddMember("Runnables", runnableArray, allocator);

    for (auto &task : this->tasks) {
        rapidjson::Value taskObject(rapidjson::kObjectType);

        taskObject.AddMember("Period", task->GetPeriod(), allocator);
        taskObject.AddMember("Offset", task->GetOffset(), allocator);

        taskArray.PushBack(taskObject, allocator);
    }
    dagObject.AddMember("Tasks", taskArray, allocator);

    // Save to json
    std::string fileName = "../data/DAG_" + thisTime + ".json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    dagObject.Accept(writer);

    ofs.close();
}

void DAG::ParseDag(std::string jsonPath) {
    std::ifstream ifs(jsonPath.c_str());
    if (ifs.fail()) {
        throw ("File doesn't exist.");
    }

    rapidjson::IStreamWrapper isw(ifs);

    rapidjson::Document doc;
    doc.ParseStream(isw);

    std::cout << "Parse Runnables Start" << std::endl;

    std::vector<int> idToRealId;

    int runnableIndex = 0;
    for (auto &runnable : doc["Runnables"].GetArray()) {
        std::shared_ptr<RUNNABLE> tmpRunnable(new RUNNABLE(runnableIndex, runnable["ID"].GetInt(), runnable["Execution Time"].GetDouble()));
        this->runnables.push_back(tmpRunnable);
        idToRealId.push_back(runnable["ID"].GetInt());
        runnableIndex++;
    }

    runnableIndex = 0;
    for (auto &runnable : doc["Runnables"].GetArray()) {
        for (auto &outputRunnable : runnable["Output Runnable's ID"].GetArray()) {
            this->runnables[runnableIndex]->LinkOutputRunnable(this->runnables[std::distance(idToRealId.begin(), std::find(idToRealId.begin(), idToRealId.end(), outputRunnable.GetInt()))]->GetSharedPtr());
        }

        runnableIndex++;
    }

    this->SetInputRunnableList();
    this->SetOutputRunnableList();

    ifs.close();
}

void DAG::GenerateDag() {
    int numberOfRunnables;
    int numberOfTasks;

    std::cout << "[DAG Generation]\n";

    std::cout << "Number of Runnables : ";
    std::cin >> numberOfRunnables;
    this->GenerateRunnables(numberOfRunnables);

    std::cout << "Number of Tasks : ";
    std::cin >> numberOfTasks;
    this->GenerateTasks(numberOfTasks);
}