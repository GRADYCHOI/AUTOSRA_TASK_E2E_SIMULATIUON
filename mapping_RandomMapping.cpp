#include "mapping.hpp"
#include <cstdlib>


class RandomMapping : public Mapping {
public:
    void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
        for (auto &runnable : runnables) {
            tasks[std::rand() % static_cast<int>(tasks.size())]->AddRunnable(runnable);
        }
    }
};