#include "DAG.hpp"


float LCM(float a, float b) {
    unsigned long long int tmp1 = static_cast<unsigned long long int>(static_cast<double>(a) * 1e+10f);
    unsigned long long int tmp2 = static_cast<unsigned long long int>(static_cast<double>(b) * 1e+10f);

    while (tmp2 != 0) {
    	unsigned long long int tmp3 = tmp1 % tmp2;
        tmp1 = tmp2;
        tmp2 = tmp3;
    }

    return static_cast<float>(static_cast<double>(static_cast<unsigned long long int>(static_cast<double>(a) * 1e+10f) * (static_cast<unsigned long long int>(static_cast<double>(b) * 1e+10f) / tmp1)) * 1e-10f);
}

const int DAG::GetMaxCycle() {
    float minPeriod = DBL_MAX;

    for (auto &task : this->tasks_) {
        if (minPeriod > task->GetPeriod()) {
            minPeriod = task->GetPeriod();
        } 
    }

    return static_cast<int>(this->GetHyperPeriod() / minPeriod);
}

const float DAG::GetHyperPeriod() {
    float tmpPeriod = this->GetTasksPriority()[0]->GetPeriod();

    for(auto& orderOfPriorityTasks : this->GetTasksPriority()) {
        tmpPeriod = LCM(static_cast<int>(tmpPeriod), static_cast<int>(orderOfPriorityTasks->GetPeriod()));
    }

    return tmpPeriod;
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

void DAG::GenerateDag() {
    int numberOfRunnables;
    int numberOfTasks;

    std::cout << "[DAG Generation]\n";

    std::cout << "[Runnable Generation] Number of Runnables : ";
    std::cin >> numberOfRunnables;
    this->GenerateRunnables(numberOfRunnables);

    std::cout << "[Task Generation] Number of Tasks : ";
    std::cin >> numberOfTasks;
    this->GenerateTasks(numberOfTasks);
}

void DAG::GenerateRunnables(int numberOfRunnables) {
    std::cout << "[Runnable Generation] Random Generation Start" << std::endl;

	std::clog << "===============================================[Debug : Runnable Generation}===============================================" << std::endl;
    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<RUNNABLE> runnable(new RUNNABLE(runnableIndex, runnableIndex, (float)(std::rand() % 100) / 1000));
        this->runnables_.push_back(runnable);
        std::clog << "[DAG.cpp] Runnable ID : " << runnable->GetId() << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;
    }
	
	std::clog << "===========================================================================================================================" << std::endl;

    this->RandomEdge();
    this->SetInputRunnableList();
    this->SetOutputRunnableList();
	
	std::clog << "=================================================[Debug : Runnable Status]=================================================" << std::endl;
    for (auto &run : inputRunnables_) std::clog << "[DAG.cpp] Runnable ID : " << run->GetId() << ", Status : " << run->GetStatus() << std::endl;
	
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
    float tmpPeriod = -1.0f;
    float tmpOffset = -1.0f;
    bool mappableFlag = false;

	std::clog << "=================================================[Debug : Task Generation}=================================================" << std::endl;
    while(!mappableFlag) {
        for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Period : ";
            std::cin >> tmpPeriod;
            std::cout << "[Task Generation] " << taskIndex << " -th Task's Offset : ";
            std::cin >> tmpOffset;
            std::shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod, tmpOffset));
            this->tasks_.push_back(task);
            std::clog << "[DAG.cpp] Task ID : " << this->tasks_[taskIndex]->GetId() << ", Period : " << this->tasks_[taskIndex]->GetPeriod() << ", Offset : " << this->tasks_[taskIndex]->GetOffset() << std::endl;
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
    float sumOfExecutionTimes = 0.0f;
    float maxPeriod = 0.0f;

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
}

float DAG::GetUtilization() {
    float tmpUtilization = 0.0f;

    for (auto &task : this->tasks_) {
        tmpUtilization += (task->GetExecutionTime() / task->GetPeriod());
    }

    return tmpUtilization;
}

void DAG::SetTaskPriority() {
    std::vector<std::pair<int, float>> tmpTaskArray; // ID, Period
	tmpTaskArray.reserve(this->GetNumberOfTasks());

	std::clog << "==================================================[Debug : Task Priority]==================================================" << std::endl;
    for (auto &task : this->tasks_) {
        tmpTaskArray.push_back(std::make_pair(task->GetId(), task->GetPeriod()));
        std::clog << "[DAG.cpp] Task ID : " << task->GetId() << ", Period : " << task->GetPeriod() << ", Offset : " << task->GetOffset() << std::endl;
    }

    std::sort(tmpTaskArray.begin(), tmpTaskArray.end(), [](std::pair<int, float> a, std::pair<int, float> b) { return a.second < b.second; });
    std::clog << "[DAG.cpp] Sort Fin " << std::endl;

    int taskSize = static_cast<int>(tmpTaskArray.size());
    for (int taskPriority = 0; taskPriority < taskSize; taskPriority++) {
        this->tasks_[tmpTaskArray[taskPriority].first]->SetPriority(taskPriority);
    }
	
	std::clog << "===========================================================================================================================" << std::endl;
}

const std::vector<std::shared_ptr<TASK>> DAG::GetTasksPriority() {
    std::sort(this->tasks_.begin(), this->tasks_.end(), [](std::shared_ptr<TASK> a, std::shared_ptr<TASK> b) { return a->GetPriority() < b->GetPriority(); });

    return this->tasks_;
}

void DAG::SetRunnablePrecedence() {
    // runnablePrecedence initialize
    std::vector<int> precedenceOfRunnables(this->GetNumberOfRunnables(), -1);

	std::clog << "===============================================[Debug : Runnable Precedence]===============================================" << std::endl;
    for (auto &inputRunnable : this->inputRunnables_) {
		std::clog << " Input Runnable ID : " << inputRunnable->GetId() << std::endl;
	}

    for (auto &inputRunnable : this->inputRunnables_) {
		this->CheckPrecedence(precedenceOfRunnables, inputRunnable, 0);
	}
	
	for (auto &runnable : this->runnables_) {
        std::clog << " [Set Precedence] Runnable ID : " << runnable->GetId() << ", Precedence : " << precedenceOfRunnables[runnable->GetId()] << std::endl;
		runnable->SetPrecedence(precedenceOfRunnables[runnable->GetId()]);
	}
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void DAG::CheckPrecedence(std::vector<int>& precedenceOfRunnables, const std::shared_ptr<RUNNABLE>& runnable, int precedence) {
    std::clog << "============================================[Debug : Runnable Check Precedence]============================================" << "\n";
    std::clog << " Runnable ID : " << runnable->GetId() << std::endl;

    if (precedenceOfRunnables[runnable->GetId()] < precedence) {
		precedenceOfRunnables[runnable->GetId()] = precedence;
	}
	
	if (runnable->GetStatus() != 1) {
        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            if (precedenceOfRunnables[outputRunnable->GetId()] < ++precedence) {
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

    for (auto &runnable : this->runnables_) {
        rapidjson::Value runnableObject(rapidjson::kObjectType);
        rapidjson::Value inputRunnableArray(rapidjson::kArrayType);
        rapidjson::Value outputRunnableArray(rapidjson::kArrayType);

        runnableObject.AddMember("ID", runnable->GetRealId(), allocator);
        runnableObject.AddMember("Execution Time", runnable->GetExecutionTime(), allocator);
        runnableObject.AddMember("Status", runnable->GetStatus(), allocator);
        runnableObject.AddMember("Precedence", runnable->GetPrecedence(), allocator);

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

    for (auto &task : this->tasks_) {
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
        std::shared_ptr<RUNNABLE> tmpRunnable(new RUNNABLE(runnableIndex, runnable["ID"].GetInt(), runnable["Execution Time"].GetFloat()));
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
	
	int taskIndex = 0;
	for (auto &task : doc["Tasks"].GetArray()) {
        std::shared_ptr<TASK> tmpTask(new TASK(taskIndex, task["Period"].GetFloat(), task["Offset"].GetFloat()));
        this->tasks_.push_back(tmpTask);
        taskIndex++;
    }

    ifs.close();
}