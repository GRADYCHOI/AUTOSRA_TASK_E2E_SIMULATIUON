#include "simulation.hpp"


double GCD(double a, double b) {
    int tmp1 = static_cast<int>(a * 1000);
    int tmp2 = static_cast<int>(b * 1000);

    while (tmp2 != 0) {
    	int tmp3 = tmp1 % tmp2;
        tmp1 = tmp2;
        tmp2 = tmp3;
    }

    return static_cast<double>(tmp1/1000);
}

void Simulation::Initialize() {
    std::clog << "[simulation.cpp] CheckPoint 0" << std::endl;
    this->maxCycle = this->dag->GetMaxCycle();
    this->hyperPeriod = this->dag->GetHyperPeriod();
    this->numberOfTasks = this->dag->GetNumberOfTasks();
    this->numberOfRunnables = this->dag->GetNumberOfRunnables();
    this->numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    std::clog << "[simulation.cpp] CheckPoint 0-1" << std::endl;
    this->ClearTables();

    std::clog << "[simulation.cpp] CheckPoint 0-2" << std::endl;

    // About File
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    this->simulationTime = std::to_string(pTimeInfo->tm_year + 1900) + "_" + std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday) + "_" + std::to_string(pTimeInfo->tm_hour) + "_" + std::to_string(pTimeInfo->tm_min);
}

void Simulation::ClearTables() {
    std::clog << "[simulation.cpp] CheckPoint 2-1" << std::endl;
    RunnableInformation initialRunnableInformation = {-1, -1, -1, -1.0, -1.0, -1.0};
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};

    std::clog << "[simulation.cpp] CheckPoint 2-2" << std::endl;
    std::vector<RunnableInformation>(this->numberOfRunnables, initialRunnableInformation).swap(this->runnableInformations);

    std::clog << "[simulation.cpp] CheckPoint 2-3" << std::endl;
    std::vector<std::vector<ExecutionInformation>>(this->numberOfRunnables, std::vector<ExecutionInformation>(this->maxCycle, initialExecutionInformation)).swap(this->runnableExecutions);

    std::clog << "[simulation.cpp] CheckPoint 2-4" << std::endl;
    std::vector<std::vector<ExecutionInformation>>(this->numberOfRunnables, std::vector<ExecutionInformation>(this->maxCycle, initialExecutionInformation)).swap(this->runnableCommunications);

    std::clog << "[simulation.cpp] CheckPoint 2-5" << std::endl;
}

void Simulation::Simulate() {
    int numberOfCase = this->dag->GetNumberOfSequenceCase();

    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        std::clog << "[simulation.cpp] CheckPoint 1" << std::endl;
        this->dag->SetRunnablePriority(caseIndex);

        std::clog << "[simulation.cpp] CheckPoint 2" << std::endl;
        this->ClearTables();

        std::clog << "[simulation.cpp] CheckPoint 3" << std::endl;
        this->SetRunnableInformations();

        std::clog << "[simulation.cpp] CheckPoint 4" << std::endl;
        this->SetRunnableExecutions();

        std::clog << "[simulation.cpp] CheckPoint 5" << std::endl;
        this->SetRunnableCommunications();
        
        std::clog << "[simulation.cpp] CheckPoint 6" << std::endl;
        this->SetProcessExecutions();

        std::clog << "[simulation.cpp] CheckPoint 7" << std::endl;
        this->SetResult();

        std::clog << "[simulation.cpp] CheckPoint 8" << std::endl;
    }
}

void Simulation::SetResult() {
    ResultInformation result = {this->dag->GetCurrentSequenceIndex(), this->GetReactionTime(), this->GetDataAge()};
    this->results.push_back(result);
}

void Simulation::SetRunnableInformations() {
    // --------------------------------------------------------------------------------------------------------------
    // runnableInformations : [1 X numberOfRunnables]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : RunnableInformation (taskId, priority, period, offset, executionTime)
    // --------------------------------------------------------------------------------------------------------------
    
    std::vector<int> runnablePriority = this->dag->GetRunnablePriority();
    
    for (auto &task : this->dag->GetTasks()) {
        for (auto &runnable : task->GetRunnables()) {
            std::clog << "[simulation.cpp] CheckPoint 3-1" << std::endl;
            int runnableId = runnable->GetId();
            std::clog << "[simulation.cpp] CheckPoint 3-2" << std::endl;
            this->runnableInformations[runnableId].taskId = task->GetId();
            std::clog << "[simulation.cpp] CheckPoint 3-3" << std::endl;
            this->runnableInformations[runnableId].priority = runnablePriority[runnableId];
            std::clog << "[simulation.cpp] CheckPoint 3-4" << std::endl;
            this->runnableInformations[runnableId].status = runnable->GetStatus();
            std::clog << "[simulation.cpp] CheckPoint 3-5" << std::endl;
            this->runnableInformations[runnableId].period = task->GetPeriod();
            std::clog << "[simulation.cpp] CheckPoint 3-6" << std::endl;
            this->runnableInformations[runnableId].offset = task->GetOffset();
            std::clog << "[simulation.cpp] CheckPoint 3-7" << std::endl;
            this->runnableInformations[runnableId].executionTime = runnable->GetExecutionTime();
            std::clog << "[simulation.cpp] CheckPoint 3-7" << std::endl;
        }
    }
}

void Simulation::SetRunnableExecutions() {
    // --------------------------------------------------------------------------------------------------------------
    // runnableInformations : [1 X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : RunnableInformation (taskId, priority, period, offset, executionTime)
    // --------------------------------------------------------------------------------------------------------------
    // runnableExecutions : [maxCycle X numberOfRunnables]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionInformation (startTime, endTime)
    // 2 : Second Cycle's ExecutionInformation (startTime, endTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------

    double unit = this->runnableInformations[0].period;
    for (auto &task : this->dag->GetTasks()) {
        unit = GCD(unit, ((task->GetOffset() != 0.0) ? GCD(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::clog << "[simulation.cpp] CheckPoint 4-1" << std::endl;
    std::vector<double> emptyTimes((static_cast<int>(this->hyperPeriod / unit)), unit);

    for (auto &runnable : this->dag->GetOrderOfPriorityRunnables()) {
        int runnableId = runnable->GetId();
        int runnableMaxCycle = static_cast<int>(this->hyperPeriod) / this->runnableInformations[runnableId].period;
        std::clog << "[simulation.cpp] CheckPoint 4-2" << std::endl;

        for (int cycle = 0; cycle < runnableMaxCycle; cycle++) {
            double releaseTime = this->runnableInformations[runnableId].period * cycle + this->runnableInformations[runnableId].offset;
            double deadTime = this->runnableInformations[runnableId].period * (cycle + 1) + this->runnableInformations[runnableId].offset;

            int unitIndex = static_cast<int>(std::floor(releaseTime / unit));
            std::clog << "[simulation.cpp] CheckPoint 4-3" << std::endl;

            // Regard time-line
            while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;
            std::clog << "[simulation.cpp] CheckPoint 4-4" << std::endl;

            // Set start time
            this->runnableExecutions[runnableId][cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);
            if (this->runnableExecutions[runnableId][cycle].startTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }
            std::clog << "[simulation.cpp] CheckPoint 4-5" << std::endl;

            // Set end time
            double executionTime = this->runnableInformations[runnableId].executionTime;

            while (executionTime) {
                std::clog << "[simulation.cpp] CheckPoint 4-6" << std::endl;
                if (emptyTimes[unitIndex] < executionTime) {
                    executionTime -= emptyTimes[unitIndex];
                    emptyTimes[unitIndex] = 0.0;

                    unitIndex++;
                } else {
                    this->runnableExecutions[runnableId][cycle].endTime = (static_cast<double>(unitIndex) * unit) + executionTime;
                    emptyTimes[unitIndex] -= executionTime;
                    executionTime = 0.0;
                }
            }
            std::clog << "[simulation.cpp] CheckPoint 4-7" << std::endl;
        }
    }
}

void Simulation::SetProcessExecutions() {
    for (auto &runnable : this->dag->GetInputRunnables()) {
        int eachMaxCycle = this->hyperPeriod / this->runnableInformations[runnable->GetId()].period;

        for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
            std::map<int, double> path;
            this->TraceProcess(runnable->GetId(), cycle, runnable->GetId(), cycle, 0, path);
        }
    }
}

void Simulation::TraceProcess(int inputRunnableId, int inputCycle, int thisRunnableId, int thisCycle, int thisHyperPeriodCount, std::map<int, double>& path) {
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

    if (this->runnableInformations[thisRunnableId].status == 1) {  //precedence처럼 해보셈
        if (this->processExecutions.find(std::make_pair(inputRunnableId, thisRunnableId)) == this->processExecutions.end()) {
            std::vector<ExecutionInformation> tmpVector = { {this->runnableCommunications[inputRunnableId][inputCycle].startTime, this->runnableCommunications[thisRunnableId][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod} };
            this->processExecutions.insert(std::make_pair(std::make_pair(inputRunnableId, thisRunnableId), tmpVector));
        } else {
            ExecutionInformation tmpInfo = {this->runnableCommunications[inputRunnableId][inputCycle].startTime, this->runnableCommunications[thisRunnableId][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod};
            if (static_cast<int>(this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].size()) != thisCycle + 1) this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].push_back(tmpInfo); //그냥 push back 말고 worst로 비교해서 넣기
            else if (this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)][thisCycle].endTime < this->runnableCommunications[thisRunnableId][thisCycle].endTime) this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)][thisCycle].endTime = this->runnableCommunications[thisRunnableId][thisCycle].endTime;
        }
    } else {
        if (path.find(thisRunnableId) == path.end()) path.insert({thisRunnableId, -1.0});
        if (this->runnableCommunications[thisRunnableId][thisCycle].startTime > path[thisRunnableId]) {
            path[thisRunnableId] = runnableCommunications[thisRunnableId][thisCycle].startTime;
            
            for (auto &outputRunnable : this->dag->GetRunnable(thisRunnableId)->GetOutputRunnables()) {
                int outputRunnableId = outputRunnable->GetId();

                int tmpCycle = 0;
                double hyperPeriodCount = 0;
                int outputRunnableReadTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;
                //std::cout << inputRunnableId << " " << thisRunnableId << " " << outputRunnableId << std::endl;
                // If thisRunnable has hyperPeriod Count, outputRunnable have same hyperPeriod start.

                while ((this->runnableCommunications[thisRunnableId][thisCycle].startTime) > outputRunnableReadTime) {
                    tmpCycle++;

                    if (tmpCycle == maxCycle || this->runnableCommunications[outputRunnableId][tmpCycle].endTime == -1.0) {
                        outputRunnableReadTime = this->runnableCommunications[outputRunnableId][0].endTime + this->hyperPeriod;

                        tmpCycle = 0;
                        hyperPeriodCount++;
                    } else {
                        double inputStartTime = this->runnableCommunications[thisRunnableId][thisCycle].startTime;
                        double outputEndTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;
                        double outputPeriod = this->runnableInformations[outputRunnableId].period;
                        tmpCycle += (((inputStartTime - outputEndTime) / outputPeriod) > 4) ? (static_cast<int>((inputStartTime - outputEndTime) / outputPeriod) - 3) : 0;

                        outputRunnableReadTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;
                    }
                }
                this->TraceProcess(inputRunnableId, inputCycle, outputRunnableId, tmpCycle, hyperPeriodCount, path);
            }
        }
    }
}

double Simulation::GetReactionTime() {
    std::pair<int, int> WorstPair;
    double WorstReactionTime =  0.0;

    for (auto &StoEs : processExecutions) {

        for (auto &StoE : StoEs.second) {
            double tmpThisReactionTime = StoE.endTime - StoE.startTime;
            if (WorstReactionTime < tmpThisReactionTime) {
                WorstReactionTime = tmpThisReactionTime;
                WorstPair.first = StoEs.first.first;
                WorstPair.second = StoEs.first.second;
            }
        }
    }
    std::cout << "Worst Pair Input, Output Runnable : " << WorstPair.first << " " << WorstPair.second << ", Reaction Time : " << WorstReactionTime << std::endl;

    return WorstReactionTime;
}

double Simulation::GetDataAge() {
    double WorstDataAge = 0.0;
    //double WorstReactionTime = 0.0;
    std::pair<int, int> WorstPair;
    std::vector<double> dataage;

    for (auto &StoEs : processExecutions) {
        for (auto &StoE : StoEs.second) {
            
            //int order = std::distance(this->runnableExecutions[StoE.first.second].begin(), std::find_if(this->runnableExecutions[StoE.first.second].begin(), this->runnableExecutions[StoE.first.second].end(), [](ExecutionInformation a) { return (a.endTime == StoE.second.endTime) ? true : false; }));
            //int thisOutputRunnable = StoEs.first.second;
            std::cout << "input : " << StoEs.first.first << " , output : " << StoEs.first.second << " , start time : " << StoE.startTime << " , " << StoE.endTime << std::endl;


        }

        
    }



    return WorstDataAge;
}

std::vector<ResultInformation> Simulation::GetBestReactionTime(int numberOfCase) {
    std::sort(this->results.begin(), this->results.end(), [](ResultInformation a, ResultInformation b) { return a.reactionTime < b.reactionTime; });

    std::vector<ResultInformation> tmpVector(numberOfCase);
    std::copy(this->results.begin(), this->results.begin() + numberOfCase, tmpVector.begin());

    return tmpVector;
}

std::vector<ResultInformation> Simulation::GetWorstReactionTime(int numberOfCase) {
    std::sort(this->results.begin(), this->results.end(), [](ResultInformation a, ResultInformation b) { return a.reactionTime > b.reactionTime; });

    std::vector<ResultInformation> tmpVector(numberOfCase);
    std::copy(this->results.begin(), this->results.begin() + numberOfCase, tmpVector.begin());

    return tmpVector;
}

std::vector<ResultInformation> Simulation::GetBestDataAge(int numberOfCase) {
    std::sort(this->results.begin(), this->results.end(), [](ResultInformation a, ResultInformation b) { return a.dataAge < b.dataAge; });

    std::vector<ResultInformation> tmpVector(numberOfCase);
    std::copy(this->results.begin(), this->results.begin() + numberOfCase, tmpVector.begin());

    return tmpVector;
}

std::vector<ResultInformation> Simulation::GetWorstDataAge(int numberOfCase) {
    std::sort(this->results.begin(), this->results.end(), [](ResultInformation a, ResultInformation b) { return a.dataAge > b.dataAge; });

    std::vector<ResultInformation> tmpVector(numberOfCase);
    std::copy(this->results.begin(), this->results.begin() + numberOfCase, tmpVector.begin());

    return tmpVector;
}

void Simulation::SaveData() {
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    rapidjson::Value resultObject(rapidjson::kObjectType);

    resultObject.AddMember("Reaction Time Ranking", this->SaveReactionTime(allocator), allocator);

    resultObject.AddMember("Data Age Ranking", this->SaveDataAge(allocator), allocator);

    // Save to json
    std::string thisTime = this->simulationTime;
    std::string fileName = "../data/Result_" + thisTime + ".json";

    std::ofstream ofs(fileName.c_str());
    rapidjson::OStreamWrapper osw(ofs);

    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    resultObject.Accept(writer);

    ofs.close();
}

void Simulation::SaveDag() {
    this->dag->SaveDag(this->simulationTime);
}

rapidjson::Value Simulation::SaveReactionTime(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    rapidjson::Value reactionTimeArray(rapidjson::kArrayType);

    // Best Reaction Time
    rankingCount = 0;

    for (auto &reactionTime : this->GetBestReactionTime(5)) {
        rapidjson::Value bestReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value bestReactionTimeArray(rapidjson::kArrayType);

        bestReactionTimeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestReactionTimeObject.AddMember("Reactoin Time", reactionTime.reactionTime, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(reactionTime.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetPeriod(), allocator);

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

        reactionTimeArray.PushBack(bestReactionTimeArray, allocator);
    }

    // Worst Reaction Time
    rankingCount = this->dag->GetNumberOfSequenceCase() + 1;

    for (auto &reactionTime : this->GetWorstReactionTime(5)) {
        rapidjson::Value worstReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value worstReactionTimeArray(rapidjson::kArrayType);

        worstReactionTimeObject.AddMember("Ranking", --rankingCount, allocator);

        worstReactionTimeObject.AddMember("Reactoin Time", reactionTime.reactionTime, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(reactionTime.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetPeriod(), allocator);

            rapidjson::Value sequenceArray(rapidjson::kArrayType);
            int numberOfRunnables = task->GetNumberOfRunnables();
            for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
                sequenceArray.PushBack(sequence[vectorPointer + runnableIndex], allocator);
            }
            vectorPointer += numberOfRunnables;
            taskObject.AddMember("Runnable Sequence", sequenceArray, allocator);

            worstReactionTimeArray.PushBack(taskObject, allocator);
        }
        worstReactionTimeObject.AddMember("Sequence", worstReactionTimeArray, allocator);

        reactionTimeArray.PushBack(worstReactionTimeObject, allocator);
    }

    return reactionTimeArray;
}

rapidjson::Value Simulation::SaveDataAge(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    rapidjson::Value dataAgeArray(rapidjson::kArrayType);

    // Best Data Age
    rankingCount = 0;

    for (auto &dataAge : this->GetBestDataAge(5)) {
        rapidjson::Value bestDataAgeObject(rapidjson::kObjectType);
        rapidjson::Value bestDataAgeArray(rapidjson::kArrayType);

        bestDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestDataAgeObject.AddMember("Reactoin Time", dataAge.dataAge, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(dataAge.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetPeriod(), allocator);

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

    // Worst Data Age
    rankingCount = this->dag->GetNumberOfSequenceCase() + 1;
    
    for (auto &dataAge : this->GetWorstDataAge(5)) {
        rapidjson::Value worstDataAgeObject(rapidjson::kObjectType);
        rapidjson::Value worstDataAgeArray(rapidjson::kArrayType);

        worstDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        worstDataAgeObject.AddMember("Reactoin Time", dataAge.dataAge, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(dataAge.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetPeriod(), allocator);

            rapidjson::Value sequenceArray(rapidjson::kArrayType);
            int numberOfRunnables = task->GetNumberOfRunnables();
            for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
                sequenceArray.PushBack(sequence[vectorPointer + runnableIndex], allocator);
            }
            vectorPointer += numberOfRunnables;
            taskObject.AddMember("Runnable Sequence", sequenceArray, allocator);

            worstDataAgeArray.PushBack(taskObject, allocator);
        }
        worstDataAgeObject.AddMember("Sequence", worstDataAgeArray, allocator);

        dataAgeArray.PushBack(worstDataAgeObject, allocator);
    }

    return dataAgeArray;
}