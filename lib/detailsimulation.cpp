#include "detailsimulation.h"


/*
void DetailSimulation::Initialize() {
    this->InitializeSequenceCommand();

    // Initialize Dag's Informations
    this->dag_->SetStatus();
    this->maxCycle_ = this->dag_->GetMaxCycle();
    this->hyperPeriod_ = static_cast<double>(this->dag_->GetHyperPeriod());
    this->numberOfTasks_ = this->dag_->GetNumberOfTasks();
    this->numberOfRunnables_ = this->dag_->GetNumberOfRunnables();
    this->numberOfInputRunnables_ = this->dag_->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables_ = this->dag_->GetNumberOfOutputRunnables();
    this->utilization_ = this->dag_->GetUtilization();
    this->utilizationBound_ = this->dag_->GetUtilizationBound();

    // Pre-save Dag & Mapping   
    this->SetDataDirectory();
    this->MakeDataDirectory();
    this->dag_->SaveDag(this->dataDirectory_);
    this->dag_->SaveMapping(this->dataDirectory_);

    this->DisplayDag();
}

void DetailSimulation::SetDataDirectory() {
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    std::string simulationTime = std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday) + "_" + std::to_string(pTimeInfo->tm_hour) + "_" + std::to_string(pTimeInfo->tm_min);
    this->dataDirectory_ = "../data/T" + std::to_string(this->numberOfTasks_) + "R" + std::to_string(this->numberOfRunnables_) + "U" + std::to_string(this->utilization_) + "W" + simulationTime;
}

void DetailSimulation::MakeDataDirectory() {
    // make directory for save datas
    if (mkdir(this->dataDirectory_.c_str(), 0776) == -1 && errno != EEXIST) {
        // std::cerr << "[AUTOSAR Simulator] directory create error : " << strerror(errno) << std::endl;
        throw std::invalid_argument("'data' directory is not exist");
    }
}


void DetailSimulation::Simulate(std::map<int, std::map<int, std::vector<ResultInformation>>>& results) {
    // Create Large vector
    this->CreateProcessExecutions();
    this->CreateVisitedWorstCycleList();

    this->SetSequence(simulationIndex);

    std::clog << "[simulation.cpp] Checkpoint 4\n";
    ResultInformation result = this->GetResult();

    this->DisplayDetailTarget(result);

    std::clog << "[simulation.cpp] Checkpoint 5\n";
    this->SaveDataToCSV(simulationIndex, result);

    std::clog << "[simulation.cpp] Checkpoint 6\n";
}

void DetailSimulation::InitializeResultsDetail(std::map<int, std::map<int, std::vector<ResultInformation>>>& results, int seedNumber) {
    int maxCycle = -1;

    for (auto &inputRunnablePair : this->processExecutions_) {
        auto inputRunnableIter = results.insert({inputRunnablePair.first, std::map<int, std::vector<ResultInformation>>()});

        for (auto &outputRunnablePair : inputRunnablePair.second) {
            maxCycle = static_cast<int>(outputRunnablePair.second.size());

            auto outputRunnableIter = inputRunnableIter->insert({outputRunnablePair.first, std::vector<ResultInformation>(maxCycle, {seedNumber, -1, -1})});
        }
    }
}


void DetailSimulation::GetReactionTimeDetail(std::map<int, std::map<int, std::vector<ResultInformation>>>& results) {
    long long int tmpEndTime = -1;
    int maxCycle = -1;

    for (auto &inputRunnablePair : this->processExecutions_) {
        for (auto &outputRunnablePair : inputRunnablePair.second) {
            maxCycle = static_cast<int>(outputRunnablePair.second.size());
            tmpEndTime = outputRunnablePair.second[maxCycle - 1].endTime;

            outputRunnablePair.second[maxCycle - 1].reactionTime = (outputRunnablePair.second[maxCycle - 1].endTime - outputRunnablePair.second[maxCycle - 1].startTime;

            for (int index = maxCycle - 1; index >= 0; --index) {
                if (tmpEndTime > outputRunnablePair.second[index].endTime) {
                    tmpEndTime = outputRunnablePair.second[index].endTime;
                    outputRunnablePair.second[index].reactionTime = (outputRunnablePair.second[index].endTime - outputRunnablePair.second[index].startTime;
                } else {
                    outputRunnablePair.second[index].reactionTime = -1;
                }
            }
        }
    }
}

void DetailSimulation::GetDataAgeDetail(std::map<int, std::map<int, std::vector<ResultInformation>>>& results) {
    long long int tmpEndTime = -1;
    int maxCycle = -1;
    int startCycle = -1;
    int endCycle = -1;

    for (auto &inputRunnablePair : this->processExecutions_) {
        for (auto &outputRunnablePair : inputRunnablePair.second) {
            maxCycle = static_cast<int>(outputRunnablePair.second.size());
            tmpEndTime = outputRunnablePair.second[maxCycle - 1].endTime;
            std::shared_ptr<RUNNABLE> outputRunnable = this->dag_->GetRunnable(outputRunnablePair.first);

            for (int index = maxCycle - 1; index >= 0; --index) {
                if (tmpEndTime > outputRunnablePair.second[index].endTime) {
                    endCycle = (tmpEndTime % this->hyperPeriod_) / outputRunnable->GetTask()->period_;

                    tmpEndTime = outputRunnablePair.second[index].endTime;
                    startCycle = (tmpEndTime % this->hyperPeriod_) / outputRunnable->GetTask()->period_ + 1;

                    if (endCycle > startCycle) {
                        outputRunnablePair.second[maxCycle - 1].reactionTime = outputRunnable->executionTimes_[endCycle].endTime - outputRunnable->executionTimes_[startCycle].endTime;
                    } else {
                        outputRunnablePair.second[maxCycle - 1].reactionTime = outputRunnable->executionTimes_[endCycle].endTime - outputRunnable->executionTimes_[startCycle].endTime;
                    }
                }
            }
        }
    }
}

// ################################ Save Data : CSV #################################

void DetailSimulation::SaveProcessTimeToCSV() {

}

void Simulation::SaveDetailToCSV(int inputRunnableId, int outputRunnableId) {
    std::string fileDirectory = this->dataDirectory_ + "/Result" + inputRunnableId +  "to" + outputRunnableId + ".csv";

    std::sort(this->results_.begin(), this->results_.end(), [](ResultInformation &a, ResultInformation &b) { return a.reactionTime < b.reactionTime; });

    std::ofstream resultFile;
    resultFile.open (fileDirectory.c_str(), std::ios_base::out | std::ios_base::app);

    for (auto &result : this->results_) {
        resultFile << result.seedNumber << "," << static_cast<double>(result.reactionTime / 1000) + (static_cast<double>(result.reactionTime % 1000) / 1000.0) << "," << static_cast<double>(result.dataAge / 1000) + (static_cast<double>(result.dataAge % 1000) / 1000.0) << "\n";
    }

    resultFile.close();
}

// ################################ Save Data : JSON #################################

void Simulation::SaveDetailToJson() {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value resultObject(rapidjson::kObjectType);
    resultObject.AddMember("Seed Number", this->results_[0].seedNumber, allocator);

    rapidjson::Value reactionTime(rapidjson::kArrayType);
    reactionTime.AddMember("Reaction Time", this->results_[0].reactionTime, allocator);

    rapidjson::Value onePath(rapidjson::kArrayType);
    for (auto &task : this->dag_->GetTasks()) {
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

        onePath.PushBack(taskObject, allocator);
    }
    reactionTime.AddMember("Tasks", onePath, allocator);

    // Save to json
    std::string fileName = this->dataDirectory_ + "/Detail_" + std::to_string(this->results_[0].seedNumber) + ".json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    dagObject.Accept(writer);

    ofs.close();
}

// ################################ Parse Data : JSON #################################

ResultInformation Simulation::ParseData(int seedNumber) {
    std::string fileDirectory = this->dataDirectory_ + "/Result.csv";
    ResultInformation result = {-1, -1, -1};

    std::ifstream resultFile;
    resultFile.open (fileDirectory.c_str(), std::ios_base::in);

    if (resultFile.fail()) { // Exception
        throw std::invalid_argument("Result.csv isn't exist in Directory");
    }

    std::string stringSeedNumber = std::to_string(seedNumber);
    std::string stringBuffer;
    while (std::getline(resultFile, stringBuffer, ',')) {
        if (stringSeedNumber == stringBuffer) {
            result.seedNumber = std::stoi(stringBuffer);

            std::getline(resultFile, stringBuffer, ',');
            result.reactionTime = static_cast<long long int>(std::stold(stringBuffer) * 1000.0);

            std::getline(resultFile, stringBuffer, ',');
            result.dataAge = static_cast<long long int>(std::stold(stringBuffer) * 1000.0);

        } else {
            std::getline(resultFile, stringBuffer, ','); // waste reactionTime Cols
            std::getline(resultFile, stringBuffer, ','); // waste dataAge Cols
        }
    }

    if (result.seedNumber == -1) { // Can't Read Seed Number
        throw std::invalid_argument("Can't read seed number in Result.csv");
    }

    resultFile.close();
}

void Simulation::DisplayDetailTarget(ResultInformation& result) {
    //std::cout << "\033[H\033[2J\033[3J";
    std::cout << "===========================================================================================================================\n";
    std::cout << " - Simulation Seed            : " << std::setw(23) << result.seedNumber << "\n";
    std::cout << " - Reaction Time              : " << std::setw(16) << result.reactionTime / 1000 << "." << std::setw(3) << result.reactionTime % 1000 << " ms\n";
    std::cout << " - Data Age                   : " << std::setw(20) << static_cast<double>(result.dataAge) / 1000.0 << " ms\n";
    std::cout << " - Max Cycle                  : " << std::setw(17) << this->maxCycle_ << " cycle\n";
    std::cout << " - Hyper Period               : " << std::setw(20) << static_cast<double>(this->hyperPeriod_) / 1000.0 << " ms\n";
    std::cout << " - Number Of Tasks            : " << std::setw(23) << this->numberOfTasks_ << "\n";
    std::cout << " - Number Of Runanbles        : " << std::setw(23) << this->numberOfRunnables_ << "\n";
    std::cout << " - Number Of Input Runnables  : " << std::setw(23) << this->numberOfInputRunnables_ << "\n";
    std::cout << " - Number Of Output Runnables : " << std::setw(23) << this->numberOfOutputRunnables_ << "\n";
    std::cout << " - Utilization                : " << std::setw(23) << this->utilization_ << "\n";
    std::cout << " - Utilization Bound          : " << std::setw(23) << this->utilizationBound_ << "\n";
    std::cout << "===========================================================================================================================" << std::endl;
}

*/