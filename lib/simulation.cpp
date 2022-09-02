#include "simulation.hpp"


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
    RunnableInformation initialRunnableInformation = {-1, -1, -1.0, -1.0, -1.0};

    std::clog << "[simulation.cpp] CheckPoint 2-2" << std::endl;
    std::vector<RunnableInformation>(this->numberOfRunnables, initialRunnableInformation).swap(this->runnableInformations_);

    std::clog << "[simulation.cpp] CheckPoint 2-3" << std::endl;
}

void Simulation::Simulate() {
	std::vector<std::vector<std::vector<RUNNABLE>>> SeqeuencesPerTasks; // [Task][Number Of Case][Runnables]
	
	
	
    int numberOfCase = ;
	
	this->SetRunnableInformations();


    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        std::clog << "[simulation.cpp] CheckPoint 5" << std::endl;
        std::vector<std::vector<CommunicationInformation>> = this->GetRunnableCommunications(this->RunnableInformations_, );
        
        std::clog << "[simulation.cpp] CheckPoint 6" << std::endl;
        this->SetProcessExecutions();

        std::clog << "[simulation.cpp] CheckPoint 7" << std::endl;
        this->SetResult();

        std::clog << "[simulation.cpp] CheckPoint 8" << std::endl;
		
		std::system("clear");
		std::cout << "===========================================================================================================================\n";
		std::cout << "simulation Case     : " << std::setw(10) << caseIndex << "/" << << std::setw(10) numberOfCase << "\n";
		std::cout << "Number Of Runnables : " << std::setw(10) << this->numberOfRunnables_ << "\n";
		std::cout << "Number Of Tasks     : " << std::setw(10) << this->numberOfTasks_ << "\n";
		std::cout << "===========================================================================================================================" << std::endl;
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
        unit = std::gcd(unit, ((task->GetOffset() != 0.0) ? std::gcd(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
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
            if (static_cast<int>(this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].size()) != inputCycle + 1) this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].push_back(tmpInfo); //그냥 push back 말고 worst로 비교해서 넣기
            else if (this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)][inputCycle].endTime < this->runnableCommunications[thisRunnableId][thisCycle].endTime) this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)][inputCycle].endTime = this->runnableCommunications[thisRunnableId][thisCycle].endTime;
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
    int outputrun = 0;
    int inputrun = 0;

    for (auto &ouputRunnable : this->dag->GetOutputRunnables()) {
        int runnableId = ouputRunnable->GetId();
        double biggestEndTime = 0.0; 
        std::vector<double> endTimes;
        for (auto &StoEs : processExecutions) {
            if (StoEs.first.second == runnableId) {
                for (auto &StoE : StoEs.second) {
                    if (StoE.endTime > biggestEndTime) {
                        biggestEndTime = StoE.endTime;
                        inputrun = StoEs.first.first;
                        outputrun = StoEs.first.second;
                    }
                }
            }
        }

        int hyperperiodCount = std::ceil(biggestEndTime/this->hyperPeriod);
        for (auto &StoEs : processExecutions) {
            if (StoEs.first.second == runnableId) {
                for (auto &StoE : StoEs.second) {
                    double tmpEndTime = StoE.endTime;
                    while (tmpEndTime <= biggestEndTime) {
                        endTimes.push_back(tmpEndTime);                    
                        tmpEndTime += this->hyperPeriod;
                    }
                }
            }
        }

        std::vector<double> endTimeTable;
        for (int count = 0; count < hyperperiodCount; count++) {
            for (auto &executionTime : this->runnableExecutions[runnableId]) {
                endTimeTable.push_back(executionTime.endTime + this->hyperPeriod*count);
            }
        }

        std::vector<int> endTimeIndex; 
        for (auto &endTime : endTimes) {
            endTimeIndex.push_back(std::distance(endTimeTable.begin(), std::find(endTimeTable.begin(), endTimeTable.end(), endTime)));
        }

        std::sort(endTimeIndex.begin(), endTimeIndex.end(), [](int a, int b) { return a < b; });

        int preindex = endTimeIndex[0];
        for (auto &index : endTimeIndex) {
            if (index > preindex + 1)  {
                if (WorstDataAge < endTimeTable[index] - endTimeTable[preindex]) {
                    WorstDataAge = endTimeTable[index] - endTimeTable[preindex];
                    std::cout << "debug : " << endTimeTable[index] << " " << endTimeTable[preindex]  << " " << ouputRunnable->GetExecutionTime() << " " << inputrun << " " << outputrun << std::endl;

                }
            }
            preindex = index;
        }
    }
    std::cout << "Worst data age : " << WorstDataAge << std::endl;

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

    std::clog << "[simulation.cpp] Checkpoint 9-1" << std::endl;

    resultObject.AddMember("Reaction Time Ranking", this->SaveReactionTime(allocator), allocator);

    std::clog << "[simulation.cpp] Checkpoint 9-2" << std::endl;

    resultObject.AddMember("Data Age Ranking", this->SaveDataAge(allocator), allocator);

    std::clog << "[simulation.cpp] Checkpoint 9-3" << std::endl;

    // Save to json
    std::string thisTime = this->simulationTime;
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
    this->dag->SaveDag(this->simulationTime);
}

rapidjson::Value Simulation::SaveReactionTime(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    int numberOfCase;
    rapidjson::Value reactionTimeArray(rapidjson::kArrayType);

    // Best Reaction Time
    numberOfCase = (this->dag->GetNumberOfSequenceCase() > 5) ? 5 : this->dag->GetNumberOfSequenceCase(); 
    rankingCount = 0;

    for (auto &reactionTime : this->GetBestReactionTime(numberOfCase)) {
        rapidjson::Value bestReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value bestReactionTimeArray(rapidjson::kArrayType);

        bestReactionTimeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestReactionTimeObject.AddMember("Reactoin Time", reactionTime.reactionTime, allocator);

        std::clog << "[simulation.cpp] Checkpoint 9-1-3" << std::endl;

        std::vector<int> sequence = this->dag->GetRunnableSequence(reactionTime.sequenceIndex);
        std::clog << "[simulation.cpp] Checkpoint 9-1-4" << std::endl;
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            std::clog << "[simulation.cpp] Checkpoint 9-1-5" << std::endl;
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetOffset(), allocator);
            std::clog << "[simulation.cpp] Checkpoint 9-1-6" << std::endl;

            rapidjson::Value sequenceArray(rapidjson::kArrayType);
            int numberOfRunnables = task->GetNumberOfRunnables();
            std::clog << "[simulation.cpp] Checkpoint 9-1-7" << std::endl;
            for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
                sequenceArray.PushBack(sequence[vectorPointer + runnableIndex], allocator);
            }
            std::clog << "[simulation.cpp] Checkpoint 9-1-8" << std::endl;
            vectorPointer += numberOfRunnables;
            taskObject.AddMember("Runnable Sequence", sequenceArray, allocator);
            std::clog << "[simulation.cpp] Checkpoint 9-1-9" << std::endl;

            bestReactionTimeArray.PushBack(taskObject, allocator);
            std::clog << "[simulation.cpp] Checkpoint 9-1-10" << std::endl;
        }
        bestReactionTimeObject.AddMember("Sequence", bestReactionTimeArray, allocator);
        std::clog << "[simulation.cpp] Checkpoint 9-1-11" << std::endl;

        reactionTimeArray.PushBack(bestReactionTimeObject, allocator);
        std::clog << "[simulation.cpp] Checkpoint 9-1-12" << std::endl;
    }

    // Worst Reaction Time
    numberOfCase = ((this->dag->GetNumberOfSequenceCase() - 5) > 5) ? 5 : ((this->dag->GetNumberOfSequenceCase() - 5) > 0) ? (this->dag->GetNumberOfSequenceCase() - 5) : 0;
    rankingCount = this->dag->GetNumberOfSequenceCase() + 1;

    for (auto &reactionTime : this->GetWorstReactionTime(numberOfCase)) {
        rapidjson::Value worstReactionTimeObject(rapidjson::kObjectType);
        rapidjson::Value worstReactionTimeArray(rapidjson::kArrayType);

        worstReactionTimeObject.AddMember("Ranking", --rankingCount, allocator);

        worstReactionTimeObject.AddMember("Reactoin Time", reactionTime.reactionTime, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(reactionTime.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetOffset(), allocator);

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

    std::clog << "[simulation.cpp] Checkpoint 9-1-13" << std::endl;

    return reactionTimeArray;
}

rapidjson::Value Simulation::SaveDataAge(rapidjson::Document::AllocatorType& allocator) {
    int rankingCount;
    int numberOfCase;
    rapidjson::Value dataAgeArray(rapidjson::kArrayType);

    // Best Data Age
    numberOfCase = (this->dag->GetNumberOfSequenceCase() > 5) ? 5 : this->dag->GetNumberOfSequenceCase(); 
    rankingCount = 0;

    for (auto &dataAge : this->GetBestDataAge(numberOfCase)) {
        rapidjson::Value bestDataAgeObject(rapidjson::kObjectType);
        rapidjson::Value bestDataAgeArray(rapidjson::kArrayType);

        bestDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        bestDataAgeObject.AddMember("Reactoin Time", dataAge.dataAge, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(dataAge.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetOffset(), allocator);

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
    numberOfCase = ((this->dag->GetNumberOfSequenceCase() - 5) > 5) ? 5 : ((this->dag->GetNumberOfSequenceCase() - 5) > 0) ? (this->dag->GetNumberOfSequenceCase() - 5) : 0;
    rankingCount = this->dag->GetNumberOfSequenceCase() + 1;
    
    for (auto &dataAge : this->GetWorstDataAge(numberOfCase)) {
        rapidjson::Value worstDataAgeObject(rapidjson::kObjectType);
        rapidjson::Value worstDataAgeArray(rapidjson::kArrayType);

        worstDataAgeObject.AddMember("Ranking", ++rankingCount, allocator);
        worstDataAgeObject.AddMember("Reactoin Time", dataAge.dataAge, allocator);

        std::vector<int> sequence = this->dag->GetRunnableSequence(dataAge.sequenceIndex);
        int vectorPointer = 0;
        for (auto &task : this->dag->GetTasks()) {
            rapidjson::Value taskObject(rapidjson::kObjectType);
            taskObject.AddMember("Period", task->GetPeriod(), allocator);
            taskObject.AddMember("Offset", task->GetOffset(), allocator);

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