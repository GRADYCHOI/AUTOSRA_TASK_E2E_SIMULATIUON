#include "precedence.hpp"


void InputToOutputPrecedence::SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) {
    int maxPrecedence = -1;

    std::clog << "\033[H\033[2J\033[3J";
	std::clog << "===============================================[Debug : Runnable Precedence]===============================================" << std::endl;

    for (auto &runnable : runnables) {
        std::clog << " Entire Runnable ID : " << runnable->id_ << std::endl;
		runnable->SetPrecedence(-1);
	}

    for (auto &inputRunnable : inputRunnables) {
        std::clog << " Input Runnable ID : " << inputRunnable->id_ << std::endl;
		this->CheckPrecedence(inputRunnable, 0);
        std::cout << "[Runnable Precedence]" << std::endl;
	}

    this->SetInputOutputRunnablePrecedence(inputRunnables, outputRunnables);
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void OutputToInputPrecedence::SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) {
    int maxPrecedence = -1;

    std::clog << "\033[H\033[2J\033[3J";
	std::clog << "===============================================[Debug : Runnable Precedence]===============================================" << std::endl;

    for (auto &runnable : runnables) {
        std::clog << " Entire Runnable ID : " << runnable->id_ << std::endl;
		runnable->SetPrecedence(-1);
	}

    for (auto &outputRunnable : outputRunnables) {
        std::clog << " Input Runnable ID : " << outputRunnable->id_ << std::endl;
		this->CheckPrecedence(outputRunnable, 0);
        std::cout << "[Runnable Precedence]" << std::endl;
	}

    this->SetInputOutputRunnablePrecedence(inputRunnables, outputRunnables);
	
	std::clog << "===========================================================================================================================" << std::endl;
}

void InputToOutputPrecedence::CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, int precedence) {
	runnable->SetPrecedence(precedence);
	
	if (runnable->GetStatus() != 1) {
        for (auto &outputRunnable : runnable->GetOutputRunnables()) {
            if (outputRunnable->GetPrecedence() < precedence) {
				CheckPrecedence(outputRunnable, precedence);
			}
        }
    }
}

void OutputToInputPrecedence::CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, int precedence) {
	runnable->SetPrecedence(precedence);
    precedence++;
	
	if (runnable->GetStatus() != 0) {
        for (auto &inputRunnable : runnable->GetInputRunnables()) {
            if (inputRunnable->GetPrecedence() < precedence) {
				CheckPrecedence(inputRunnable, precedence);
			}
        }
    }
}

void Precedence::SetInputOutputRunnablePrecedence(std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) {
    int maxPrecedence = INT_MIN;
    for (auto &inputRunnable : inputRunnables) {
        maxPrecedence = (maxPrecedence > inputRunnable->GetPrecedence()) ? maxPrecedence : inputRunnable->GetPrecedence();
    }

    maxPrecedence = INT_MIN;
    for (auto &outputRunnable : outputRunnables) {
        maxPrecedence = (maxPrecedence > outputRunnable->GetPrecedence()) ? maxPrecedence : outputRunnable->GetPrecedence();
    }

    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** What do you want precedence strategy? ***" << "\n";
    std::cout << " 0 : Input Runnables  - Middle Runnalbes - Output Runnables" << "\n";
    std::cout << " 1 : Output Runnables - Middle Runnalbes - Input Runnables" << "\n";
    std::cout << "\n" << "Enter Number : ";
    std::cin >> precedencePositionStrategy;

    switch (precedencePositionStrategy) {
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