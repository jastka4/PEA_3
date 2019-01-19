//
// Created by jskalska on 19/01/2019.
//
#pragma once

#include "TSP.h"
#include "Ant.h"

class ACO: public TSP {
    double alpha;
    double beta;    // this parameter raises the weight of distance over pheromone
    double evaporationRate; // evaporation rate
    int qval;
    int maxTours;
    int antsPopulation;
    std::vector<Ant*> ants;
    double** pheromones;
    double best;
    int bestIndex;

    void init();
    void restartAnts();
    double antProduct(int from, int to);
    int selectNextCity( int ant );
    int simulateAnts();
    void updateTrails();
public:
    ACO();

    void setNumberOfAnts(int antsPopulation);

    void menu() override;
    int getBest() const;
};
