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

void OneCase::SetSequence(int caseIndex) {
    for (auto &task : this->dag_->GetTasksInPriority()) {
        task->SortRunnablesByPrecedence();
        task->SetSequence(task->GetRunnables());
    }

    // Reducing remained case count
    this->numberOfRemainedCase_ -= 1;
}