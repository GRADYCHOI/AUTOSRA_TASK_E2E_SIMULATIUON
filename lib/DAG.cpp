#include "DAG.hpp"


/*
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
*/

int DAG::InitializeMaxCycle() {
    double minPeriod = DBL_MAX;

    for (auto &task : this->tasks_) {
        if (minPeriod > task->GetPeriod()) {
            minPeriod = task->GetPeriod();
        } 
    }

    return static_cast<int>(this->GetHyperPeriod() / minPeriod);
}

double DAG::InitializeHyperPeriod() {
    double tmpPeriod = this->GetOrderOfPriorityTasks()[0]->GetPeriod();

    for(auto& orderOfPriorityTasks : this->GetOrderOfPriorityTasks()) {
        tmpPeriod = std::lcm(tmpPeriod, orderOfPriorityTasks->GetPeriod());
    }

    return tmpPeriod;
}

void DAG::SetInputRunnableList() {
    // inputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->inputRunnables_);

    for (auto &runnable : this->runnables_) {
		if (runnable->GetStatus() == 0) {
			inputRunnables.push_back(runnable);
		}
	}
}

void DAG::SetOutputRunnableList() {
    // outputRunnables clear
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    tmpList.swap(this->outputRunnables_);

    for (auto &runnable : this->runnables_) {
		if (runnable->GetStatus() == 1) {
			outputRunnables.push_back(runnable);
		}
	}
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

void DAG::GenerateRunnables(int numberOfRunnables) {
    std::cout << "[Runnable Generation] Random Generation Start" << std::endl;

	std::clog << "===============================================[Debug : Runnable Generation}===============================================" << std::endl;
    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<RUNNABLE> runnable(new RUNNABLE(runnableIndex, runnableIndex, (double)(std::rand() % 100) / 1000));
        this->runnables_.push_back(runnable);
        std::clog << "[DAG.cpp] Runnable ID : " << runnable->GetId() << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;
    }
	
	std::clog << "===========================================================================================================================" << std::endl;

    this->RandomEdge();
    this->SetInputRunnableList();
    this->SetOutputRunnableList();
	
	std::clog << "=================================================[Debug : Runnable Status]=================================================" << std::endl;
    for (auto &run : inputRunnables) std::clog << "DAG.cpp] Runnable ID : " run->GetId() << ", Status : " << run->GetStatus() << std::endl;
	
	std::cout << "[Runnable Generation] Random Generation End" << std::endl;
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void DAG::RandomEdge() { //Runnable edge random generation
    int rate = -1;

    std::cout << "[Runnable Generation] Enter Edge Rate(20~40) : ";
    std::cin >> rate; 

    for (auto &runnable : this->runnables_) {
        for (auto &outputRunnable : this->runnables_) {
            if ((rand() % 100) < rate && runnable->GetId() < outputRunnable->GetId()) {
                runnable->LinkOutputRunnable(outputRunnable->GetSharedPtr());
            }
        }
    }
}

void DAG::GenerateTasks(int numberOfTasks) {
	std::cout << "[Task Generation] Generation Start" << std::endl;
    double tmpPeriod = -1.0;
    double tmpOffset = -1.0;
    bool mappableFlag = false;

	std::clog << "=================================================[Debug : Task Generation}=================================================" << std::endl;
    while(!mappableFlag) {
        for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Period : ";
            std::cin >> tmpPeriod;
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Offset : ";
            std::cin >> tmpOffset;
            std::shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod, tmpOffset));
            this->tasks.push_back(task);
            std::clog << "[DAG.cpp] Task ID : " << tasks[taskIndex]->GetId() << ", Period : " << tasks[taskIndex]->GetPeriod() << ", Offset : " << tasks[taskIndex]->GetOffset() << std::endl;
        }

        if (this->CheckMappable()) {
            std::cout << "This Mappable!" << std::endl;
            mappableFlag = true;
        }
        else {
            std::cout << "Increse Tasks Period!" << std::endl;
            this->ClearTaskMapping();
            std::vector<std::shared_ptr<TASK>>().swap(this->tasks_);
			std::cout << "[Task Generation] ReGeneration Tasks" << std::endl;
        }
    }
	
	std::cout << "[Task Generation] Generation End" << std::endl;
	
	std::clog << "===========================================================================================================================" << std::endl;
}

bool DAG::CheckMappable() {
    double sumOfExecutionTimes = 0.0;
    double maxPeriod = 0.0;

    for (auto &runnable : this->runnables_) {
        sumOfExecutionTimes += runnable->GetExecutionTime();
    }

    for (auto &task : this->tasks_) {
        if (maxPeriod < task->GetPeriod()) {
            maxPeriod = task->GetPeriod();
        }
    }

    return ((sumOfExecutionTimes / maxPeriod) < UTILIZATION) ? true : false;
}

void DAG::ClearTaskMapping() {
    for (auto &task : this->tasks_) {
        task->ClearMapping();
    }
	
	this->maxCycle_ = -1;
	this->hyperPeriod_ = -1.0;
}

double DAG::GetUtilization() {
    double tmpUtilization = 0.0;

    for (auto &task : this->tasks_) {
        tmpUtilization += (task->GetExecutionTime() / task->GetPeriod());
    }

    return tmpUtilization;
}

void DAG::SetTaskPriority() {
    std::vector<std::pair<int, double>> tmpTaskArray; // ID, Period
	tmpTaskArray.reserve(this->GetNumberOfTasks);

	std::clog << "==================================================[Debug : Task Priority]==================================================" << std::endl;
    for (auto &task : this->tasks_) {
        tmpTaskArray.push_back(std::make_pair(task->GetId(), task->GetPeriod()));
    }

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), [](std::pair<int, double> a, std::pair<int, double> b) { return a.second < b.second; });

	this->taskPriority_.reserve(this->GetNumberOfTasks());
    for (auto &tmpTask : tmpTaskArray) {
        this->taskPriority_.push_back(this->task_[tmpTask.first]);
    }
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void DAG::SetRunnablePrecedence() {
    // runnablePrecedence initialize
    std::vector<int> precedenceOfRunnables(this->GetNumberOfRunnables(), -1);
	std::vector<std::pair<int, int>> tmpPrecedenceOfRunnables; // ID, Precedence
	

	std::clog << "===============================================[Debug : Runnable Precedence]===============================================" << std::endl;
    for (auto &inputRunnable : this->inputRunnables) {
		CheckPrecedence(tmpPrecedenceOfRunnables, inputRunnable, 0);
	}
	
	for (auto &runnable : this->runnables_) {
		runnable->SetPrecedence(precedenceOfRunnables[runnable->GetId()]);
	}
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void DAG::CheckPrecedence(std::vector<int>& precedenceOfRunnables, std::shared_ptr<RUNNABLE> runnable, int precedence) {
    if (precedenceOfRunnables[runnable->GetId()] < precedence) {
		precedenceOfRunnables[runnable->GetId()] = precedence;
	}
	
	if (runnable->GetStatus() == 2) {
        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            if (runnablePrecedence[outputRunnable->GetId()] < ++precedence) {
				CheckPrecedence(precedenceOfRunnables, outputRunnable, precedence);
			}
        }
    }
}

/* Save File & Parsing File Section */

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
	
	int taskIndex = 0;
	for (auto &task : doc["Tasks"].GetArray()) {
        std::shared_ptr<TASK> tmpTask(new TASK(taskIndex, task["Period"].GetDouble(), task["Offset"].GetDouble()));
        this->tasks.push_back(tmpTask);
        taskIndex++;
    }

    ifs.close();
}