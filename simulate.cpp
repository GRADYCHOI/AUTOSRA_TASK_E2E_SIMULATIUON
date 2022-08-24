void Simulate() {
    int option;

    std::cout << "[Simulation] Select Communication Option Number\n";
    std::cout << " - 0 : Runnable Implicit Communication\n";
    std::cout << " - 1 : Task Implicit Communication\n";
    std::cout << " - 2 : Logical Execution Time (LET)\n"
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Enter : ";
    std::cin >> option;

    switch (option) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            std::cout << "wrong number." << std::endl;
            break;
    }
}