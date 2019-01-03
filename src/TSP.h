//
// Created by jskalska on 09/12/2018.
//

#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>
#include "../include/GraphMatrix.h"

class TSP {
public:
    enum neighbourhood {
        SWAP = 0,
        INSERT = 1,
        INVERT = 2
    };

    virtual ~TSP(){};

    bool searchInFile(std::ifstream &fin, std::string toFind);
    bool loadFromFile(std::string filename);
    virtual void menu() = 0;

protected:
    GraphMatrix gm;
    int numberOfCities;
};
