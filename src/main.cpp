//
// Created by jskalska on 08/12/2018.
//

#include <iostream>
#include <ctime>
#include <thread>
#include "GA.h"
#include "ACO.h"

void testMenu();


int main() {
    srand(time(NULL));

    TSP *ga = new GA();
    TSP *aco = new ACO();

    int chosen;
    std::string file_name;

    while (chosen != 5) {
        std::cout << "1. Load from file\n"
                     "2. Genetic Algorithm\n"
                     "3. Ant Colony Optimization Algorithm\n"
                     "4. Test\n"
                     "5. Exit.\n"
                     "Please enter the appropriate number: ";

        std::cin >> chosen;

        switch (chosen) {
            case 1:
                std::cout << "Please enter the file name: ";
                std::cin >> file_name;
                if (ga->loadFromFile(file_name) && aco->loadFromFile(file_name)) {
                    std::cout << "Loaded correctly!\n";
                }
                else std::cout << "The file could not be loaded!\n";
                break;
            case 2:
                ga->menu();
                break;
            case 3:
                aco->menu();
                break;
            case 4:
                testMenu();
            break;
            default:
                std::cin.clear();
                std::cin.sync();
                break;
        }
    }

    delete ga;
    delete aco;

    return 0;
}

void testMenu() {
    std::cout << "1. Genetic Algorithm\n"
                 "2. Ant Colony Optimization Algorithm\n"
                 "3. Exit\n";

    int chosen;
    std::string file_name;
    std::cin >> chosen;
    switch(chosen) {
        case 1:
//            testGA();
            break;
        case 2:
//            testACO();
            break;
        case 3:
            return;
        default:
            std::cout << "Please enter the correct number!\n";
            std::cin.clear();
            std::cin.sync();
            break;
    }

    testMenu();
}