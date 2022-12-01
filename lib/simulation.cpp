#include "simulation.hpp"


void Simulation::Initialize() {
    // this->InitializeSequenceCommand();
    std::unique_ptr<Sequence> sequenceClass;
    sequenceClass = std::make_unique<CustomCase>(this->dag_);
    this->SetSequenceCommand(sequenceClass);

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

    this->DisplayDag();

    this->SetNumberOfPathList();
    std::cerr << "ckpt init2\n";
    // this->SetVisitRunnable();
    this->SetPathes();
    std::cerr << "ckpt init3\n";
}

void Simulation::SetDataDirectory() {
    time_t rawTime;
    struct tm* pTimeInfo;

    rawTime = time(NULL);
    pTimeInfo = localtime(&rawTime);

    std::string simulationTime = std::to_string(pTimeInfo->tm_mon + 1) + "_" + std::to_string(pTimeInfo->tm_mday) + "_" + std::to_string(pTimeInfo->tm_hour) + "_" + std::to_string(pTimeInfo->tm_min);
    this->dataDirectory_ = "../data/T" + std::to_string(this->numberOfTasks_) + "R" + std::to_string(this->numberOfRunnables_) + "U" + std::to_string(this->utilization_) + "W" + simulationTime;
}

void Simulation::MakeDataDirectory() {
    // make directory for save datas
    if (mkdir(this->dataDirectory_.c_str(), 0776) == -1 && errno != EEXIST) {
        // std::cerr << "[AUTOSAR Simulator] directory create error : " << strerror(errno) << std::endl;
        throw std::invalid_argument("'data' directory is not exist");
    }
}

// void Simulation::InitializeSequenceCommand() {
//     // Initialize sequence
//     // std::cout << "\033[H\033[2J\033[3J";
//     std::cout << "*** What do you want sequencing strategy? ***" << "\n";
//     std::cout << " 0 : Sequencing by Precedence" << "\n";
//     std::cout << " 1 : All Case" << "\n";
//     std::cout << " 2 : One Case" << "\n";
//     std::cout << "\n" << "Enter Number : ";
//     int sequenceMethod = -1;
//     std::cin >> sequenceMethod;

//     std::unique_ptr<Sequence> sequenceClass;
//     switch(sequenceMethod) {
//         case 0: {
//             sequenceClass = std::make_unique<SequenceByPrecedence>(this->dag_);
//             break;
//         }

//         case 1: {
//             sequenceClass = std::make_unique<SequenceByAllcase>(this->dag_);
//             break;
//         }

//         case 2: {
//             sequenceClass = std::make_unique<OptimizeCase>(this->dag_);
//             break;
//         }

//         case 3: {
//             sequenceClass = std::make_unique<OneCase>(this->dag_);
//             break;
//         }

//         default: {
//             throw std::invalid_argument("[Sequence Method] Entering wrong number.");
//         }
//     }

//     this->SetSequenceCommand(sequenceClass);
// }

// void Simulation::Simulate() {
//     // For Reduce malloc delay
//     std::cerr << "ckpt1\n";
//     this->CreateProcessExecutions();
//     std::cerr << "ckpt2\n";
//     this->CreateVisitedWorstCycleList();
//     std::cerr << "ckpt3\n";
//     this->SetResultMap();
//     std::cerr << "ckpt4\n";

//     // Set Time
//     int limitProcessTime;
//     std::cout << "Select limitation of time (second) : ";
//     std::cin >> limitProcessTime;

//     // Random Number Generator
//     std::clock_t simulationStart = std::clock();
//     std::clock_t simulationCheckpoint;

//     int numberOfCase = this->GetNumberOfCase();
//     std::cerr << numberOfCase << std::endl;
//     for (int caseIndex = numberOfCase; caseIndex > 0; caseIndex--) {
//         int simulationIndex = this->GetRandomEmptyIndex();
//         std::cerr << simulationIndex << std::endl;
//         this->SetSequence(simulationIndex);
//         std::cerr << simulationIndex << std::endl;

//         this->SetResult();
//         int seedNumber = simulationIndex;

//         simulationCheckpoint = std::clock();
//         double processTime = static_cast<double>(simulationCheckpoint - simulationStart) / CLOCKS_PER_SEC;

//         this->DisplayResult(seedNumber, processTime, limitProcessTime);

//         this->SaveDataToCSV(seedNumber);

//         if (limitProcessTime != 0) {
//             if (static_cast<int>(processTime) > limitProcessTime) {
//                 break;
//             }
//         }
//     }

//     //this->SaveAllDataToCSV();
// }

void Simulation::Simulate() {
    this->SetSequence(1);
    this->SetRunnableCommunicationTimes();

    std::string fileDirectory = this->dataDirectory_ + "/Result.csv";

    std::ofstream resultFile;
    resultFile.open (fileDirectory.c_str(), std::ios_base::out | std::ios_base::app);

    std::cout << "[Debug] Number Of Pathes" << this->pathes_.size() << "\n";
    for (auto &path : this->pathes_) {
        std::shared_ptr<RUNNABLE> sensorRunnable;
        std::shared_ptr<RUNNABLE> actuatorRunnable;

        for (int i = 0; i < this->numberOfRunnables_; i++) {
            if (path[i]) {
                if (this->dag_->GetRunnable(i)->GetStatus() == 0) {
                    sensorRunnable = this->dag_->GetRunnable(i);
                } else if (this->dag_->GetRunnable(i)->GetStatus() == 1) {
                    actuatorRunnable = this->dag_->GetRunnable(i);
                }
            }
        }

        long long int worstReactionTime = 0;
        long long int worstDataAge = 0;

        std::vector<int> worstCycle(this->numberOfRunnables_, 0);
        int preSensorRunnableCycle = 0;
        int currentSensorRunnableCycle = 0;
        int currentActuatorRunnableCycle = 0;

        for (currentSensorRunnableCycle = 0; currentSensorRunnableCycle < sensorRunnable->GetMaxCycle(); currentSensorRunnableCycle++) {
            long long int hyperPeriodCount = 0ll;
            int currentTime = sensorRunnable->executionTimes_[currentSensorRunnableCycle].endTime;

            std::cout << "[Debug] Start : " << currentTime << "\n";
            for (int i = sensorRunnable->GetId() + 1; i < this->numberOfRunnables_; i++) {
                if (path[i]) {
                    std::shared_ptr<RUNNABLE> currentRunnable = this->dag_->GetRunnable(i);

                    if (currentRunnable->executionTimes_[currentRunnable->GetMaxCycle() - 1].startTime < currentTime) {
                        currentTime = currentRunnable->executionTimes_[0].endTime;
                        hyperPeriodCount += 1ll;
                        worstCycle[i] = 0;
                    } else {
                        worstCycle[i] = 0;
                        while (currentRunnable->executionTimes_[worstCycle[i]].startTime < currentTime) worstCycle[i]++;
                        currentTime = currentRunnable->executionTimes_[worstCycle[i]].endTime;
                    }
                }
            }
            std::cout << "[Debug] end\n";

            currentActuatorRunnableCycle = worstCycle[actuatorRunnable->GetId()];

            if (worstReactionTime < ((this->dag_->GetHyperPeriod() * hyperPeriodCount) + actuatorRunnable->executionTimes_[currentActuatorRunnableCycle].endTime - sensorRunnable->executionTimes_[currentSensorRunnableCycle].startTime)) worstReactionTime = (this->dag_->GetHyperPeriod() * hyperPeriodCount) + actuatorRunnable->executionTimes_[currentActuatorRunnableCycle].endTime - sensorRunnable->executionTimes_[currentSensorRunnableCycle].startTime;
            if (worstDataAge < ((this->dag_->GetHyperPeriod() * hyperPeriodCount) + actuatorRunnable->executionTimes_[(currentActuatorRunnableCycle - 1 < 0) ? actuatorRunnable->GetMaxCycle() - 1 : currentActuatorRunnableCycle - 1].endTime - sensorRunnable->executionTimes_[preSensorRunnableCycle].startTime)) worstDataAge = (this->dag_->GetHyperPeriod() * hyperPeriodCount) + actuatorRunnable->executionTimes_[(currentActuatorRunnableCycle - 1 < 0) ? actuatorRunnable->GetMaxCycle() : currentActuatorRunnableCycle - 1].endTime - sensorRunnable->executionTimes_[preSensorRunnableCycle].startTime;
            if (worstDataAge < worstReactionTime) worstDataAge = worstReactionTime;

            preSensorRunnableCycle = currentSensorRunnableCycle;
        }

        resultFile << static_cast<double>(worstReactionTime / 1000ll) + (static_cast<double>(worstReactionTime % 1000ll) / 1000.0) << ",";
        resultFile << static_cast<double>(worstDataAge / 1000ll) + (static_cast<double>(worstDataAge % 1000ll) / 1000.0) << ",";

        std::cout << "[Debug] worstReactionTime" << worstReactionTime << "\n";
        std::cout << "[Debug] worstDataAge" << worstDataAge << "\n";

        for (int i = 0; i < this->numberOfRunnables_; i++) {
            if (path[i]) {
                if (this->dag_->GetRunnable(i)->GetStatus() != 1) {
                    resultFile << i << ",";
                } else {
                    resultFile << i << "\n";
                }
            }
        }
    }

    resultFile.close();

    this->dag_->SaveDag(this->dataDirectory_);
    this->dag_->SaveMapping(this->dataDirectory_);
}

// void Simulation::SetResult() {
//     std::cerr << "ckpt1\n";
//     this->SetRunnableCommunicationTimes();
//     std::cerr << "ckpt2\n";
//     this->SetProcessExecutionsList();
//     std::cerr << "ckpt3\n";
//     this->InitResultMap();
//     std::cerr << "ckpt4\n";
//     this->GetWorstReactionTime();
//     std::cerr << "ckpt5\n";
//     this->GetWorstDataAge();
//     std::cerr << "ckpt6\n";
// }

// void Simulation::CreateProcessExecutions() {
//     std::map<int, std::map<int, std::vector<RequiredTime>>>().swap(this->processExecutions_);

//     std::vector<bool> visitiedRunnable(this->numberOfRunnables_, false);

//     for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
//         std::fill(visitiedRunnable.begin(), visitiedRunnable.end(), false);
//         auto inputRunnableIterPair = this->processExecutions_.insert({inputRunnable->id_, std::map<int, std::vector<RequiredTime>>()});

//         this->TraceProcessExecutions(inputRunnableIterPair.first, inputRunnable, visitiedRunnable);
//     }
// }

// void Simulation::TraceProcessExecutions(auto& inputRunnableIter, const std::shared_ptr<RUNNABLE>& thisRunnable, std::vector<bool>& visitiedRunnable) {
//     if (!visitiedRunnable[thisRunnable->id_]) {
//         visitiedRunnable[thisRunnable->id_] = true;

//         if (thisRunnable->GetStatus() != 1) {
//             for (auto &outputRunnable : thisRunnable->GetOutputRunnables()) {
//                 this->TraceProcessExecutions(inputRunnableIter, outputRunnable, visitiedRunnable);
//             }
//         } else {
//             auto outputRunnableIter = inputRunnableIter->second.find(thisRunnable->id_);

//             if (outputRunnableIter == inputRunnableIter->second.end()) {
//                 inputRunnableIter->second.insert({thisRunnable->id_, std::vector<RequiredTime>(this->dag_->GetRunnable(inputRunnableIter->first)->GetMaxCycle(), {-1LL, -1LL})});
//             }
//         }
//     }
// }

// void Simulation::InitializeProcessExecutions() {
//     RequiredTime initializedRequiredTime = {-1LL, -1LL};

//     for (auto& inputRunnablePair : this->processExecutions_) {
//         for (auto& outputRunnablePair : inputRunnablePair.second) {
//             std::fill(outputRunnablePair.second.begin(), outputRunnablePair.second.end(), initializedRequiredTime);
//         }
//     }
// }

void Simulation::SetNumberOfPathList() {
    std::vector<long long int> number_of_path(this->numberOfRunnables_, 1ll);
    
    for (int i = this->numberOfRunnables_ - 1; i >= 0; --i) {
		if (this->dag_->GetRunnable(i)->GetStatus() != 1) { // sensor, middle runnable
            int tmp_number_of_path = 0;

            for (auto &output_runnable : this->dag_->GetRunnable(i)->GetOutputRunnables()) {
                tmp_number_of_path += number_of_path[output_runnable->GetId()];
            }

            number_of_path[i] = tmp_number_of_path;
		}
	}

    this->numberOfPathList_.swap(number_of_path);
}

const long long int Simulation::GetNumberOfPath() {
    long long int number_of_path = 0;

    for (auto &input_runnable : this->dag_->GetInputRunnables()) {
        number_of_path += this->numberOfPathList_[input_runnable->GetId()];
    }

    return number_of_path;
}

// void Simulation::SetVisitRunnable() {
//     std::vector<std::vector<bool>>(this->GetNumberOfPath(), std::vector<bool>(this->numberOfRunnables_, false)).swap(this->visitRunnable_);
//     // std::vector<std::vector<bool>>((this->GetNumberOfPath() > 5ll) ? 5ll : this->GetNumberOfPath(), std::vector<bool>(this->numberOfRunnables_, false)).swap(this->visitRunnable_);

//     const long long int numberofPath = this->GetNumberOfPath();
//     // const long long int numberofPath = (this->GetNumberOfPath() > 5ll) ? 5ll : this->GetNumberOfPath();
//     for (long long int i = 0; i < numberofPath; i++) {
//         std::shared_ptr<RUNNABLE> tmp_runnable;
//         long long int path_index = i + 1;

//         // Select Sensor Runnable
//         for (auto &input_runnable : this->dag_->GetInputRunnables()) {
//             if (path_index > this->numberOfPathList_[input_runnable->GetId()]) {
//                 path_index -= this->numberOfPathList_[input_runnable->GetId()];
//             } else {
//                 this->visitRunnable_[i][input_runnable->GetId()] = true;
//                 tmp_runnable = input_runnable->GetSharedPtr();
//                 break;
//             }
//         }

//         // Trace Path
//         while (path_index > 0) {
//             if (tmp_runnable->GetStatus() != 1) {
//                 for (auto &runnable : tmp_runnable->GetOutputRunnables()) { 
//                     if (path_index > this->numberOfPathList_[runnable->GetId()]) {
//                         path_index -= this->numberOfPathList_[runnable->GetId()];
//                     } else {
//                         this->visitRunnable_[i][runnable->GetId()] = true;
//                         tmp_runnable = runnable;
//                     }
//                 }

//                 // std::cerr << "ramained path_index : " << path_index << "\n";
//             } else {
//                 break;
//             }
//         }
//     }
//     // std::cerr << "ckpt Set Visit 4\n";
// }

void Simulation::SetPathes() {
    std::vector<std::vector<bool>>().swap(this->pathes_);
    this->pathes_.reserve(this->GetNumberOfPath());

    std::vector<bool> tmp_path(this->numberOfRunnables_, false);

    for (auto &input_runnable : this->dag_->GetInputRunnables()) {
        this->TracePath(input_runnable, tmp_path);
    }
}

void Simulation::TracePath(const std::shared_ptr<RUNNABLE>& runnable, std::vector<bool> tmp_path) {
    tmp_path[runnable->GetId()] = true;

    if (runnable->GetStatus() == 1) {        
        this->pathes_.push_back(tmp_path);
    } else {
        for (auto &output_runnable : runnable->GetOutputRunnables()) {
            this->TracePath(output_runnable, tmp_path);
        }
    }
}

// void Simulation::GetReactionTimeList() {
//     for (auto &visitRunnable : this->visitRunnable_) {
//         int maxCycle = 0;

//         for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
//             if (visitRunnable[inputRunnable->GetId()]) {
//                 maxCycle = inputRunnable->GetMaxCycle();
//                 break;
//             }
//         }
        
//         std::vector<long long int> result(maxCycle, -1LL);

//         // std::clock_t simulationStart = std::clock();
        
//         for (int cycle = 0; cycle < maxCycle; cycle++) {
//             result[cycle] = this->SetReactionTimeNew(visitRunnable, cycle);
//         }

//         // std::clock_t simulationEnd1 = std::clock();

//         // std::vector<std::future<long long int>> workers;
//         // workers.reserve(maxCycle);
//         // for (int cycle = 0; cycle < maxCycle; cycle++) {
//         //     workers.emplace_back(std::async(std::launch::async, &Simulation::SetReactionTimeNew, this, visitRunnable, cycle));
//         // }

//         // for (int cycle = 0; cycle < maxCycle; cycle++) {
//         //     result[cycle] = workers[cycle].get();
//         // }

//         // std::vector<std::thread> workers;
//         // int max_thread = (MAX_THREAD < maxCycle) ? MAX_THREAD : maxCycle;
//         // for (int threadIndex = 0; threadIndex < max_thread; threadIndex++) {
//         //     int startIndex = ((maxCycle / max_thread)  * threadIndex);
//         //     int endIndex = (((maxCycle / max_thread)  * (threadIndex + 1) - 1) > maxCycle) ? maxCycle : ((maxCycle / max_thread)  * (threadIndex + 1) - 1);
//         //     workers.emplace_back(std::thread(&Simulation::SetReactionTimeNew, this, visitRunnable, startIndex, endIndex, result));
//         // }

//         // for (int threadIndex = 0; threadIndex < MAX_THREAD; threadIndex++) {
//         //     workers[threadIndex].join();
//         // }

//         // std::clock_t simulationEnd2 = std::clock();

//         // std::cout << "Base Tracer : " << static_cast<double>(simulationEnd1 - simulationStart) / CLOCKS_PER_SEC << "ms, Thread Tracer : " << static_cast<double>(simulationEnd2 - simulationEnd1) / CLOCKS_PER_SEC << std::endl;

//     }
// }

// long long int Simulation::SetReactionTimeNew(std::vector<bool> visitRunnable, int cycle) {
//     long long int startTime = 0;
//     long long int thisTime = 0;

//     // Check all runnables
//     for (auto &runnable : this->dag_->GetRunnables()) {
//         // Check runnable is reacted
//         if (visitRunnable[runnable->GetId()]) {

//             if (runnable->GetStatus() == 0) {
//                 startTime = runnable->executionTimes_[cycle].startTime;
//             }

//             // Check runnable is input or middle runnable
//             int tmp_cycle = 0;
//             long long int hyperPeriod = (thisTime / this->hyperPeriod_) ? ((thisTime / this->hyperPeriod_) * this->hyperPeriod_) : 0;

//             while (thisTime > runnable->executionTimes_[tmp_cycle].startTime + hyperPeriod) {
//                 ++tmp_cycle;

//                 if (tmp_cycle >= this->hyperPeriod_) {
//                     hyperPeriod += this->hyperPeriod_;
//                     tmp_cycle = 0;
//                 }
//             }

//             thisTime = runnable->executionTimes_[tmp_cycle].endTime + hyperPeriod;

//             if (runnable->GetStatus() == 1) {
//                 break;
//             }
//         }
//     }

//     return thisTime - startTime;
// }

// // ################################## Trace Execution Time : Individual Pathes ###################################

// void Simulation::

// /* Old-version tracer */

// // ################################ Trace Execution Time : Non-Recursion Version #################################

// void Simulation::CreateVisitedWorstCycleList() {
//     std::vector<VisitedInformation>().swap(this->visitedWorstCycleList_);
//     this->visitedWorstCycleList_.reserve(this->numberOfRunnables_);
    
//     for (auto &runnable : this->dag_->GetRunnables()) {
//         this->visitedWorstCycleList_.push_back({runnable, -1, -1, -1LL, -1LL});
//     }
// }

// void Simulation::InitializeVisitedWorstCycleList() {
//     for (auto &visitedInformation : this->visitedWorstCycleList_) {
//         visitedInformation.inputRunnableCycle = -1;
//         visitedInformation.worstCycle = -1;
//         visitedInformation.startTime = -1LL;
//         visitedInformation.endTime = -1LL;
//     }
// }

// void Simulation::SetProcessExecutionsList() {
//     this->InitializeProcessExecutions();

//     for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
//         // this->InitializeVisitedWorstCycleList();

//         auto inputRunnableIter = this->processExecutions_.find(inputRunnable->id_);
//         if (inputRunnableIter != this->processExecutions_.end()) {
//             int maxCycle = inputRunnable->GetMaxCycle();

//             // std::clock_t simulationStart = std::clock();

//             for (int cycle = 0; cycle < maxCycle; cycle++) {
//                 this->TraceTimeList(inputRunnable, cycle);
//                 this->SetExecutionList(inputRunnableIter, cycle);
//             }

//             // std::clock_t simulationEnd1 = std::clock();

//             // std::vector<std::future<void>> workers;
//             // workers.reserve(maxCycle);
//             // for (int cycle = 0; cycle < maxCycle; cycle++) {
//             //     workers.emplace_back(std::async(std::launch::async, &Simulation::SetReactionTimeThread, this, inputRunnable, cycle, inputRunnableIter));
//             // }

//             // for (int cycle = 0; cycle < maxCycle; cycle++) {
//             //     workers[cycle].get();
//             // }

//             // std::clock_t simulationEnd2 = std::clock();

//             // std::cout << "Base Tracer : " << static_cast<double>(simulationEnd1 - simulationStart) / CLOCKS_PER_SEC << "ms, Thread Tracer : " << static_cast<double>(simulationEnd2 - simulationEnd1) / CLOCKS_PER_SEC << "ms." << std::endl;

//         } else {
//             throw std::invalid_argument("[SetProcessExecutions] input Runnable is wrong");
//         }
//     }
// }

// void Simulation::TraceTimeList(std::shared_ptr<RUNNABLE> inputRunnable, int inputRunnableCycle) {
//     this->visitedWorstCycleList_[inputRunnable->id_].inputRunnableCycle = inputRunnableCycle;
//     this->visitedWorstCycleList_[inputRunnable->id_].worstCycle = inputRunnableCycle;
//     this->visitedWorstCycleList_[inputRunnable->id_].startTime = this->visitedWorstCycleList_[inputRunnable->id_].runnable->executionTimes_[inputRunnableCycle].startTime;
//     this->visitedWorstCycleList_[inputRunnable->id_].endTime = this->visitedWorstCycleList_[inputRunnable->id_].runnable->executionTimes_[inputRunnableCycle].endTime;

//     // Check all runnables
//     for (int runnableIndex = 0; runnableIndex < this->numberOfRunnables_; runnableIndex++) {

//         // Check runnable is reacted
//         if (this->visitedWorstCycleList_[runnableIndex].inputRunnableCycle == inputRunnableCycle) {
            
//             // Check runnable is input or middle runnable
//             if (this->visitedWorstCycleList_[runnableIndex].runnable->GetStatus() != 1) {
//                 long long int thisEndTime = this->visitedWorstCycleList_[runnableIndex].endTime;

//                 // Check next runnable
//                 for (auto &outputRunnable : this->visitedWorstCycleList_[runnableIndex].runnable->GetOutputRunnables()) {

//                     // Re-write cycle count
//                     if (thisEndTime > this->visitedWorstCycleList_[outputRunnable->id_].startTime) {
//                         // Set input runnable's cycle
//                         this->visitedWorstCycleList_[outputRunnable->id_].inputRunnableCycle = inputRunnableCycle;

//                         // Set worst cycle & start time
//                         do {
//                             this->visitedWorstCycleList_[outputRunnable->id_].worstCycle++;
//                             this->visitedWorstCycleList_[outputRunnable->id_].startTime = this->visitedWorstCycleList_[outputRunnable->id_].runnable->executionTimes_[this->visitedWorstCycleList_[outputRunnable->id_].worstCycle % this->visitedWorstCycleList_[outputRunnable->id_].runnable->GetMaxCycle()].startTime + (this->hyperPeriod_ * static_cast<long long int>((this->visitedWorstCycleList_[outputRunnable->id_].worstCycle / this->visitedWorstCycleList_[outputRunnable->id_].runnable->GetMaxCycle())));
//                         } while (thisEndTime > this->visitedWorstCycleList_[outputRunnable->id_].startTime);

//                         // Set end time
//                         this->visitedWorstCycleList_[outputRunnable->id_].endTime = this->visitedWorstCycleList_[outputRunnable->id_].runnable->executionTimes_[this->visitedWorstCycleList_[outputRunnable->id_].worstCycle % this->visitedWorstCycleList_[outputRunnable->id_].runnable->GetMaxCycle()].endTime + (this->hyperPeriod_ * static_cast<long long int>((this->visitedWorstCycleList_[outputRunnable->id_].worstCycle / this->visitedWorstCycleList_[outputRunnable->id_].runnable->GetMaxCycle())));
//                     }
//                 }
//             } else {
//                 continue;                
//             }
//         } else {
//             continue;
//         }
//     }
// }

// void Simulation::SetReactionTimeThread(std::shared_ptr<RUNNABLE> inputRunnable, int inputRunnableCycle, std::map<int, std::map<int, std::vector<RequiredTime>>>::iterator inputRunnableIter) {
//     std::vector<RequiredTime> tmpList(this->numberOfRunnables_, {-1, -1});
//     tmpList[inputRunnable->GetId()].startTime = inputRunnable->executionTimes_[inputRunnableCycle].startTime;
//     tmpList[inputRunnable->GetId()].endTime = inputRunnable->executionTimes_[inputRunnableCycle].endTime;

//     // Check all runnables
//     for (auto runnable : this->dag_->GetRunnables()) {
//         // Check runnable is reacted
//         if (tmpList[runnable->GetId()].startTime > -1) {
//             long long int thisTime = tmpList[runnable->GetId()].endTime;

//             for (auto &outputRunnable : runnable->GetOutputRunnables()) {
//                 int tmp_cycle = 0;
//                 long long int hyperPeriod = (thisTime / this->hyperPeriod_) ? ((thisTime / this->hyperPeriod_) * this->hyperPeriod_) : 0;

//                 while (thisTime > outputRunnable->executionTimes_[tmp_cycle].startTime + hyperPeriod) {
//                     ++tmp_cycle;

//                     if (tmp_cycle >= this->hyperPeriod_) {
//                         hyperPeriod += this->hyperPeriod_;
//                         tmp_cycle = 0;
//                     }
//                 }

//                 tmpList[inputRunnable->GetId()].startTime = outputRunnable->executionTimes_[tmp_cycle].startTime + hyperPeriod;
//                 tmpList[inputRunnable->GetId()].endTime = outputRunnable->executionTimes_[tmp_cycle].endTime + hyperPeriod;
//             }
//         }
//     }

//     for (auto &outputRunnablePair : inputRunnableIter->second) {
//         outputRunnablePair.second[inputRunnableCycle].startTime = tmpList[inputRunnableIter->first].startTime;
//         outputRunnablePair.second[inputRunnableCycle].endTime = tmpList[outputRunnablePair.first].endTime;
//     }
// }

// void Simulation::SetExecutionList(auto& inputRunnableIter, int inputRunnableCycle) {
//     for (auto &outputRunnablePair : inputRunnableIter->second) {
//         outputRunnablePair.second[inputRunnableCycle].startTime = this->visitedWorstCycleList_[inputRunnableIter->first].startTime;
//         outputRunnablePair.second[inputRunnableCycle].endTime = this->visitedWorstCycleList_[outputRunnablePair.first].endTime;
//     }
// }

// // ################################ Trace Execution Time : Recursion Version #################################

// void Simulation::CreateVisitedWorstCycle() {
//     std::vector<int>(this->numberOfRunnables_, -1).swap(this->visitedWorstCycle_);
// }

// void Simulation::InitializeVisitedWorstCycle() {
//     std::fill(this->visitedWorstCycle_.begin(), this->visitedWorstCycle_.end(), -1);
// }

// void Simulation::SetProcessExecutions() {
//     this->InitializeProcessExecutions();

//     for (auto &inputRunnable : this->dag_->GetInputRunnables()) {
//         this->InitializeVisitedWorstCycle();

//         auto inputRunnableIter = this->processExecutions_.find(inputRunnable->id_);
//         if (inputRunnableIter != this->processExecutions_.end()) {
//             int maxCycle = inputRunnable->GetMaxCycle();

//             for (int cycle = 0; cycle < maxCycle; cycle++) {
//                 this->TraceTime(inputRunnableIter, cycle, inputRunnable, cycle);
//             }
//         } else {
//             throw std::invalid_argument("[SetProcessExecutions] input Runnable is wrong");
//         }
//     }
// }

// void Simulation::TraceTime(auto& inputRunnableIter, int inputCycle, const std::shared_ptr<RUNNABLE>& thisRunnable, int thisCycle) {
//     if (this->visitedWorstCycle_[thisRunnable->id_] < thisCycle) {
//         this->visitedWorstCycle_[thisRunnable->id_] = thisCycle;

//         if (thisRunnable->GetStatus() != 1) {
//             for (auto &outputRunnable : thisRunnable->GetOutputRunnables()) {
//                 int thisBaseCycle = thisCycle % thisRunnable->GetMaxCycle();
//                 int thisHyperPeriodCount = thisCycle / thisRunnable->GetMaxCycle();
//                 int outputMaxCycle = outputRunnable->GetMaxCycle();
//                 int outputCycle = (this->visitedWorstCycle_[outputRunnable->id_] != -1) ? this->visitedWorstCycle_[outputRunnable->id_] : 0;

//                 while (thisRunnable->executionTimes_[thisBaseCycle].endTime >= outputRunnable->executionTimes_[outputCycle % outputMaxCycle].startTime) {
//                     ++outputCycle;

//                     if (thisHyperPeriodCount < (outputCycle / outputMaxCycle)) {
//                         break;
//                     }
//                 }

//                 this->TraceTime(inputRunnableIter, inputCycle, outputRunnable, outputCycle);
//             }
//         } else {
//             int thisMaxCycle = thisRunnable->GetMaxCycle();
//             auto outputRunnableIter = inputRunnableIter->second.find(thisRunnable->id_);

//             outputRunnableIter->second[inputCycle].startTime = this->dag_->GetRunnable(inputRunnableIter->first)->executionTimes_[inputCycle].startTime;
//             outputRunnableIter->second[inputCycle].endTime = thisRunnable->executionTimes_[thisCycle % thisMaxCycle].endTime + (this->hyperPeriod_ * static_cast<long long int>(thisCycle / thisMaxCycle));
//         }
//     } else {
//         for (auto &outputRunnablePair : inputRunnableIter->second) {
//             if (inputCycle > 0) {
//                 if (outputRunnablePair.second[inputCycle].endTime < outputRunnablePair.second[inputCycle - 1].endTime) {
//                     outputRunnablePair.second[inputCycle].startTime = this->dag_->GetRunnable(inputRunnableIter->first)->executionTimes_[inputCycle].startTime;
//                     outputRunnablePair.second[inputCycle].endTime = outputRunnablePair.second[inputCycle - 1].endTime;
//                 }
//             }
//         }
//     }
// }

// // ################################ Get Result #################################

// void Simulation::SetResultMap() {
//     ResultInformation initalizedResultInformation = {0, 0ll, 0ll};

//     for (auto &inputRunnablePair : this->processExecutions_) {
//         auto resultInputRunnablePair = this->results_.find(inputRunnablePair.first);

//         if (resultInputRunnablePair == this->results_.end()) {
//             this->results_.insert({inputRunnablePair.first, std::map<int, ResultInformation>()});
//             resultInputRunnablePair = this->results_.find(inputRunnablePair.first);
//         }

//         for (auto &outputRunnablePair : inputRunnablePair.second) {
//             auto resultOutputRunnablePair = resultInputRunnablePair->second.find(outputRunnablePair.first);

//             if (resultOutputRunnablePair != resultInputRunnablePair->second.end()) {
//                 resultOutputRunnablePair->second.seedNumber = 0;
//                 resultOutputRunnablePair->second.reactionTime = 0ll;
//                 resultOutputRunnablePair->second.dataAge = 0ll;
//             } else {
//                 resultInputRunnablePair->second.insert({outputRunnablePair.first, initalizedResultInformation});
//             }
//         }
//     }
// }

// void Simulation::InitResultMap() {
//     for (auto &inputRunnablePair : this->results_) {
//         for (auto &outputRunnablePair : inputRunnablePair.second) {
//             outputRunnablePair.second.seedNumber = 0;
//             outputRunnablePair.second.reactionTime = 0ll;
//             outputRunnablePair.second.dataAge = 0ll;
//         }
//     }
// }

// void Simulation::GetWorstReactionTime() {
//     long long int tmpEndTime = -1ll;
//     int maxCycle = -1;

//     for (auto &inputRunnablePair : this->processExecutions_) {
//         auto resultInputRunnablePair = this->results_.find(inputRunnablePair.first);

//         for (auto &outputRunnablePair : inputRunnablePair.second) {
//             auto resultOutputRunnablePair = resultInputRunnablePair->second.find(outputRunnablePair.first);
//             maxCycle = static_cast<int>(outputRunnablePair.second.size());
//             tmpEndTime = outputRunnablePair.second[maxCycle - 1].endTime;

//             if (resultOutputRunnablePair->second.reactionTime < (outputRunnablePair.second[maxCycle - 1].endTime - outputRunnablePair.second[maxCycle - 1].startTime)) {
//                 resultOutputRunnablePair->second.reactionTime = outputRunnablePair.second[maxCycle - 1].endTime - outputRunnablePair.second[maxCycle - 1].startTime;
//             }

//             for (int index = maxCycle - 1; index >= 0; --index) {
//                 if (tmpEndTime > outputRunnablePair.second[index].endTime) {
//                     tmpEndTime = outputRunnablePair.second[index].endTime;
                    
//                     if (resultOutputRunnablePair->second.reactionTime < (outputRunnablePair.second[index].endTime - outputRunnablePair.second[index].startTime)) {
//                         resultOutputRunnablePair->second.reactionTime = outputRunnablePair.second[index].endTime - outputRunnablePair.second[index].startTime;
//                     }
//                 }
//             }
//         }
//     }
// }

// void Simulation::GetWorstDataAge() {
//     long long int tmpEndTime = -1ll;
//     long long int previousEndTime = -1ll;
//     long long int currentEndTime = -1ll;
//     int maxCycle = -1;

//     for (auto &inputRunnablePair : this->processExecutions_) {
//         std::cerr << "ckpt1\n";
//         auto resultInputRunnablePair = this->results_.find(inputRunnablePair.first);

//         for (auto &outputRunnablePair : inputRunnablePair.second) {
//             std::cerr << "ckpt2\n";
//             auto resultOutputRunnablePair = resultInputRunnablePair->second.find(outputRunnablePair.first);
//             tmpEndTime = outputRunnablePair.second[maxCycle - 1].endTime;
//             long long int tmpDataAge = 0ll;
//             int tmpCycle = 0;
//             maxCycle = static_cast<int>(outputRunnablePair.second.size());
//             std::shared_ptr<RUNNABLE> outputRunnable = this->dag_->GetRunnable(outputRunnablePair.first);

//             std::cerr << "ckpt3\n";
//             if (resultOutputRunnablePair->second.dataAge < (outputRunnablePair.second[maxCycle - 1].endTime - outputRunnablePair.second[maxCycle - 1].startTime)) {
//                 resultOutputRunnablePair->second.dataAge = outputRunnablePair.second[maxCycle - 1].endTime - outputRunnablePair.second[maxCycle - 1].startTime;
//             }

//             for (int index = maxCycle - 1; index > 0; --index) {
//                 if (tmpEndTime > outputRunnablePair.second[index].endTime) {
//                     std::cerr << "ckpt4\n";
//                     tmpEndTime = outputRunnablePair.second[index].endTime;
//                     previousEndTime = outputRunnablePair.second[index - 1].endTime % this->hyperPeriod_;
//                     currentEndTime = outputRunnablePair.second[index].endTime % this->hyperPeriod_;

//                     std::cerr << "ckpt5\n";
//                     tmpCycle = 0;
//                     while (currentEndTime != outputRunnable->executionTimes_[tmpCycle].endTime) ++tmpCycle;
//                     if (tmpCycle == 0) tmpCycle = maxCycle;

//                     std::cerr << "ckpt6\n";
//                     tmpDataAge = (outputRunnablePair.second[index - 1].endTime - outputRunnablePair.second[index - 1].startTime) + (outputRunnable->executionTimes_[tmpCycle - 1].endTime - previousEndTime);

//                     std::cerr << "ckpt7\n";
//                     if (currentEndTime < previousEndTime) tmpDataAge += this->hyperPeriod_;

//                     if (resultOutputRunnablePair->second.dataAge < tmpDataAge) {
//                         resultOutputRunnablePair->second.dataAge = tmpDataAge;
//                     }
//                 }
//             }
//         }
//     }
// }

// // ################################ Save Data : CSV #################################

// // Save data in one iteration
// void Simulation::SaveDataToCSV(int seedNumber) {
//     std::string fileDirectory = this->dataDirectory_ + "/Result.csv";

//     std::ofstream resultFile;
//     resultFile.open (fileDirectory.c_str(), std::ios_base::out | std::ios_base::app);

//     for (auto &inputRunnablePair : this->results_) {
//         for (auto &outputRunnablePair : inputRunnablePair.second) {
//             resultFile << seedNumber << ",";
//             resultFile << inputRunnablePair.first << ",";
//             resultFile << outputRunnablePair.first << ",";
//             resultFile << static_cast<double>(outputRunnablePair.second.reactionTime / 1000) + (static_cast<double>(outputRunnablePair.second.reactionTime % 1000) / 1000.0) << ",";
//             resultFile << static_cast<double>(outputRunnablePair.second.dataAge / 1000) + (static_cast<double>(outputRunnablePair.second.dataAge % 1000) / 1000.0) << "\n";
//         }
//     }

//     resultFile.close();
// }

// Save data after all simulation
// void Simulation::SaveAllDataToCSV() {
//     std::string fileDirectory = this->dataDirectory_ + "/Result.csv";

//     std::sort(this->results_.begin(), this->results_.end(), [](ResultInformation &a, ResultInformation &b) { return a.reactionTime < b.reactionTime; });

//     std::ofstream resultFile;
//     resultFile.open (fileDirectory.c_str(), std::ios_base::out | std::ios_base::app);

//     for (auto &result : this->results_) {
//         resultFile << result.seedNumber << "," << static_cast<double>(result.reactionTime / 1000) + (static_cast<double>(result.reactionTime % 1000) / 1000.0) << "," << static_cast<double>(result.dataAge / 1000) + (static_cast<double>(result.dataAge % 1000) / 1000.0) << "\n";
//     }

//     resultFile.close();
// }

// ################################ Save Data : JSON #################################

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

// ################################ Display Data on Console #################################

void Simulation::DisplayDag() {
    // std::cout << "\033[H\033[2J\033[3J";
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
}

// void Simulation::DisplayResult(int seedNumber, double processTime, int limitProcessTime) {
//     // std::cout << "\033[H\033[2J\033[3J";
//     std::cout << "===========================================================================================================================\n";
//     std::cout << " - Simulation Case            : " << std::setw(10) << (this->GetNumberOfCase() - this->GetNumberOfRemainedCase()) << " / " << std::setw(10) << this->GetNumberOfCase() << "\n";
//     std::cout << " - Simulation Seed            : " << std::setw(23) << seedNumber << "\n";
// //    std::cout << " - Reaction Time              : " << std::setw(16) << result.reactionTime / 1000 << "." << std::setw(3) << result.reactionTime % 1000 << " ms\n";
// //    std::cout << " - Data Age                   : " << std::setw(20) << static_cast<double>(result.dataAge) / 1000.0 << " ms\n";
//     std::cout << " - Max Cycle                  : " << std::setw(17) << this->maxCycle_ << " cycle\n";
//     std::cout << " - Hyper Period               : " << std::setw(20) << static_cast<double>(this->hyperPeriod_) / 1000.0 << " ms\n";
//     std::cout << " - Number Of Tasks            : " << std::setw(23) << this->numberOfTasks_ << "\n";
//     std::cout << " - Number Of Runanbles        : " << std::setw(23) << this->numberOfRunnables_ << "\n";
//     std::cout << " - Number Of Input Runnables  : " << std::setw(23) << this->numberOfInputRunnables_ << "\n";
//     std::cout << " - Number Of Output Runnables : " << std::setw(23) << this->numberOfOutputRunnables_ << "\n";
//     std::cout << " - Utilization                : " << std::setw(23) << this->utilization_ << "\n";
//     std::cout << " - Utilization Bound          : " << std::setw(23) << this->utilizationBound_ << "\n";
//     std::cout << " - Simulation Process Time    : " << std::setw(10) << processTime << " / " << std::setw(8) << limitProcessTime << " s\n";
//     std::cout << "===========================================================================================================================" << std::endl;
// }