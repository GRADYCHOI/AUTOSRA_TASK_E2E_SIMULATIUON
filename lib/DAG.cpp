#include "DAG.hpp"


void DAG::SetSTatus() {
    void SetMaxCycle();
    void SetHyperPeriod();
    void SetUtilization();
}

void DAG::SetMaxCycle() {
    int minPeriod = INT_MAX;

    for (auto &task : this->tasks_) {
        if (minPeriod > task->period_) {
            minPeriod = task->period_;
        } 
    }

    this->maxCycle_ = static_cast<int>(this->GetHyperPeriod() / minPeriod);
}

void DAG::SetHyperPeriod() {
    long long int tmpPeriod = this->tasks_[0]->period_;

    for(auto &task : this->tasks_) {
        tmpPeriod = std::lcm(tmpPeriod, task->period_);
    }

    this->hyperPeriod_ = tmpPeriod;
}

void DAG::SetUtilizetion() {
    double tmpUtilization = 0.0;

    for (auto &task : this->tasks_) {
        tmpUtilization += (static_cast<double>(task->executionTime_) / static_cast<double>(task->period_));
    }

    this->utilization_ = tmpUtilization;
}

void DAG::SetUtilizetionBound() {
    double tmpUtilizationBound = static_cast<double>(this->GetNumberOfTasks()) * (std::pow(2.0, (1.0 / static_cast<double>(this->GetNumberOfTasks()))) - 1.0);

    this->utilizationBound_ = tmpUtilizationBound;
}

void DAG::SetInputRunnableList() {
    // inputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->inputRunnables_);

    for (auto &runnable : this->runnables_) {
		if (runnable->GetStatus() == 0) {
			inputRunnables_.push_back(runnable);
		}
	}
}

void DAG::SetOutputRunnableList() {
    // outputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->outputRunnables_);

    for (auto &runnable : this->runnables_) {
		if (runnable->GetStatus() == 1) {
			outputRunnables_.push_back(runnable);
		}
	}
}

void DAG::GenerateRunnables() {
    int numberOfRunnables;

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "[Runnable Generation] Number of Runnables : ";
    std::cin >> numberOfRunnables;

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "[Runnable Generation] Random Generation Start" << std::endl;

	std::clog << "===============================================[Debug : Runnable Generation}===============================================" << std::endl;
    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<RUNNABLE> runnable(new RUNNABLE(runnableIndex, runnableIndex, std::rand() % 1000));
        this->runnables_.push_back(runnable);
        std::clog << "[DAG.cpp] Runnable ID : " << runnable->id_ << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;
    }
	
	std::clog << "===========================================================================================================================" << std::endl;

    this->RandomEdge();
    this->SetInputRunnableList();
    this->SetOutputRunnableList();
	
	std::clog << "=================================================[Debug : Runnable Status]=================================================" << std::endl;
    for (auto &runnable : inputRunnables_) std::clog << "[DAG.cpp] Runnable ID : " << runnable->id_ << ", Status : " << runnable->GetStatus() << std::endl;
	
	std::cout << "[Runnable Generation] Random Generation End" << std::endl;
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void DAG::RandomEdge() { //Runnable edge random generation
    int rate = -1;

    std::cout << "[Runnable Generation] Enter Edge Rate(20~40) : ";
    std::cin >> rate; 

    for (auto &runnable : this->runnables_) {
        for (auto &outputRunnable : this->runnables_) {
            if ((rand() % 100) < rate && runnable->id_ < outputRunnable->id_) {
                runnable->LinkOutputRunnable(outputRunnable->GetSharedPtr());
            }
        }
    }
}

void DAG::GenerateTasks() {
    int numberOfTasks;

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "[Task Generation] Number of Tasks : ";
    std::cin >> numberOfTasks;

    std::cout << "\033[H\033[2J\033[3J";
	std::cout << "[Task Generation] Generation Start" << std::endl;
    int tmpPeriod = -1;
    int tmpOffset = -1;
    bool mappableFlag = false;

	std::clog << "=================================================[Debug : Task Generation}=================================================" << std::endl;
    while(!mappableFlag) {
        for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Period : ";
            std::cin >> tmpPeriod;
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Offset : ";
            std::cin >> tmpOffset;
            
            std::shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod * 1000, tmpOffset * 1000));
            this->tasks_.push_back(task);
            std::clog << "[DAG.cpp] Task ID : " << this->tasks_[taskIndex]->id_ << ", Period : " << this->tasks_[taskIndex]->GetPeriod() << ", Offset : " << this->tasks_[taskIndex]->GetOffset() << std::endl;
        }

        if (this->CheckMappable()) {
            std::cout << "This Mappable!" << std::endl;
            mappableFlag = true;
        }
        else {
            std::cout << "Increse Tasks Period!" << std::endl;
            this->ClearMapping();
            std::vector<std::shared_ptr<TASK>>().swap(this->tasks_);
            std::cout << "\033[H\033[2J\033[3J";
			std::cout << "[Task Generation] ReGeneration Tasks" << std::endl;
        }
    }
	
	std::cout << "[Task Generation] Generation End" << std::endl;
	
	std::clog << "===========================================================================================================================" << std::endl;
}

bool DAG::CheckMappable() {
    int sumOfExecutionTimes = 0;
    int maxPeriod = 0;

    for (auto &runnable : this->runnables_) {
        sumOfExecutionTimes += runnable->GetExecutionTime();
    }

    for (auto &task : this->tasks_) {
        if (maxPeriod < task->GetPeriod()) {
            maxPeriod = task->GetPeriod();
        }
    }

    this->SetUtilizationBound();

    return ((static_cast<double>(sumOfExecutionTimes) / static_cast<double>(maxPeriod)) < this->GetUtilizationBound()) ? true : false;
}

void DAG::ClearMapping() {
    for (auto &task : this->tasks_) {
        task->ClearMapping();
    }
}

void DAG::SetTaskPriority() {
    std::vector<std::shared_ptr<TASK>> tmpTaskArray(this->tasks_);

    std::clog << "\033[H\033[2J\033[3J";
	std::clog << "==================================================[Debug : Task Priority]==================================================" << std::endl;

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), [](std::shared_ptr<TASK> a, std::shared_ptr<TASK> b) { return a->period_ < b->period_; });
    std::clog << "[DAG.cpp] Sort Fin " << std::endl;

    int numberOfTasks = this->GetNumberOfTasks();
    for (int taskPriority = 0; taskPriority < numberOfTasks; taskPriority++) {
        tmpTaskArray[taskPriority]->SetPriority(taskPriority);
    }

    this->tasksInPriority_.swap(tmpTaskArray);
	
	std::clog << "===========================================================================================================================" << std::endl;
}

/* Save File & Parsing File Section */

void DAG::SaveDag(std::string subDirectory) {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value dagObject(rapidjson::kObjectType);
    rapidjson::Value runnableArray(rapidjson::kArrayType);
    rapidjson::Value taskArray(rapidjson::kArrayType);

    for (auto &runnable : this->runnables_) {
        rapidjson::Value runnableObject(rapidjson::kObjectType);
        rapidjson::Value inputRunnableArray(rapidjson::kArrayType);
        rapidjson::Value outputRunnableArray(rapidjson::kArrayType);

        runnableObject.AddMember("ID", runnable->realId_, allocator);
        runnableObject.AddMember("Execution Time", static_cast<double>(runnable->executionTime_) / 1000.0, allocator);
        runnableObject.AddMember("Status", runnable->GetStatus(), allocator);
        runnableObject.AddMember("Precedence", runnable->GetPrecedence(), allocator);

        for (auto &inputRunnable : runnable->GetInputRunnables()) {
            inputRunnableArray.PushBack(inputRunnable->realId_, allocator);
        }
        runnableObject.AddMember("Input Runnables's ID", inputRunnableArray, allocator);

        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            outputRunnableArray.PushBack(outputRunnable->realId_, allocator);
        }
        runnableObject.AddMember("Output Runnable's ID", outputRunnableArray, allocator);

        runnableArray.PushBack(runnableObject, allocator);
    }
    dagObject.AddMember("Runnables", runnableArray, allocator);

    for (auto &task : this->tasks_) {
        rapidjson::Value taskObject(rapidjson::kObjectType);

        taskObject.AddMember("Period", static_cast<double>(task->period_) / 1000.0, allocator);
        taskObject.AddMember("Offset", static_cast<double>(task->offset_) / 1000.0, allocator);

        taskArray.PushBack(taskObject, allocator);
    }
    dagObject.AddMember("Tasks", taskArray, allocator);

    // Save to json
    std::string fileName = "../data/" + subDirectory + "/DAG.json";

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
        std::shared_ptr<RUNNABLE> tmpRunnable(new RUNNABLE(runnableIndex, runnable["ID"].GetInt(), static_cast<int>(runnable["Execution Time"].GetDouble() * 1000.0)));
        tmpRunnable->SetPrecedence(runnable["Precedence"].GetInt());
        this->runnables_.push_back(tmpRunnable);
        idToRealId.push_back(runnable["ID"].GetInt());
        runnableIndex++;
    }

    runnableIndex = 0;
    for (auto &runnable : doc["Runnables"].GetArray()) {
        for (auto &outputRunnable : runnable["Output Runnable's ID"].GetArray()) {
            this->runnables_[runnableIndex]->LinkOutputRunnable(this->runnables_[std::distance(idToRealId.begin(), std::find(idToRealId.begin(), idToRealId.end(), outputRunnable.GetInt()))]->GetSharedPtr());
        }

        runnableIndex++;
    }

    this->SetInputRunnableList();
    this->SetOutputRunnableList();

    std::cout << "Parse Tasks Start" << std::endl;
	
	int taskIndex = 0;
	for (auto &task : doc["Tasks"].GetArray()) {
        std::shared_ptr<TASK> tmpTask(new TASK(taskIndex, static_cast<int>(task["Period"].GetDouble() * 1000.0), static_cast<int>(task["Offset"].GetDouble() * 1000.0)));
        this->tasks_.push_back(tmpTask);
        taskIndex++;
    }

    ifs.close();
}

void DAG::ParseMapping(std::string jsonPath) {
    std::ifstream ifs(jsonPath.c_str());
    if (ifs.fail()) {
        throw ("File doesn't exist.");
    }

    rapidjson::IStreamWrapper isw(ifs);

    rapidjson::Document doc;
    doc.ParseStream(isw);

    std::cout << "Parse Mapping Start" << std::endl;

    this->ClearTaskMapping();
    std::vector<std::shared_ptr<TASK>>().swap(this->tasks_);
	
	int taskIndex = 0;
	for (auto &task : doc["Tasks"].GetArray()) {
        std::shared_ptr<TASK> tmpTask(new TASK(taskIndex, static_cast<int>(task["Period"].GetDouble() * 1000.0), static_cast<int>(task["Offset"].GetDouble() * 1000.0)));
        tmpTask->SetPriority(task["Priority"].GetInt());
        tmpTask->SetCore(task["Core"].GetInt());
        this->tasks_.push_back(tmpTask);
        taskIndex++;

        for (auto &runnableRealId : task["Runnables"].GetArray()) {
            auto iter = std::find_if(this->runnables_.begin(), this->runnables_.end(), [&runnableRealId](std::shared_ptr<RUNNABLE> a) { return a->GetRealId() == runnableRealId.GetInt(); });
            if (this->runnables_.end() != iter) {
                tmpTask->AddRunnable(*iter);
            }
        }
    }

    ifs.close();
}

void DAG::SaveMapping(std::string subDirectory) {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value dagObject(rapidjson::kObjectType);
    rapidjson::Value runnableArray(rapidjson::kArrayType);
    rapidjson::Value taskArray(rapidjson::kArrayType);

    for (auto &task : this->tasks_) {
        rapidjson::Value taskObject(rapidjson::kObjectType);
        rapidjson::Value runnableArray(rapidjson::kArrayType);

        taskObject.AddMember("Period", static_cast<double>(task->period_) / 1000.0, allocator);
        taskObject.AddMember("Offset", static_cast<double>(task->offset_) / 1000.0, allocator);
        taskObject.AddMember("Priority", task->GetPriority(), allocator);
        taskObject.AddMember("Core", task->GetCore(), allocator);

        for (auto &runnable : task->GetRunnables()) {
            runnableArray.PushBack(runnable->realId_, allocator);
        }
        taskObject.AddMember("Runnables", runnableArray, allocator);

        taskArray.PushBack(taskObject, allocator);
    }
    dagObject.AddMember("Tasks", taskArray, allocator);

    // Save to json
    std::string fileName = "../data/" + subDirectory + "/Mapping.json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    dagObject.Accept(writer);

    ofs.close();
}