//
// Created by jskalska on 30/12/2018.
//

#pragma once

#include "Graph.h"
#include "TSP.h"
#include <vector>
#include <map>
#include <set>
#include <utility> // pair
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <iostream>
#include <algorithm> // sort, next_permutation

typedef std::pair<std::vector<int>, int> my_pair;

// sort vector with pair
struct sort_pred
{
    bool operator()(const my_pair& firstElem, const my_pair& secondElem)
    {
        return firstElem.second < secondElem.second;
    }
};

class GA: public TSP {
private:
    std::vector< my_pair > population; // each element is a pair: vector and total cost
    int populationSize; // size of population
    int realPopulationSize; // real size population
    int generations; // amount of generations
    int mutationRate; // mutation rate
    int mutationMethod; // mutation method
    int crossoverRate; // crossover rate
    int crossoverMethod; // crossover method
    bool showPopulationEnabled; // flag to show population

    void initializePopulation(); // generates the initial population
    int isValidSolution(std::vector<int>& solution); // checks if a solution is valid
    void showPopulation(); // shows population
    void multiPointCrossoverWithInversion(std::vector<int>& parent1, std::vector<int>& parent2); // makes the crossover
    void partiallyMatchedCrossover(std::vector<int>& parent1, std::vector<int>& parent2); // makes the crossover
    void swapMutation(std::vector<int>& chromosome);
    void scrambleMutation(std::vector<int>& chromosome);
    void mutation(std::vector<int> &chromosome1, std::vector<int> &chromosome2);
    void insertBinarySearch(std::vector<int>& child, int total_cost); // uses binary search to insert
    int getCostBestSolution(); // returns cost of the best solution
    bool existsChromosome(const std::vector<int> & v); // checks if exists the chromosome
public:
    GA();
    virtual ~GA() {};

    void setPopulationSize(int populationSize);
    void setMutationRate(int mutationRate);
    void geneticAlgorithm(); // runs genetic algorithm
    void menu() override; // menu for genetic algorithm
};
