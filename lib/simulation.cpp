#include "simulation.hpp"


void Simulation::Initialize() {
    this->maxCycle_ = this->dag_->GetMaxCycle();
    this->hyperPeriod_ = static_cast<double>(this->dag_->GetHyperPeriod()) / 1000.0;
    this->numberOfTasks_ = this->dag_->GetNumberOfTasks();
    this->numberOfRunnables_ = this->dag_->GetNumberOfRunnables();
    this->numberOfInputRunnables_ = this->dag_->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables_ = this->dag_->GetNumberOfOutputRunnables();
    this->utilization_ = this->dag_->GetUtilization();

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "===========================================================================================================================\n";
    std::cout << " - Max Cycle                  : " << this->maxCycle_ << "\n";
    std::cout << " - Hyper Period               : " << this->hyperPeriod_ << "\n";
    std::cout << " - Number Of Tasks            : " << this->numberOfTasks_ << "\n";
    std::cout << " - Number Of Runanbles        : " << this->numberOfRunnables_ << "\n";
    std::cout << " - Number Of Input Runnables  : " << this->numberOfInputRunnables_ << "\n";
    std::cout << " - Number Of Output Runnables : " << this->numberOfOutputRunnables_ << "\n";
    std::cout << " - Utilization                : " << this->utilization_ << "\n";
    std::cout << "===========================================================================================================================" << std::endl;

    // About File
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    std::string simulationTime =  + std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday);
    this->dataDirectory_ = "../data/" + simulationTime;

    // reserve time vector
    this->starts_.resize(3);
    this->ends_.resize(3);

    // make directory for save datas
    if (mkdir(this->dataDirectory_, 0776) == -1 && errno != EEXIST) {
        std::cerr << strerror(errno) << " directory create error : " << strerror(errno);
        return -1;
    }
}

int Simulation::GetNumberOfCase() {
    int numberOfCase = 1;

    for (auto &task : this->dag_->GetTasks()) {
        task->SortRunnablesByPrecedence();

        int sameCase = 1;
        int currentPrecedence = -1;
        for (auto &runnable : task->GetRunnables()) {
            if (currentPrecedence == runnable->GetPrecedence()) {
                sameCase += 1;
            } else {
                currentPrecedence = runnable->GetPrecedence();
                numberOfCase *= sameCase;
                sameCase = 1;
            }
        }
    }

    return numberOfCase;
}

void Simulation::SetSequenceMatrix() {
    std::vector<std::vector<std::vector<int>>>().swap(this->sequenceMatrix_);

    int taskIndex = 0;
    for (auto &task : this->dag_->GetTasks()) {
        task->SortRunnablesByPrecedence();

        int currentPrecedence = -1;

        int precedenceIndex = 0;
        for (auto &runnable : task->GetRunnables()) {
            if (currentPrecedence == runnable->GetPrecedence()) {
                
            } else {
                this->sequenceMatrix_[taskIndex][precedenceIndex].emplace_back(runnable->id_);
            }

            precedenceIndex += 1;
        }

        taskIndex += 1;
    }
}

void Simulation::Simulate(int communicationMethod) {
    this->starts_[0] = std::clock();
    this->starts_[1] = std::clock();

    this->SetSequenceMatrix();

    int numberOfCase = this->GetNumberOfCase();
    std::vector<bool>(numberOfCase, false).swap(this->visitiedPermutationNumber_);
    
    this->ends_[1] = std::clock();
    this->starts_[2] = std::clock();

    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        int simulationSeed = static_cast<int>();

        this->SetSequence(numberOfCase, runnableExecutionPermutation);

        std::vector<std::vector<RequiredTime>> runnableTimeTable; // [ID][Case][Time]

        std::vector<ResultInformation> result = this->GetResult(caseIndex, runnableExecutionPermutation, runnableExecutions, runnableCommunicationPermutation, runnableCommunications);
        this->results_.emplace_back(result);
		
		std::cout << "\033[H\033[2J\033[3J";
		std::cout << "===========================================================================================================================\n";
        std::cout << " - Simulation Case            : " << std::setw(10) << (caseIndex + 1) << "/" << std::setw(10) << numberOfCase << "\n";
        std::cout << " - Reaction Time              : " << std::setw(10) << static_cast<double>(this->GetMaxReactionTime(result)) / 1000.0 << "\n";
        std::cout << " - Data Age                   : " << std::setw(10) << static_cast<double>(this->GetMaxDataAge(result)) / 1000.0 << "\n";
        std::cout << " - Max Cycle                  : " << this->maxCycle_ << "\n";
        std::cout << " - Hyper Period               : " << this->hyperPeriod_ << "\n";
        std::cout << " - Number Of Tasks            : " << this->numberOfTasks_ << "\n";
        std::cout << " - Number Of Runanbles        : " << this->numberOfRunnables_ << "\n";
        std::cout << " - Number Of Input Runnables  : " << this->numberOfInputRunnables_ << "\n";
        std::cout << " - Number Of Output Runnables : " << this->numberOfOutputRunnables_ << "\n";
        std::cout << " - Utilization                : " << this->utilization_ << "\n";
		std::cout << "===========================================================================================================================" << std::endl;
    }

    this->ends_[2] = std::clock();
}

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

bool Simulation::CheckRunnablePriorityRunnablePriority(std::vector<int>& currentSequence) {
    std::clog << "=================================[Debug : Output Runnable's Priority -> Runnable Priority]=================================" << "\n";

    for (int index = 0; index < this->numberOfRunnables_; index++) {
        this->dag_->GetRunnable(currentSequence[index])->SetPriorityInTask(index);
    }

    int prePrecedence = -1;
    int preTaskPriority = -1;
    int preOutputTaskPriority = -1;
    int preOutputRunnablePriority = -1;
    int currentPrecedence = -1;
    int currentTaskPriority = -1;
    int currentOutputTaskPriority = -1;
    int currentOutputRunnablePriority = -1;

    for (auto &runnable : currentSequence) {
        std::shared_ptr<RUNNABLE> currentRunnable = this->dag_->GetRunnable(runnable);
        currentPrecedence = currentRunnable->GetPrecedence();
        currentTaskPriority = currentRunnable->GetTask()->GetPriority();
        currentOutputTaskPriority = this->GetMaxOutputTaskPriority(currentRunnable);
        currentOutputRunnablePriority = this->GetMaxOutputRunnablePriority(currentRunnable);

        if (currentTaskPriority == 0) continue;

        if (preTaskPriority > currentTaskPriority) {
            return false;

        } else if (preTaskPriority < currentTaskPriority) {
            prePrecedence = currentPrecedence;
            preTaskPriority = currentTaskPriority;
            preOutputTaskPriority = currentOutputTaskPriority;
            preOutputRunnablePriority = currentOutputRunnablePriority;

        } else {
            if (prePrecedence > currentPrecedence) {
                return false;

            } else if (prePrecedence < currentPrecedence) {
                prePrecedence = currentPrecedence;
                preTaskPriority = currentTaskPriority;
                preOutputTaskPriority = currentOutputTaskPriority;
                preOutputRunnablePriority = currentOutputRunnablePriority;

            } else {
                if (preOutputTaskPriority > currentOutputTaskPriority) { //
                    return false;

                } else if (preOutputTaskPriority > currentOutputTaskPriority) {
                    prePrecedence = currentPrecedence;
                    preTaskPriority = currentTaskPriority;
                    preOutputTaskPriority = currentOutputTaskPriority;
                    preOutputRunnablePriority = currentOutputRunnablePriority;

                } else {
                    if (preOutputRunnablePriority > currentOutputRunnablePriority) {
                        return false;

                    } else {
                        prePrecedence = currentPrecedence;
                        preTaskPriority = currentTaskPriority;
                        preOutputTaskPriority = currentOutputTaskPriority;
                        preOutputRunnablePriority = currentOutputRunnablePriority;
                    }
                }
            }
        }
    }

    return true;
}

std::vector<ResultInformation> Simulation::GetResult(int caseIndex,
                                                     std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                                                     std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                                                     std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                                                     std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications) {
    std::clog << "=================================[Debug : Output Runnable's Priority -> Runnable Priority]=================================" << std::endl;
    int executionCaseIndex = caseIndex;
    std::vector<int> executionPermutationPointer(this->numberOfRunnables_);
    for (auto &executionPermutation : runnableExecutionPermutation) {
        int permutationPointerNumber = executionCaseIndex % static_cast<int>(executionPermutation.size());
        executionCaseIndex /= static_cast<int>(executionPermutation.size());
        
        for (auto &runnableId : executionPermutation[permutationPointerNumber]) {
            executionPermutationPointer[runnableId] = permutationPointerNumber;
        }
    }

    int comunicationCaseIndex = caseIndex;
    std::vector<int> communicationPermutationPointer(this->numberOfRunnables_);
    for (auto &communicationPermutation : runnableCommunicationPermutation) {
        int permutationPointerNumber = comunicationCaseIndex % static_cast<int>(communicationPermutation.size());
        comunicationCaseIndex /= static_cast<int>(communicationPermutation.size());
        
        for (auto &runnableId : communicationPermutation[permutationPointerNumber]) {
            communicationPermutationPointer[runnableId] = permutationPointerNumber;
        }
    }

    std::map<std::pair<int, int>, std::vector<RequiredTime>> processExecutions;
    this->SetProcessExecutions(executionPermutationPointer, runnableExecutions, communicationPermutationPointer, runnableCommunications, processExecutions);

    std::vector<ResultInformation> result;

    for (auto &processExecution : processExecutions) {
        result.push_back({processExecution.first.first, processExecution.first.second, caseIndex, this->GetReactionTime(processExecution), this->GetDataAge(executionPermutationPointer, runnableExecutions, processExecution)});
        //result.push_back({processExecution.first.first, processExecution.first.second, caseIndex, this->GetReactionTime(processExecution), 0});
    }
    std::clog << "ckpt 6" << std::endl;

    return result;
}

void Simulation::SetProcessExecutions(std::vector<int>& executionPermutationPointer,
                                      std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                                      std::vector<int>& communicationPermutationPointer,
                                      std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications,
                                      std::map<std::pair<int, int>, std::vector<RequiredTime>>& processExecutions) {
    for (auto &runnable : this->dag_->GetInputRunnables()) {
        int eachMaxCycle = static_cast<int>(this->hyperPeriod_ / runnable->GetTask()->GetPeriod());
        std::vector<int> worstCyclePerRunnable(this->numberOfRunnables_, -1);

        for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
            this->TraceProcess(executionPermutationPointer,
                                runnableExecutions,
                                communicationPermutationPointer,
                                runnableCommunications,
                                runnable->GetId(), // input Runnable's ID
                                cycle,             // input Runnable's cycle
                                runnable->GetId(), // current Runnable's ID
                                cycle,             // current Runnable's cycle
                                0,
                                worstCyclePerRunnable,
                                processExecutions);
        }
    }
}

void Simulation::TraceProcess(std::vector<int>& executionPermutationPointer,
                              std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                              std::vector<int>& communicationPermutationPointer,
                              std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications,
                              int inputRunnableId,
                              int inputCycle,
                              int thisRunnableId,
                              int thisCycle,
                              int thisHyperPeriodCount,
                              std::vector<int>& worstCyclePerRunnable,
                              std::map<std::pair<int, int>, std::vector<RequiredTime>>& processExecutions) {
    if (this->dag_->GetRunnable(thisRunnableId)->GetStatus() == 1) {  //precedence처럼 해보셈
        auto iter = processExecutions.find(std::make_pair(inputRunnableId, thisRunnableId));

        if (iter == processExecutions.end()) {
            std::vector<RequiredTime> tmpVector = { {runnableExecutions[inputRunnableId][executionPermutationPointer[inputRunnableId]][inputCycle].startTime,
                                                             runnableExecutions[thisRunnableId][executionPermutationPointer[thisRunnableId]][thisCycle].endTime + static_cast<long long int>(thisHyperPeriodCount) * this->hyperPeriod_} };
            processExecutions.insert(std::make_pair(std::make_pair(inputRunnableId, thisRunnableId), tmpVector));
        } else {
            RequiredTime tmpInfo = {runnableExecutions[inputRunnableId][executionPermutationPointer[inputRunnableId]][inputCycle].startTime,
                                            runnableExecutions[thisRunnableId][executionPermutationPointer[thisRunnableId]][thisCycle].endTime + static_cast<long long int>(thisHyperPeriodCount) * this->hyperPeriod_};

            if (static_cast<int>((*iter).second.size()) == (inputCycle + 1)) {
                if ((*iter).second.back().endTime < tmpInfo.endTime) {
                    (*iter).second.back().endTime = tmpInfo.endTime;
                }
            } else {
                (*iter).second.emplace_back(tmpInfo);
            }
        }
    } else {
        if (worstCyclePerRunnable[thisRunnableId] < thisCycle) {
            worstCyclePerRunnable[thisRunnableId] = thisCycle;
            
            for (auto &outputRunnable : this->dag_->GetRunnable(thisRunnableId)->GetOutputRunnables()) {
                int outputRunnableId = outputRunnable->GetId();

                int tmpCycle = 0;
                int maxCycle = static_cast<int>(runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]].size());
                int hyperPeriodCount = 0;
                long long int thisRunnableWriteTime = runnableCommunications[thisRunnableId][communicationPermutationPointer[thisRunnableId]][thisCycle].endTime;
                long long int outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][tmpCycle].startTime;

                while (thisRunnableWriteTime > outputRunnableReadTime) {
                    tmpCycle++;

                    if (tmpCycle == maxCycle) {
                        tmpCycle = 0;
                        hyperPeriodCount++;

                        outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][tmpCycle].startTime + (hyperPeriodCount * this->hyperPeriod_);
                    } else {
                        outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][tmpCycle].startTime + (hyperPeriodCount * this->hyperPeriod_);
                    }
                }

                this->TraceProcess(executionPermutationPointer, runnableExecutions, communicationPermutationPointer, runnableCommunications, inputRunnableId, inputCycle, outputRunnableId, tmpCycle, (thisHyperPeriodCount + hyperPeriodCount), worstCyclePerRunnable, processExecutions);
            }
        } else {
            auto iter = std::find_if(processExecutions.begin(), processExecutions.end(), [inputRunnableId](std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& a) { return a.first.first == inputRunnableId; });

            while (iter != processExecutions.end()) {
                if (static_cast<int>((*iter).second.size()) == inputCycle) {
                    (*iter).second.push_back( { runnableExecutions[inputRunnableId][executionPermutationPointer[inputRunnableId]][inputCycle].startTime, (*iter).second.back().endTime } );
                }

                iter = std::find_if(++iter, processExecutions.end(), [inputRunnableId](std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& a) { return a.first.first == inputRunnableId; });
            }
        }
    }
}

int Simulation::GetReactionTime(std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& processExecution) {
    int WorstReactionTime =  0;

    for (auto &StartToEndTime : processExecution.second) {
        int tmpThisReactionTime = static_cast<int>(StartToEndTime.endTime - StartToEndTime.startTime);

        if (WorstReactionTime < tmpThisReactionTime) {
            WorstReactionTime = tmpThisReactionTime;
        }
    }

    return WorstReactionTime;
}

int Simulation::GetDataAge(std::vector<int>& executionPermutationPointer,
                           std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                           std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& processExecution) {
    int WorstDataAge = 0;
    int pointer = 0;
    long long int preEndTime = processExecution.second[0].endTime;
    long long int currentEndTime = processExecution.second[0].endTime;

    int hyperPeriodCount = 0;
    int maxCycle = static_cast<int>(runnableExecutions[processExecution.first.second][executionPermutationPointer[processExecution.first.second]].size());

    while (preEndTime != (runnableExecutions[processExecution.first.second][executionPermutationPointer[processExecution.first.second]][pointer].endTime + (hyperPeriodCount * this->hyperPeriod_))) {
        pointer++;

        if (pointer >= maxCycle) {
            pointer -= maxCycle;
            hyperPeriodCount++;
        }
    }

    for (auto &StartToEndTime : processExecution.second) {
        currentEndTime = StartToEndTime.endTime;

        if (currentEndTime != preEndTime) {
            long long int tmpEndTime = runnableExecutions[processExecution.first.second][executionPermutationPointer[processExecution.first.second]][pointer].endTime + static_cast<long long int>(hyperPeriodCount) * this->hyperPeriod_;

            while (currentEndTime > (runnableExecutions[processExecution.first.second][executionPermutationPointer[processExecution.first.second]][pointer].endTime + static_cast<long long int>(hyperPeriodCount) * this->hyperPeriod_)) {
                tmpEndTime = runnableExecutions[processExecution.first.second][executionPermutationPointer[processExecution.first.second]][pointer].endTime + static_cast<long long int>(hyperPeriodCount) * this->hyperPeriod_;

                pointer++;

                if (pointer >= maxCycle) {
                    pointer -= maxCycle;
                    hyperPeriodCount++;
                }
            }

            int dataAge = static_cast<int>(tmpEndTime - preEndTime);
            if (WorstDataAge < dataAge) {
                WorstDataAge = dataAge;
            }
        }

        preEndTime = currentEndTime;
    }

    return WorstDataAge;
}

std::vector<std::vector<ResultInformation>>& Simulation::GetBestReactionTime() {
    std::sort(this->results_.begin(), this->results_.end(), [&, this](std::vector<ResultInformation>& a, std::vector<ResultInformation>& b) { return this->GetMaxReactionTime(a) < this->GetMaxReactionTime(b); });

    return this->results_;
}

std::vector<std::vector<ResultInformation>>& Simulation::GetBestDataAge() {
    std::sort(this->results_.begin(), this->results_.end(), [&, this](std::vector<ResultInformation>& a, std::vector<ResultInformation>& b) { return this->GetMaxDataAge(a) < this->GetMaxDataAge(b); });

    return this->results_;
}

const int Simulation::GetMaxReactionTime(std::vector<ResultInformation>& a) {
    int maxReactionTime = 0;

    for (auto &resultInformation : a) {
        if (maxReactionTime < resultInformation.reactionTime) {
            maxReactionTime = resultInformation.reactionTime;
        }
    }

    return maxReactionTime;
}

const int Simulation::GetMaxDataAge(std::vector<ResultInformation>& a) {
    int maxDataAge = 0;

    for (auto &resultInformation : a) {
        if (maxDataAge < resultInformation.dataAge) {
            maxDataAge = resultInformation.dataAge;
        }
    }

    return maxDataAge;
}

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

void Simulation::SaveDag() {
    this->dag_->SaveDag(this->simulationTime_);
}

void Simulation::SaveMapping() {
    this->dag_->SaveMapping(this->simulationTime_);
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

void Simulation::SaveDataToCSV() {
    // Save to json
    std::string thisTime = this->simulationTime_;
    std::string allCaseFileName = "../data/Result_AllCase_" + thisTime + ".csv";
    std::string taskPriorityFileName = "../data/Result_TaskPriority_" + thisTime + ".csv";
    std::string runnablePriorityFileName = "../data/Result_RunnablePriority_" + thisTime + ".csv";

    std::ofstream allCase;
    allCase.open (allCaseFileName.c_str());

    std::ofstream taskPriority;
    taskPriority.open (taskPriorityFileName.c_str());

    std::ofstream runnablePriority;
    runnablePriority.open (runnablePriorityFileName.c_str());
    int count_all = 0;
    int count_task = 0;
    int count_runnable = 0;

    for (auto &reactionTime : this->GetBestReactionTime()) {
        std::vector<int> sequence = this->sequence_[reactionTime[0].sequenceIndex];
        int maxReactionTime = this->GetMaxReactionTime(reactionTime);
        int maxDataAge = this->GetMaxDataAge(reactionTime);

        allCase << count_all++ << "," << static_cast<double>(maxReactionTime) / 1000.0 << "," << static_cast<double>(maxDataAge) / 1000.0 << "\n";

        if (this->CheckTaskPriorityRunnablePriority(sequence)) {
            taskPriority << count_task++ << "," <<static_cast<double>(maxReactionTime) / 1000.0 << "," << static_cast<double>(maxDataAge) / 1000.0 << "\n";
        }

        if (this->CheckRunnablePriorityRunnablePriority(sequence)) {
            runnablePriority << count_runnable++ << "," << static_cast<double>(maxReactionTime) / 1000.0 << "," << static_cast<double>(maxDataAge) / 1000.0 << "\n";
        }
    }

    runnablePriority.close();
    taskPriority.close();
    allCase.close();
}

void Simulation::SaveDataToCSV() {
    if(mkdir("../data/"))
}