#include "sequence.hpp"


void Sequence::InitializeVisitedPermutationNumber() {
    std::vector<bool>(this->GetNumberOfCase(), false).swap(this->visitedPermutationNumber_);
}

int Sequence::GetNumberOfCase() {
    if (this->numberOfCase_ == -1) {
        int numberOfCase = 1;

        for (auto &taskPair : this->sequenceMatrix_) {
            for (auto &runnablesWithSamePrecedence : taskPair.second) {
                numberOfCase *= static_cast<int>(std::tgamma(static_cast<int>(runnablesWithSamePrecedence.size()) + 1));
            }
        }

        this->numberOfCase_ = numberOfCase;
        this->numberOfRemainedCase_ = numberOfCase;
    }

    return this->numberOfCase_;
}

int Sequence::GetRandomEmptyIndex() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, this->GetNumberOfRemainedCase() - 1);

    int simulationSeed = dis(gen);
    int simulationIndex = -1;

    for (int tmpSimulationIndex = 0; tmpSimulationIndex <= simulationSeed; tmpSimulationIndex++) {
        while (this->visitedPermutationNumber_[++simulationIndex]);
    }

    return simulationIndex;
}

void Sequence::SetSequence(int caseIndex) {
    this->visitedPermutationNumber_[caseIndex] = true;

    for (auto& [task, runnableMatrix] : this->sequenceMatrix_) { // 원본은 바뀌면 안된다.
        std::vector<std::shared_ptr<RUNNABLE>> sequence;
        sequence.reserve(task->GetNumberOfRunnables());

        for (auto runnablesWithSamePrecedence : runnableMatrix) {
            int numberOfPermutation = static_cast<int>(std::tgamma(static_cast<int>(runnablesWithSamePrecedence.size()) + 1));
            int permutationNumber = caseIndex % numberOfPermutation;
            caseIndex /= numberOfPermutation;

            for (int index = 0; index < permutationNumber; index++) {
                std::next_permutation(runnablesWithSamePrecedence.begin(), runnablesWithSamePrecedence.end(), [](std::shared_ptr<RUNNABLE>& a, std::shared_ptr<RUNNABLE>& b) { return a->id_ < b->id_; });
            }

            sequence.insert(sequence.end(), runnablesWithSamePrecedence.begin(), runnablesWithSamePrecedence.end());
        }

        task->SetSequence(sequence);
    }

    // Reducing remained case count
    this->numberOfRemainedCase_ -= 1;
}

void SequenceByPrecedence::SetSequenceMatrix() {
    std::map<std::shared_ptr<TASK>, std::vector<std::vector<std::shared_ptr<RUNNABLE>>>> sequenceMatrix;

    for (auto &task : this->dag_->GetTasksInPriority()) {
        task->SortRunnablesByPrecedence();
        auto sequenceMatrixIterPair = sequenceMatrix.insert({task, std::vector<std::vector<std::shared_ptr<RUNNABLE>>>()});

        int currentPrecedence = -2; // '-1' is initialization number of precedence
        for (auto &runnable : task->GetRunnables()) {
            if (currentPrecedence == runnable->GetPrecedence()) {
                sequenceMatrixIterPair.first->second.back().emplace_back(runnable);
            } else {
                sequenceMatrixIterPair.first->second.emplace_back(std::vector<std::shared_ptr<RUNNABLE>>(1, runnable));
            }
            currentPrecedence = runnable->GetPrecedence();
        }
    }

    sequenceMatrix.swap(this->sequenceMatrix_);
}

void SequenceByAllcase::SetSequenceMatrix() {
    std::map<std::shared_ptr<TASK>, std::vector<std::vector<std::shared_ptr<RUNNABLE>>>> sequenceMatrix;

    for (auto &task : this->dag_->GetTasksInPriority()) {
        task->SortRunnablesByPrecedence();
        sequenceMatrix.insert({task, std::vector<std::vector<std::shared_ptr<RUNNABLE>>>(1, task->GetRunnables())});
    }

    sequenceMatrix.swap(this->sequenceMatrix_);
}

void OptimizeCase::SetSequence(int caseIndex) {
    for (auto &task : this->dag_->GetTasksInPriority()) {
        long long int targetPeriod = task->GetPeriod();

        for (auto& runnable : task->GetRunnables()) {
            // Receive
            float receiveInterWeight = 0.0f;
            std::vector<std::shared_ptr<RUNNABLE>> sameTaskInputRunnables;

            for (auto inputRunnable : runnable->GetInputRunnables()) {
                if (task->GetId() != inputRunnable->GetTask()->GetId()) {
                    if (targetPeriod > inputRunnable->GetTask()->GetPeriod()) {
                        float tmpInterWeight = static_cast<float>(targetPeriod) / static_cast<float>(inputRunnable->GetTask()->GetPeriod());
                        if (receiveInterWeight < tmpInterWeight) {
                            receiveInterWeight = tmpInterWeight;
                        }
                    }
                } else { 
                    sameTaskInputRunnables.push_back(inputRunnable);
                }
            }

            for (auto inputRunnable : sameTaskInputRunnables) {
                receiveInterWeight = std::max(receiveInterWeight, inputRunnable->GetReceiveInterWeight());
            }

            runnable->SetReceiveInterWeight(receiveInterWeight);

            // Send
            float sendInterWeight = 0.0f;
            std::vector<std::shared_ptr<RUNNABLE>> sameTaskOutputRunnables;

            for (auto outputRunnable : runnable->GetOutputRunnables()) {
                if (task->GetId() != outputRunnable->GetTask()->GetId()) {
                    if (targetPeriod > outputRunnable->GetTask()->GetPeriod()) {
                        float tmpInterWeight = static_cast<float>(outputRunnable->GetTask()->GetPeriod()) / static_cast<float>(targetPeriod);
                        if (sendInterWeight < tmpInterWeight) {
                            sendInterWeight = tmpInterWeight;
                        }
                    }
                } else {
                    sameTaskOutputRunnables.push_back(outputRunnable);
                }
            }

            for (auto outputRunnable : sameTaskOutputRunnables) {
                sendInterWeight = std::max(sendInterWeight, outputRunnable->GetSendInterWeight());
            }

            runnable->SetSendInterWeight(sendInterWeight);
        }

        std::vector<std::shared_ptr<RUNNABLE>> sequence = task->CopyRunnables();
        std::sort(sequence.begin(), sequence.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b){ return (a->GetInterWeight() != b->GetInterWeight()) ? a->GetInterWeight() > b->GetInterWeight() : a->GetPrecedence() < b->GetPrecedence(); });

        task->SetSequence(sequence);
    }

    // Reducing remained case count
    this->numberOfRemainedCase_ -= 1;
}

void OneCase::SetSequence(int caseIndex) {
    std::cerr << "[Sequence] ckpt0\n";
    for (auto &task : this->dag_->GetTasksInPriority()) {
        std::cerr << "[Sequence] ckpt1\n";
        task->SortRunnablesByPrecedence();
        std::cerr << "[Sequence] ckpt2\n";
        auto sequence = task->GetRunnables();
        std::cerr << "[Sequence] ckpt3\n";

        task->SetSequence(sequence);
        std::cerr << "[Sequence] ckpt4\n";
    }

    // Reducing remained case count
    this->numberOfRemainedCase_ -= 1;
}