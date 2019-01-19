//
// Created by jskalska on 19/01/2019.
//
#pragma once

class Ant {
public:

    int curCity, nextCity, pathIndex;
    int* tabu;
    int* path;
    int tourLength;

    explicit Ant(int numberOfCities);
    virtual ~Ant();
};
