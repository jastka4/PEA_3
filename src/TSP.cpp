//
// Created by jskalska on 09/12/2018.
//

#include "TSP.h"

bool TSP::searchInFile(std::ifstream &inputFile, std::string toFind){
    std::string tmp;
    std::string toFind2;
    if (toFind.substr(toFind.size()-1,1)==":"){
        toFind2 = toFind.substr(0,toFind.size()-1);
    }
    int check = 20;
    do {
        inputFile >> tmp;
        check--;
    }
    while (tmp != toFind && tmp != toFind2  && check > 0);
    if (check == 0) return false;
    return true;
}

bool TSP::loadFromFile(std::string filename) {
    std::ifstream inputFile;
    inputFile.open("../data/input/" + filename);
    if (filename.find(".atsp")!=std::string::npos){ //atsp
        std::string tmp;
        if (!searchInFile(inputFile,"DIMENSION:")) return false;

        inputFile >> numberOfCities;
        gm.createMatrix(numberOfCities);

        if (!searchInFile(inputFile,"EDGE_WEIGHT_TYPE:")) return false;
        if (!searchInFile(inputFile,"EXPLICIT")) return false;
        if (!searchInFile(inputFile,"EDGE_WEIGHT_FORMAT:")) return false;
        if (!searchInFile(inputFile,"FULL_MATRIX")) return false;

        if (!searchInFile(inputFile,"EDGE_WEIGHT_SECTION")) return false;
        for (int i = 0; i < numberOfCities; i++){
            for (int j = 0; j < numberOfCities; j++){
                int length;
                inputFile >> length;
                gm.setEdge(i,j,length);
            }
        }
        return true;
    }
    else if (filename.find(".tsp")!=std::string::npos) { //tsp
        std::string tmp;
        if (!searchInFile(inputFile, "DIMENSION:")) return false;

        inputFile >> numberOfCities;
        gm.createMatrix(numberOfCities);

        if (!searchInFile(inputFile, "EDGE_WEIGHT_TYPE:")) return false;
        inputFile >> tmp;
        if (tmp == ":") inputFile >> tmp;
        if (tmp == "EXPLICIT") {
            if (!searchInFile(inputFile, "EDGE_WEIGHT_FORMAT:")) return false;
            inputFile >> tmp;
            if (tmp.find("LOWER_DIAG_ROW") != std::string::npos) {
                if (!searchInFile(inputFile, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = 0; j <= i; j++) {
                        int length;
                        inputFile >> length;
                        gm.setEdge(i, j, length);
                        gm.setEdge(j, i, length);
                    }
                }
                return true;
            } else if (tmp.find("FULL_MATRIX") != std::string::npos) {
                if (!searchInFile(inputFile, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = 0; j < numberOfCities; j++) {
                        int length;
                        inputFile >> length;
                        gm.setEdge(i, j, length);
                    }
                }
                return true;
            } else if (tmp.find("UPPER_DIAG_ROW") != std::string::npos) {
                if (!searchInFile(inputFile, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = i; j < numberOfCities; j++) {
                        int length;
                        inputFile >> length;
                        gm.setEdge(i, j, length);
                        gm.setEdge(j, i, length);
                    }
                }
                return true;
            } else return false;
        } else if (tmp == "EUC_2D") {
            if (!searchInFile(inputFile, "NODE_COORD_SECTION")) return false;

            double **eucCoord = new double *[numberOfCities];
            for (int i = 0; i < numberOfCities; i++) {
                eucCoord[i] = new double[2];
                int n;
                inputFile >> n;
                inputFile >> eucCoord[i][0] >> eucCoord[i][1];
            }

            for (int i = 0; i < numberOfCities; i++) {
                for (int j = 0; j <= i; j++) {
                    double xDistance = eucCoord[i][0] - eucCoord[j][0];
                    double yDistance = eucCoord[i][1] - eucCoord[j][1];
                    int distance = (int) sqrt(xDistance * xDistance + yDistance * yDistance);
                    gm.setEdge(i, j, distance);
                    gm.setEdge(j, i, distance);
                }
            }
            delete[] eucCoord;
            return true;
        }
    }
    else if (filename.find(".txt")!=std::string::npos){
        inputFile >> numberOfCities;
        gm.createMatrix(numberOfCities);
        for (int i = 0; i < numberOfCities; i++){
            for (int j = 0; j < numberOfCities; j++){
                int length;
                inputFile >> length;
                //if (length == -1) length = 0;
                gm.setEdge(i,j,length);
            }
        }
        return true;
    }
    return false;
}
