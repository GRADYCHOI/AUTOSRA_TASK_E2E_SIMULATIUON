#include "simulation.hpp"


void Simulation::Initialize() {
    this->dag_->SetStatus();

    this->maxCycle_ = this->dag_->GetMaxCycle();
    this->hyperPeriod_ = static_cast<double>(this->dag_->GetHyperPeriod()) / 1000.0;
    this->numberOfTasks_ = this->dag_->GetNumberOfTasks();
    this->numberOfRunnables_ = this->dag_->GetNumberOfRunnables();
    this->numberOfInputRunnables_ = this->dag_->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables_ = this->dag_->GetNumberOfOutputRunnables();
    this->utilization_ = this->dag_->GetUtilization();
    this->utilizationBound_ = this->dag_->GetUtilizationBound();

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "===========================================================================================================================\n";
    std::cout << " - Max Cycle                  : " << this->maxCycle_ << "\n";
    std::cout << " - Hyper Period               : " << this->hyperPeriod_ << "\n";
    std::cout << " - Number Of Tasks            : " << this->numberOfTasks_ << "\n";
    std::cout << " - Number Of Runanbles        : " << this->numberOfRunnables_ << "\n";
    std::cout << " - Number Of Input Runnables  : " << this->numberOfInputRunnables_ << "\n";
    std::cout << " - Number Of Output Runnables : " << this->numberOfOutputRunnables_ << "\n";
    std::cout << " - Utilization                : " << this->utilization_ << "\n";
    std::cout << " - Utilization Bound          : " << this->utilizationBound_ << "\n";
    std::cout << "===========================================================================================================================" << std::endl;

    // About File
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    std::string simulationTime =  + std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday);
    this->dataDirectory_ = "../data/" + "T" + std::to_string(this->numberOfTasks_) + "R" + std::to_string(this->numberOfRunnables_) + "U" + std::to_string(this->utilization_) + "W" + simulationTime;

    // make directory for save datas
    if (mkdir(this->dataDirectory_, 0776) == -1 && errno != EEXIST) {
        std::cerr << strerror(errno) << " directory create error : " << strerror(errno);
        return -1;
    }

    // Save Dag & Mapping
    this->dag_->SaveDag(this->dataDirectory_);
    this->dag_->SaveMapping(this->dataDirectory_);
}

int Simulation::GetNumberOfCase() {
    int numberOfCase = 1;

    for (auto &taskIndexSequence : this->sequenceMatrix_) {
        for (auto &precedenceIndexSequence = 0; taskIndexSequence) {
            numberOfCase *= static_cast<int>(precedenceIndexSequence.size());
        }
    }

    return numberOfCase;
}

// TODO : sequenceMatrix에 맞춰 변경 후 task에도 반영시키기
void Simulation::SetSequence(int caseIndex) {
    this->visitiedPermutationNumber_[caseIndex] = true;

    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        int caseNumber = caseIndex;
        std::vector<int> executionPermutationPointer;
        executionPermutationPointer.reserve(this->numberOfRunnables_);

        for (auto &executionPermutation : runnableExecutionPermutation) {
            int permutationPointerNumber = caseNumber % static_cast<int>(executionPermutation.size());
            caseNumber /= static_cast<int>(executionPermutation.size());
            
            for (auto &runnableId : executionPermutation[permutationPointerNumber]) {
                executionPermutationPointer.emplace_back(runnableId);
            }
        }

        this->sequence_.emplace_back(executionPermutationPointer);
    }
}

void Simulation::SetSequenceMatrix() {
    std::vector<std::vector<std::vector<int>>> sequenceMatrix;

    int taskIndex = 0;
    for (auto &task : this->dag_->GetTasks()) {
        int currentPrecedence = -1;
        task->SortRunnablesByPrecedence();
        sequenceMatrix.emplace_back(std::vector<std::vector<int>>());

        int precedenceIndex = 0;
        for (auto &runnable : task->GetRunnables()) {
            if (currentPrecedence == runnable->GetPrecedence()) {
                sequenceMatrix[taskIndex][precedenceIndex].emplace_back(runnable->id_);
            } else {
                sequenceMatrix[taskIndex].emplace_back(std::vector<int>(1, runnable->id_));
                precedenceIndex += 1;
            }

            currentPrecedence = runnable->GetPrecedence();
        }

        taskIndex += 1;
    }

    sequenceMatrix.swap(this->sequenceMatrix_);
}

void Simulation::Simulate() {
    this->SetSequenceMatrix();

    int numberOfCase = this->GetNumberOfCase();
    std::vector<bool>(numberOfCase, false).swap(this->visitiedPermutationNumber_);

    // For Reduce malloc delay
    this->CreateProcessExecutions();
    this->CreateWorstCycle();

    std::random_device rd;
    std::mt19937 gen(rd());

    std::clock_t simulationStart = std::clock();
    std::clock_t simulationCheckpoint;
    for (int caseIndex = numberOfCase; caseIndex > 0; caseIndex--) {
        std::uniform_int_distribution<int> dis(0, caseIndex);
        int simulationSeed = dis(gen());
        int simulationIndex = -1;

        for (int tmpSimulationIndex = 0; tmpSimulationIndex <= simulationSeed; tmpSimulationIndex++) {
            while (this->visitiedPermutationNumber_[++simulationIndex]);
        }

        this->SetSequence(simulationIndex);
        ResultInformation result = this->GetResult();

        this->SaveDataToCSV(simulationIndex, result);

        simulationCheckpoint = std::clock();
		
		std::cout << "\033[H\033[2J\033[3J";
		std::cout << "===========================================================================================================================\n";
        std::cout << " - Simulation Case            : " << std::setw(10) << (caseIndex + 1) << " / " << std::setw(10) << numberOfCase << "\n";
        std::cout << " - Simulation Seed            : " << std::setw(23) << simulationIndex << "\n";
        std::cout << " - Reaction Time              : " << std::setw(23) << static_cast<double>(result.reactionTime) / 1000.0 << "ms\n";
        std::cout << " - Data Age                   : " << std::setw(23) << static_cast<double>(result.dataAge) / 1000.0 << "ms\n";
        std::cout << " - Max Cycle                  : " << std::setw(23) << this->maxCycle_ << "\n";
        std::cout << " - Hyper Period               : " << std::setw(23) << static_cast<double>(this->hyperPeriod_) / 1000.0 << "ms\n";
        std::cout << " - Number Of Tasks            : " << std::setw(23) << this->numberOfTasks_ << "\n";
        std::cout << " - Number Of Runanbles        : " << std::setw(23) << this->numberOfRunnables_ << "\n";
        std::cout << " - Number Of Input Runnables  : " << std::setw(23) << this->numberOfInputRunnables_ << "\n";
        std::cout << " - Number Of Output Runnables : " << std::setw(23) << this->numberOfOutputRunnables_ << "\n";
        std::cout << " - Utilization                : " << std::setw(23) << this->utilization_ << "\n";
        std::cout << " - Utilization Bound          : " << std::setw(23) << this->utilizationBound_ << "\n";
        std::cout << " - Simulation Process Time    : " << std::setw(23) << static_cast<double>(simulationCheckpoint - simulationStart) / CLOCKS_PER_SEC << "ms\n"
		std::cout << "===========================================================================================================================" << std::endl;

        
        if (static_cast<long long int>(simulationCheckpoint - simulationStart) > limitProcessTime) {
            break;
        }
    }
}

ResultInformation Simulation::GetResult() {
    this->GetRunnableCommunications(runnableTimeTable);
    this->SetProcessExecutions();

    ResultInformation result;
    result.reactionTime = this->GetMaxReactionTime();
    result.dataAge = this->GetMaxDataAge();

    return result;
}

void Simulation::SetProcessExecutions() {
    this->InitializeProcessExecutions();

    for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
        this->InitializeVisitedWorstCycle();
        auto inputRunnableIter = this->processExecutions_.find(inputRunnable->id_);

        for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
            this->TraceProcess(inputRunnableIter, cycle, inputRunnable, cycle, 0);
        }
    }
}

void Simulation::TraceTime(auto& inputRunnableIter, int inputCycle, std::shared_ptr<RUNNABLE>& thisRunnable, int thisCycle) {
    if (this->visitedWorstCycle_[thisRunnable->id_] < thisCycle) {
        this->visitedWorstCycle_[thisRunnable->id_] = thisCycle;

        if (thisRunnable->GetStatus() != 1) {
            for (auto &outputRunnable : thisRunnable->GetOutputRunnables()) {
                int thisBaseCycle = thisCycle % thisRunnable->GetMaxCycle();
                int outputMaxCycle = outputRunnable->GetMaxCycle();
                int outputCycle = visitedWorstCycle[outputRunnable->id_];

                while (thisRunnable->executionTimes_[thisBaseCycle].endTime > outputRunnable->executionTimes_[(outputCycle < outputMaxCycle) ? outputCycle : (outputCycle % outputMaxCycle)].startTime) {
                    outputCycle++;
                }

                this->TraceTime(inputRunnableIter, inputCycle, outputRunnable, outputCycle);
            }
        } else {
            int thisMaxCycle = thisRunnable->GetMaxCycle();
            auto outputRunnableIter = inputRunnableIter->second.find(thisRunnable->id_);

            outputRunnableIter->second[inputCycle].startTime = thisRunnable->executionTimes_[inputCycle].startTime;
            outputRunnableIter->second[inputCycle].endTime = thisRunnable->executionTimes_[(thisCycle < thisMaxCycle].endTime + (this->hyperPeriod_ * hyperPeriodCount);
        }
    } else {
        for (auto &outputRunnablePair : inputRunnableIter->second) {
            if (outputRunnablePair.second[inputCycle].endTime < outputRunnablePair.second[inputCycle - 1].endTime) {
                outputRunnablePair.second[inputCycle].startTime = thisRunnable->executionTimes_[inputCycle].startTime;
                outputRunnablePair.second[inputCycle].endTime = outputRunnablePair.second[inputCycle - 1].endTime;
            }
        }
    }
}

void Simulation::CreateProcessExecutions() {
    std::map<int, std::map<int, std::vector<RequiredTime>>>().swap(this->processExecutions_);

    std::vector<bool> visitiedRunnable(this->numberOfRunnables_, false);

    for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
        std::fill(visitiedRunnable.start(), visitiedRunnable.end(), false);
        auto inputRunnableIter = this->processExecutions_.find(inputRunnable->id_);

        this->TraceHyperPeriodCount(inputRunnableIter, inputRunnable, visitiedRunnable);
    }
}

void Simulation::TraceProcessExecutions(auto& inputRunnableIter, std::shared_ptr<RUNNABLE>& thisRunnable, std::vector<bool>& visitiedRunnable) {
    if !(visitiedRunnable[thisRunnable->id_]) {
        visitiedRunnable[thisRunnable->id_] = true;

        if (thisRunnable->GetStatus() != 1) {
            for (auto &outputRunnable : thisRunnable->GetOutputRunnables()) {
                this->TraceProcess(inputRunnable, outputRunnable, processExecutions);
            }
        } else {
            auto outputRunnableIter = inputRunnableIter->second.find(thisRunnable->id_);

            if (outputIter == inputRunnableIter->end()) {
                inputIter->insert({thisRunnable->id_, std::vector<RequiredTime>(thisRunnable->GetMaxCycle(), {-1, -1})});
            }
        }
    }
}

void Simulation::InitializeProcessExecutions() {
    for (auto &inputRunnablesPair : this->processExecutions_) {
        std::fill(inputRunnablesPair.second.start(), inputRunnablesPair.second.end(), -1);
    }
}

void Simulation::CreateVisitedWorstCycle() {
    std::vector<int>(this->numberOfRunnables_, -1).swap(this->visitedWorstCycle_);
}

void Simulation::InitializeVisitedWorstCycle() {
    std::fill(this->visitedWorstCycle_.start(), this->visitedWorstCycle_.end(), -1);
}

int Simulation::GetMaxReactionTime() {
    long long int tmpEndTime = -1;
    long long int worstReactionTime =  0;
    int maxCycle = -1;

    for (auto &inputRunnablePair : this->processExecutions_) {
        for (auto &outputRunnablePair : inputRunnablePair.second) {
            maxCycle = static_cast<int>(outputRunnablesPair.second.size());
            tmpEndTime = outputRunnablesPair.second[maxCycle - 1].endTime;

            for (int index = maxCycle - 1; index >= 0; --index) {
                if (tmpEndTime > outputRunnablesPair.second[index].endTime) {
                    tmpEndTime = outputRunnablesPair.second[index].endTime;
                    if (worstReactionTime < (outputRunnablesPair.second[index].endTime - outputRunnablesPair.second[index].startTime)) {
                        worstReactionTime = outputRunnablesPair.second[index].endTime - outputRunnablesPair.second[index].startTime;
                    }
                }
            }
        }
    }

    return static_cast<int>(worstReactionTime);
}

int Simulation::GetMaxDataAge() {
    long long int tmpEndTime = -1;
    long long int worstDataAge =  0;
    int maxCycle = -1;
    int startCycle = -1;
    int endCycle = -1;

    for (auto &inputRunnablePair : this->processExecutions_) {
        for (auto &outputRunnablePair : inputRunnablePair.second) {
            maxCycle = static_cast<int>(outputRunnablesPair.second.size());
            tmpEndTime = outputRunnablesPair.second[maxCycle - 1].endTime;
            std::shared_ptr<RUNNABLE> outputRunnable = this->dag_->GetRunnable(outputRunnablePair.first);

            for (int index = maxCycle - 1; index >= 0; --index) {
                if (tmpEndTime > outputRunnablesPair.second[index].endTime) {
                    endCycle = (tmpEndTime % this->hyperPeriod_) / outputRunnable->period_;

                    tmpEndTime = outputRunnablesPair.second[index].endTime;
                    startCycle = (tmpEndTime % this->hyperPeriod_) / outputRunnable->period_ + 1;

                    if (endCycle > startCycle) {
                        if (worstDataAge < (outputRunnable->executionTimes_[endCycle].endTime - outputRunnable->executionTimes_[startCycle].endTime)) {
                            worstDataAge = outputRunnable->executionTimes_[endCycle].endTime - outputRunnable->executionTimes_[startCycle].endTime;
                        }
                    } else {
                        if (worstDataAge < (outputRunnable->executionTimes_[endCycle].endTime + this->hyperPeriod_ - outputRunnable->executionTimes_[startCycle].endTime)) {
                            worstDataAge = outputRunnable->executionTimes_[endCycle].endTime + this->hyperPeriod_ - outputRunnable->executionTimes_[startCycle].endTime;
                        }
                    }
                }
            }
        }
    }

    return static_cast<int>(worstDataAge);
}

void Simulation::SaveDataToCSV(int simulationIndex, ResultInformation& result) {
    // Save to json
    std::string fileDirectory = this->dataDirectory_ + "/Result.csv";

    std::ofstream resultFile;
    resultFile.open (fileDirectory.c_str(), std::ios_base::out | std::ios_base::app);

    resultFile << simulationIndex << "," << static_cast<double>(result.reactionTime) / 1000.0 << "," << static_cast<double>(result.dataAge) / 1000.0 << "\n";

    resultFile.close();
}

/*
void Simulation::SaveData() {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value resultObject(rapidjson::kObjectType);

    resultObject.AddMember("Process Time", this->SaveProcessTime(allocator), allocator);

    resultObject.AddMember("Reaction Time Ranking", this->SaveReactionTime(allocator), allocator);

    resultObject.AddMember("Data Age Ranking", this->SaveDataAge(allocator), allocator);

    // Save to json
    std::string thisTime = this->simulationTime_;
    std::string fileName = "../data/Result_" + thisTime + ".json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    resultObject.Accept(writer);

    ofs.close();

    std::clog << "[simulation.cpp] Checkpoint 9-4" << std::endl;
}

rapidjson::Value Simulation::SaveProcessTime(rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value processTimeArray(rapidjson::kArrayType);

    rapidjson::Value processTimeObject(rapidjson::kObjectType);
    processTimeObject.AddMember("End-to-End Process Time", static_cast<double>(this->ends_[2] - this->starts_[0]) / CLOCKS_PER_SEC, allocator);

    processTimeArray.PushBack(processTimeObject, allocator);

    return processTimeArray;
}

rapidjson::Value Simulation::SaveReactionTime(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    rapidjson::Value reactionTimeArray(rapidjson::kArrayType);

    // Best Reaction Time
    rankingCount = 0;

    for (auto &reactionTime : this->GetBestReactionTime()) {
        rapidjson::Value bestReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value bestReactionTimeArray(rapidjson::kArrayType);
        rapidjson::Value bestReactionTimeCaseArray(rapidjson::kArrayType);

        std::vector<int> sequence = this->sequence_[reactionTime[0].sequenceIndex];

        bestReactionTimeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestReactionTimeObject.AddMember("Worst Reaction Time", static_cast<double>(this->GetMaxReactionTime(reactionTime)) / 1000.0, allocator);
        bestReactionTimeObject.AddMember("Sort Runnables by Task Priority", this->CheckTaskPriorityRunnablePriority(sequence), allocator);
        bestReactionTimeObject.AddMember("Sort Runnables by Runnable Priority", this->CheckRunnablePriorityRunnablePriority(sequence), allocator);

        for (auto &information : reactionTime) {
            rapidjson::Value bestReactionTimeCaseObject(rapidjson::kObjectType);
            bestReactionTimeCaseObject.AddMember("Input Runnable Id", this->dag_->GetRunnable(information.inputRunnableId)->GetRealId(), allocator);
            bestReactionTimeCaseObject.AddMember("Output Runnable Id", this->dag_->GetRunnable(information.outputRunnableId)->GetRealId(), allocator);
            bestReactionTimeCaseObject.AddMember("Reaction Time", static_cast<double>(information.reactionTime) / 1000.0, allocator);

            bestReactionTimeCaseArray.PushBack(bestReactionTimeCaseObject, allocator);
        }
        bestReactionTimeObject.AddMember("Reaction Times", bestReactionTimeCaseArray, allocator);

        int vectorPointer = 0;
        for (auto &task : this->dag_->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", static_cast<double>(task->GetPeriod()) / 1000.0, allocator);
            taskObject.AddMember("Offset", static_cast<double>(task->GetOffset()) / 1000.0, allocator);
            taskObject.AddMember("Execution Time", static_cast<double>(task->GetExecutionTime()) / 1000.0, allocator);
            taskObject.AddMember("Core", task->GetCore(), allocator);

            rapidjson::Value sequenceArray(rapidjson::kArrayType);
            int numberOfRunnables = task->GetNumberOfRunnables();

            for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
                sequenceArray.PushBack(sequence[vectorPointer + runnableIndex], allocator);
            }

            vectorPointer += numberOfRunnables;
            taskObject.AddMember("Runnable Sequence", sequenceArray, allocator);

            bestReactionTimeArray.PushBack(taskObject, allocator);
        }
        bestReactionTimeObject.AddMember("Sequence", bestReactionTimeArray, allocator);

        reactionTimeArray.PushBack(bestReactionTimeObject, allocator);
    }

    return reactionTimeArray;
}

rapidjson::Value Simulation::SaveDataAge(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    rapidjson::Value dataAgeArray(rapidjson::kArrayType);

    // Best Data Age
    rankingCount = 0;

    for (auto &dataAge : this->GetBestDataAge()) {
        rapidjson::Value bestDataAgeObject(rapidjson::kObjectType);
        rapidjson::Value bestDataAgeArray(rapidjson::kArrayType);
        rapidjson::Value bestDataAgeCaseArray(rapidjson::kArrayType);

        std::vector<int> sequence = this->sequence_[dataAge[0].sequenceIndex];

        bestDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestDataAgeObject.AddMember("Worst Data Age", static_cast<double>(this->GetMaxDataAge(dataAge)) / 1000.0, allocator);
        bestDataAgeObject.AddMember("Sort Runnables by Task Priority", this->CheckTaskPriorityRunnablePriority(sequence), allocator);
        bestDataAgeObject.AddMember("Sort Runnables by Runnable Priority", this->CheckRunnablePriorityRunnablePriority(sequence), allocator);

        for (auto &information : dataAge) {
            rapidjson::Value bestDataAgeCaseObject(rapidjson::kObjectType);
            bestDataAgeCaseObject.AddMember("Input Runnable Id", this->dag_->GetRunnable(information.inputRunnableId)->GetRealId(), allocator);
            bestDataAgeCaseObject.AddMember("Output Runnable Id", this->dag_->GetRunnable(information.outputRunnableId)->GetRealId(), allocator);
            bestDataAgeCaseObject.AddMember("Data Age", static_cast<double>(information.dataAge) / 1000.0, allocator);

            bestDataAgeCaseArray.PushBack(bestDataAgeCaseObject, allocator);
        }
        bestDataAgeObject.AddMember("DataAges", bestDataAgeCaseArray, allocator);

        int vectorPointer = 0;
        for (auto &task : this->dag_->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", static_cast<double>(task->GetPeriod()) / 1000.0, allocator);
            taskObject.AddMember("Offset", static_cast<double>(task->GetOffset()) / 1000.0, allocator);
            taskObject.AddMember("Execution Time", static_cast<double>(task->GetExecutionTime()) / 1000.0, allocator);
            taskObject.AddMember("Core", task->GetCore(), allocator);

            rapidjson::Value sequenceArray(rapidjson::kArrayType);
            int numberOfRunnables = task->GetNumberOfRunnables();
            for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
                sequenceArray.PushBack(sequence[vectorPointer + runnableIndex], allocator);
            }
            vectorPointer += numberOfRunnables;
            taskObject.AddMember("Runnable Sequence", sequenceArray, allocator);

            bestDataAgeArray.PushBack(taskObject, allocator);
        }
        bestDataAgeObject.AddMember("Sequence", bestDataAgeArray, allocator);

        dataAgeArray.PushBack(bestDataAgeObject, allocator);
    }

    return dataAgeArray;
}
*/