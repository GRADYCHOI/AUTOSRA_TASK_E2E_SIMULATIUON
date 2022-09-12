#include "simulation.hpp"


void Simulation::Initialize() {
    this->maxCycle_ = this->dag_->GetMaxCycle();
    this->hyperPeriod_ = this->dag_->GetHyperPeriod();
    this->numberOfTasks_ = this->dag_->GetNumberOfTasks();
    this->numberOfRunnables_ = this->dag_->GetNumberOfRunnables();
    this->numberOfInputRunnables_ = this->dag_->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables_ = this->dag_->GetNumberOfOutputRunnables();

    //std::system("clear");
    std::cout << "===========================================================================================================================\n";
    std::cout << " - Max Cycle                  : " << this->maxCycle_ << "\n";
    std::cout << " - Hyper Period               : " << static_cast<double>(this->hyperPeriod_) / 1000.0 << "\n";
    std::cout << " - Number Of Tasks            : " << this->numberOfTasks_ << "\n";
    std::cout << " - Number Of Runanbles        : " << this->numberOfRunnables_ << "\n";
    std::cout << " - Number Of Input Runnables  : " << this->numberOfInputRunnables_ << "\n";
    std::cout << " - Number Of Output Runnables : " << this->numberOfOutputRunnables_ << "\n";
    std::cout << " - Utilization                : " << this->dag_->GetUtilization() << "\n";
    std::cout << "===========================================================================================================================" << std::endl;

    // About File
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    this->simulationTime_ = std::to_string(pTimeInfo->tm_year + 1900) + "_" + std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday) + "_" + std::to_string(pTimeInfo->tm_hour) + "_" + std::to_string(pTimeInfo->tm_min);
}

void Simulation::GetRunnableScheduleInformations(int communicationMethod,
                                                 std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                                                 std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                                                 std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                                                 std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    std::cout << "[Simulation] Get Runnable Execution Times" << std::endl;
    this->GetRunnableExecutions(runnableExecutionPermutation, runnableExecutions);
    
    switch (communicationMethod) {
        case RunnableImplicitMethod:
            runnableCommunicationPermutation = runnableExecutionPermutation;
            runnableCommunications = runnableExecutions;
            break;

        case TaskImplicitMethod:
            this->SetCommunication(std::unique_ptr<Communication>(new TaskImplicit()));

            std::cout << "[Simulation] Get Runnable Communication Times" << std::endl;
            this->GetRunnableCommunications(runnableCommunicationPermutation, runnableCommunications);
            break;

        case LETMethod:
            this->SetCommunication(std::unique_ptr<Communication>(new LET()));

            std::cout << "[Simulation] Get Runnable Communication Times" << std::endl;
            this->GetRunnableCommunications(runnableCommunicationPermutation, runnableCommunications);
            break;
    }
}

void Simulation::Simulate(int communicationMethod) {
    std::vector<std::vector<std::vector<int>>> runnableExecutionPermutation;  // [Priority][Case][ID]
    std::vector<std::vector<std::vector<int>>> runnableCommunicationPermutation;  // [Priority][Case][ID]
    std::vector<std::vector<std::vector<ExecutionInformation>>> runnableExecutions; // [ID][Case][Time]
    std::vector<std::vector<std::vector<ExecutionInformation>>> runnableCommunications; // [ID][Case][Time]

    this->GetRunnableScheduleInformations(communicationMethod, runnableExecutionPermutation, runnableCommunicationPermutation, runnableExecutions, runnableCommunications);

    int numberOfCase = 1;
    for (auto &schedulingPriority : runnableExecutionPermutation) {
        numberOfCase *= static_cast<int>(schedulingPriority.size()); // number of case
    }

    this->results_.reserve(numberOfCase);
    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        ResultInformation result = this->GetResult(caseIndex, runnableExecutionPermutation, runnableExecutions, runnableCommunicationPermutation, runnableCommunications);
        this->results_.push_back(result);
		
		//std::system("clear");
		std::cout << "===========================================================================================================================\n";
        std::cout << " - Simulation Case            : " << std::setw(10) << caseIndex << "/" << std::setw(10) << numberOfCase << "\n";
        std::cout << " - Reaction Time              : " << std::setw(10) << static_cast<double>(result.reactionTime) / 1000.0 << "\n";
        std::cout << " - Data Age                   : " << std::setw(10) << static_cast<double>(result.dataAge) / 1000.0 << "\n";
        std::cout << " - Max Cycle                  : " << this->maxCycle_ << "\n";
        std::cout << " - Hyper Period               : " << static_cast<double>(this->hyperPeriod_) / 1000.0 << "\n";
        std::cout << " - Number Of Tasks            : " << this->numberOfTasks_ << "\n";
        std::cout << " - Number Of Runanbles        : " << this->numberOfRunnables_ << "\n";
        std::cout << " - Number Of Input Runnables  : " << this->numberOfInputRunnables_ << "\n";
        std::cout << " - Number Of Output Runnables : " << this->numberOfOutputRunnables_ << "\n";
        std::cout << " - Utilization                : " << this->dag_->GetUtilization() << "\n";
		std::cout << "===========================================================================================================================" << std::endl;
    }

    this->SetSequence(numberOfCase, runnableExecutionPermutation);
}

void Simulation::SetSequence(int numberOfCase,
                             std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation) {
    this->sequence_.reserve(numberOfCase);

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

ResultInformation Simulation::GetResult(int caseIndex,
                           std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                           std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                           std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                           std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
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

    std::map<std::pair<int, int>, std::vector<ExecutionInformation>> processExecutions;
    this->SetProcessExecutions(executionPermutationPointer, runnableExecutions, communicationPermutationPointer, runnableCommunications, processExecutions);

    ResultInformation result = {caseIndex, this->GetReactionTime(processExecutions), this->GetDataAge(executionPermutationPointer, runnableExecutions, processExecutions)};

    return result;
}

void Simulation::SetProcessExecutions(std::vector<int>& executionPermutationPointer,
                                      std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                                      std::vector<int>& communicationPermutationPointer,
                                      std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications,
                                      std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions) {
    for (auto &task : this->dag_->GetTasks()) {
        for (auto &runnable : task->GetRunnables()) {
            int eachMaxCycle = static_cast<int>(this->hyperPeriod_ / task->GetPeriod());

            for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
                std::vector<int> worstCyclePerRunnable(this->numberOfRunnables_, -1);
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
}

void Simulation::TraceProcess(std::vector<int> executionPermutationPointer,
                              std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                              std::vector<int> communicationPermutationPointer,
                              std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications,
                              int inputRunnableId,
                              int inputCycle,
                              int thisRunnableId,
                              int thisCycle,
                              int thisHyperPeriodCount,
                              std::vector<int>& worstCyclePerRunnable,
                              std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions) {
    // --------------------------------------------------------------------------------------------------------------
    // runnableExecutions : [maxCycle X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionInformation (startTime, endTime)
    // 2 : Second Cycle's ExecutionInformation (startTime, endTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // runnableCommunications : [maxCycle X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's CommunicationInformation (readTime, writeTime)
    // 2 : Second Cycle's CommunicationInformation (readTime, writeTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // processExecutions : [maxCycle X (InputRunnable, OutputRunnable) pair]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionTimeInformation (startTime, writeTime)
    // 2 : Second Cycle's ExecutionTimeInformation (startTime, writeTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------

    if (this->dag_->GetRunnable(thisRunnableId)->GetStatus() == 1) {  //precedence처럼 해보셈
        auto iter = processExecutions.find(std::make_pair(inputRunnableId, thisRunnableId));

        if (iter == processExecutions.end()) {
            std::vector<ExecutionInformation> tmpVector = { {runnableExecutions[inputRunnableId][executionPermutationPointer[inputRunnableId]][inputCycle].startTime,
                                                             runnableExecutions[thisRunnableId][executionPermutationPointer[thisRunnableId]][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod_} };
            processExecutions.insert(std::make_pair(std::make_pair(inputRunnableId, thisRunnableId), tmpVector));
        } else {
            ExecutionInformation tmpInfo = {runnableExecutions[inputRunnableId][executionPermutationPointer[inputRunnableId]][inputCycle].startTime,
                                            runnableExecutions[thisRunnableId][executionPermutationPointer[thisRunnableId]][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod_};

            if (static_cast<int>((*iter).second.size()) == inputCycle) {
                if ((*iter).second.back().endTime > tmpInfo.endTime) {
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
                int thisRunnableWriteTime = runnableCommunications[thisRunnableId][communicationPermutationPointer[thisRunnableId]][thisCycle].endTime;
                int outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][tmpCycle].startTime;

                while (thisRunnableWriteTime > outputRunnableReadTime) {
                    tmpCycle++;

                    if (tmpCycle == maxCycle) {
                        outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][0].startTime + this->hyperPeriod_;

                        tmpCycle = 0;
                        hyperPeriodCount++;
                    } else {
                        outputRunnableReadTime = runnableCommunications[outputRunnableId][communicationPermutationPointer[outputRunnableId]][tmpCycle].endTime;
                    }
                }

                this->TraceProcess(executionPermutationPointer, runnableExecutions, communicationPermutationPointer, runnableCommunications, inputRunnableId, inputCycle, outputRunnableId, tmpCycle, (thisHyperPeriodCount + hyperPeriodCount), worstCyclePerRunnable, processExecutions);
            }
        }
    }
}

int Simulation::GetReactionTime(std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions) {
    int WorstReactionTime =  0;

    for (auto &InputToOutputRunnable : processExecutions) {
        for (auto &StartToEndTime : InputToOutputRunnable.second) {
            int tmpThisReactionTime = StartToEndTime.endTime - StartToEndTime.startTime;
            if (WorstReactionTime < tmpThisReactionTime) {
                WorstReactionTime = tmpThisReactionTime;
            }
        }
    }

    return WorstReactionTime;
}

int Simulation::GetDataAge(std::vector<int> executionPermutationPointer,
                              std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                              std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions) {
    int WorstDataAge = 0;

    for (auto &ouputRunnable : this->dag_->GetOutputRunnables()) {
        int runnableId = ouputRunnable->GetId();
        int biggestEndTime = 0; 

        for (auto &StoEs : processExecutions) {
            if (StoEs.first.second == runnableId) {
                for (auto &StoE : StoEs.second) {
                    if (StoE.endTime > biggestEndTime) {
                        biggestEndTime = StoE.endTime;
                    }
                }
            }
        }

        int hyperPeriodCount = std::ceil(biggestEndTime/this->hyperPeriod_);

        // Reacted time
        std::vector<int> endTimes;
        endTimes.reserve(hyperPeriodCount * this->maxCycle_);
        for (auto &StoEs : processExecutions) {
            if (StoEs.first.second == runnableId) {
                for (auto &StoE : StoEs.second) {
                    int tmpEndTime = StoE.endTime;
                    while (tmpEndTime <= biggestEndTime) {
                        endTimes.emplace_back(tmpEndTime);
                        tmpEndTime += this->hyperPeriod_;
                    }
                }
            }
        }

        // just output time
        std::vector<int> endTimeTable;
        endTimeTable.reserve(hyperPeriodCount * this->maxCycle_);
        for (int count = 0; count < hyperPeriodCount; count++) {
            for (auto &executionTime : runnableExecutions[runnableId]) {
                endTimeTable.emplace_back(executionTime[executionPermutationPointer[runnableId]].endTime + this->hyperPeriod_ * count);
            }
        }

        std::vector<int> endTimeIndex; 
        endTimeIndex.reserve(hyperPeriodCount * this->maxCycle_);
        for (auto &endTime : endTimes) {
            endTimeIndex.push_back(std::distance(endTimeTable.begin(), std::find(endTimeTable.begin(), endTimeTable.end(), endTime)));
        }

        std::sort(endTimeIndex.begin(), endTimeIndex.end(), [](int a, int b) { return a < b; });

        int preindex = endTimeIndex[0];
        for (auto &index : endTimeIndex) {
            if (index > preindex + 1)  {
                if (WorstDataAge < endTimeTable[index] - endTimeTable[preindex]) {
                    WorstDataAge = endTimeTable[index] - endTimeTable[preindex];
                }
            }
            preindex = index;
        }
    }

    return WorstDataAge;
}

std::vector<ResultInformation>& Simulation::GetBestReactionTime() {
    std::sort(this->results_.begin(), this->results_.end(), [](ResultInformation& a, ResultInformation& b) { return a.reactionTime < b.reactionTime; });

    return this->results_;
}

std::vector<ResultInformation>& Simulation::GetBestDataAge() {
    std::sort(this->results_.begin(), this->results_.end(), [](ResultInformation& a, ResultInformation& b) { return a.dataAge < b.dataAge; });

    return this->results_;
}

void Simulation::SaveData() {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value resultObject(rapidjson::kObjectType);

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

rapidjson::Value Simulation::SaveReactionTime(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    rapidjson::Value reactionTimeArray(rapidjson::kArrayType);

    // Best Reaction Time
    rankingCount = 0;

    for (auto &reactionTime : this->GetBestReactionTime()) {
        rapidjson::Value bestReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value bestReactionTimeArray(rapidjson::kArrayType);

        bestReactionTimeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestReactionTimeObject.AddMember("Reaction Time", static_cast<double>(reactionTime.reactionTime) / 1000.0, allocator);

        std::vector<int> sequence = this->sequence_[reactionTime.sequenceIndex];

        int vectorPointer = 0;
        for (auto &task : this->dag_->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", static_cast<double>(task->GetPeriod()) / 1000.0, allocator);
            taskObject.AddMember("Offset", static_cast<double>(task->GetOffset()) / 1000.0, allocator);
            taskObject.AddMember("Execution Time", static_cast<double>(task->GetExecutionTime()) / 1000.0, allocator);

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

        bestDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestDataAgeObject.AddMember("Data Age", static_cast<double>(dataAge.dataAge) / 1000.0, allocator);

        std::vector<int> sequence = this->sequence_[dataAge.sequenceIndex];
        int vectorPointer = 0;
        for (auto &task : this->dag_->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", static_cast<double>(task->GetPeriod()) / 1000.0, allocator);
            taskObject.AddMember("Offset", static_cast<double>(task->GetOffset()) / 1000.0, allocator);

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