void Sequence::SetSequenceMatrix() {
    std::vector<std::vector<std::vector<std::shared_ptr<RUNNABLE>>>> sequenceMatrix;

    int taskIndex = 0;
    for (auto &task : this->dag_->GetTasks()) {
        int currentPrecedence = -2;
        task->SortRunnablesByPrecedence();
        sequenceMatrix.emplace_back(std::vector<std::vector<std::shared_ptr<RUNNABLE>>>());

        int precedenceIndex = -1;
        for (auto &runnable : task->GetRunnables()) {
            if (currentPrecedence == runnable->GetPrecedence()) {
                sequenceMatrix[taskIndex][precedenceIndex].emplace_back(runnable);
            } else {
                sequenceMatrix[taskIndex].emplace_back(std::vector<std::shared_ptr<RUNNABLE>>(1, runnable));
                precedenceIndex += 1;
            }
            currentPrecedence = runnable->GetPrecedence();
        }

        taskIndex += 1;
    }

    sequenceMatrix.swap(this->sequenceMatrix_);
}

void Sequence::SetSequence(int caseIndex) {
    this->visitedPermutationNumber_[caseIndex] = true;

    for (auto runnableMatrix : this->sequenceMatrix_) { // 원본은 바뀌면 안된다.
        for (auto &runnablesWithSamePrecedence : runnableMatrix) {
            int permutationNumber = caseIndex % static_cast<int>(runnablesWithSamePrecedence.size());
            caseIndex /= static_cast<int>(runnablesWithSamePrecedence.size());

            for (int index = 1; index < permutationNumber; index++) {
                std::next_permutation(runnablesWithSamePrecedence.begin(), runnablesWithSamePrecedence.end(), [](std::shared_ptr<RUNNABLE>& a, std::shared_ptr<RUNNABLE>& b) { return a->id_ < b->id_; });
            }
        }

        runnableMatrix[0][0]->GetTask()->SetSequence(runnableMatrix);
    }
}