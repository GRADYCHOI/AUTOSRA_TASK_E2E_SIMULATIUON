#include "precedence.hpp"


void InputToOutputPrecedence::SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) {
    int maxPrecedence = -1;

    std::clog << "\033[H\033[2J\033[3J";
	std::clog << "===============================================[Debug : Runnable Precedence]===============================================" << std::endl;

    for (auto &runnable : runnables) {
        std::clog << " Entire Runnable ID : " << inputRunnable->id_ << std::endl;
		runnable->SetPrecedence(-1);
	}

    for (auto &inputRunnable : inputRunnables) {
        std::clog << " Input Runnable ID : " << inputRunnable->id_ << std::endl;
		this->CheckPrecedence(inputRunnable, 0);
        std::cout << "[Runnable Precedence]" << std::endl;
	}

    this->SetInputOutputRunnablePrecedence(std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables);
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void OutputToInputPrecedence::SetPrecedence(std::vector<RUNNABLE> inputRunnables, int numberOfRunnables) {

}

void InputToOutputPrecedence::CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, const int precedence) {
    std::clog << "============================================[Debug : Runnable Check Precedence]============================================" << "\n";
    std::clog << " Runnable ID : " << runnable->id_ << std::endl;

    if (precedenceOfRunnables[runnable->id_] < precedence) {
		precedenceOfRunnables[runnable->id_] = precedence;
	}
	
	if (runnable->GetStatus() != 0) {
        for (auto &inputRunnable : runnable->GetInputRunnables()) {
            if (precedenceOfRunnables[inputRunnable->id_] < ++precedence) {
				CheckPrecedence(precedenceOfRunnables, inputRunnable, precedence);
			}
        }
    }
}

void OutputToInputPrecedence::CheckPrecedence(std::vector<int>& precedenceOfRunnables, const std::shared_ptr<RUNNABLE>& runnable, int precedence) {
    std::clog << "============================================[Debug : Runnable Check Precedence]============================================" << "\n";
    std::clog << " Runnable ID : " << runnable->id_ << std::endl;

    if (precedenceOfRunnables[runnable->id_] < precedence) {
		precedenceOfRunnables[runnable->id_] = precedence;
	}
	
	if (runnable->GetStatus() != 1) {
        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            if (precedenceOfRunnables[outputRunnable->id_] < ++precedence) {
				CheckPrecedenceInput(precedenceOfRunnables, outputRunnable, precedence);
			}
        }
    }
}

void Precedence::SetInputOutputRunnablePrecedence(std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) {
    int maxPrecedence;

    for (auto &inputRunnable : inputRunnables) {
        maxPrecedence = (maxPrecedence > inputRunnable->GetPrecedence()) ? maxPrecedence : inputRunnable->GetPrecedence();
    }

    for (auto &outputRunnable : outputRunnables) {
        maxPrecedence = (maxPrecedence > outputRunnable->GetPrecedence()) ? maxPrecedence : outputRunnable->GetPrecedence();
    }

    int strategyNumber;

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** What do you want precedence strategy? ***" << "\n";
    std::cout << " 0 : Input Runnables  - Middle Runnalbes - Output Runnables" << "\n";
    std::cout << " 1 : Output Runnables - Middle Runnalbes - Input Runnables" << "\n";
    std::cout << "\n" << "Enter Number : ";
    std::cin >> strategyNumber;

    switch (strategyNumber) {
        case 0 : {
            for (auto &inputRunnable : inputRunnables) {
                inputRunnable->SetPrecedence(0);
            }

            for (auto &outputRunnable : outputRunnables) {
                outputRunnable->SetPrecedence(maxPrecedence);
            }

            break;
        }

        case 1 : {
            for (auto &inputRunnable : inputRunnables) {
                inputRunnable->SetPrecedence(maxPrecedence);
            }

            for (auto &outputRunnable : outputRunnables) {
                outputRunnable->SetPrecedence(0);
            }
            
            break;
        }

        default : {
            std::cout << "Wrong Precedence Strategy." << std::endl;
            return 0;
        }
    }
}