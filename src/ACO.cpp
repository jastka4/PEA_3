//
// Created by jskalska on 19/01/2019.
//
#include "ACO.h"

// runtime Structures and global variables
ACO::ACO()
{
    alpha = 1.0;
    beta = 5.0;
    qval = 100;
    maxTours = 20;
    antsPopulation = 50;
    evaporationRate = 0.5;
    best = INT_MAX;
}

// function init() - initializes the entire graph
void ACO::init()
{
    pheromones = new double*[gm.getNumberOfVertexes()];
    for (int i = 0; i < gm.getNumberOfVertexes(); i++)
    {
        pheromones[i] = new double[gm.getNumberOfVertexes()];
        for (int j = 0; j < gm.getNumberOfVertexes(); j++)
        {
            pheromones[i][j] = 1.0 / gm.getNumberOfVertexes();
        }
    }

    // initializing the ants
    for(int i = 0, to = 0; i < antsPopulation; i++)
    {
        if (to == gm.getNumberOfVertexes())
            to = 0;

        Ant *ant = new Ant(gm.getNumberOfVertexes());
        ant->curCity = to++;

        for (int from = 0; from < gm.getNumberOfVertexes(); from++)
        {
            ant->tabu[from] = 0;
            ant->path[from] = -1;
        }

        ant->pathIndex = 1;
        ant->path[0] = ant->curCity;
        ant->nextCity = -1;
        ant->tourLength = 0;

        // loading first city into tabu list
        ant->tabu[ant->curCity] =1;
        ants.push_back(ant);
    }
}

// reinitialize all ants and redistribute them
void ACO::restartAnts()
{
    for (int ant = 0, to = 0; ant<antsPopulation; ant++)
    {
        if (ants[ant]->tourLength < best)
        {
            best = ants[ant]->tourLength;
            bestIndex = ant;
        }

        ants[ant]->nextCity = -1;
        ants[ant]->tourLength = 0;

        for (int i = 0; i < gm.getNumberOfVertexes(); i++)
        {
            ants[ant]->tabu[i] = 0;
            ants[ant]->path[i] = -1;
        }

        if (to == gm.getNumberOfVertexes())
            to = 0;

        ants[ant]->curCity = to++;

        ants[ant]->pathIndex = 1;
        ants[ant]->path[0] = ants[ant]->curCity;

        ants[ant]->tabu[ants[ant]->curCity] = 1;
    }
}

double ACO::antProduct(int from, int to)
{
    return pow(pheromones[from][to], alpha) * pow(1.0 / gm.getEdgeLength(from, to), beta);
}

int ACO::selectNextCity( int ant )
{
    int from, to;
    double denom = 0.0;

    from=ants[ant]->curCity;

    for (to = 0; to < gm.getNumberOfVertexes(); to++)
    {
        if(ants[ant]->tabu[to] == 0)
        {
            denom += antProduct( from, to );
        }
    }

    assert(denom != 0.0);

    do
    {
        double p;
        to++;

        if (to >= gm.getNumberOfVertexes())
            to = 0;
        if (ants[ant]->tabu[to] == 0)
        {
            p = antProduct(from,to)/denom;

            double x = (double) rand() / RAND_MAX;
            if(x < p)
            {
                break;
            }
        }
    } while(true);

    return to;
}

int ACO::simulateAnts()
{
    int k;
    int moving = 0;

    for (k = 0; k < antsPopulation; k++)
    {
        // checking if there are any more cities to visit
        if (ants[k]->pathIndex < gm.getNumberOfVertexes())
        {
            ants[k]->nextCity = selectNextCity(k);
            ants[k]->tabu[ants[k]->nextCity] = 1;
            ants[k]->path[ants[k]->pathIndex++] = ants[k]->nextCity;

            ants[k]->tourLength += gm.getEdgeLength(ants[k]->curCity, ants[k]->nextCity);

            //handle last case->last city to first
            if (ants[k]->pathIndex == gm.getNumberOfVertexes())
            {
                ants[k]->tourLength += gm.getEdgeLength(ants[k]->path[gm.getNumberOfVertexes() -1], ants[k]->path[0]);
            }

            ants[k]->curCity = ants[k]->nextCity;
            moving++;

        }
    }

    return moving;
}

// updating trails
void ACO::updateTrails()
{
    int from,to;
    // pheromones evaporation
    for (from = 0; from < gm.getNumberOfVertexes(); from++)
    {
        for (to = 0; to < gm.getNumberOfVertexes(); to++)
        {
            if (from != to)
            {
                pheromones[from][to] *=( 1.0 - evaporationRate);

                if (pheromones[from][to] < 0.0)
                {
                    pheromones[from][to] = 1.0 / gm.getNumberOfVertexes();
                }
            }
        }
    }

    // add new pheromones to the trails
    for (int ant = 0; ant < antsPopulation; ant++)
    {
        for (int i = 0; i < gm.getNumberOfVertexes(); i++)
        {
            if (i < gm.getNumberOfVertexes() - 1 )
            {
                from = ants[ant]->path[i];
                to = ants[ant]->path[i + 1];
            }
            else
            {
                from = ants[ant]->path[i];
                to = ants[ant]->path[0];
            }

            pheromones[from][to] += qval / ants[ant]->tourLength;
            pheromones[to][from] = pheromones[from][to];

        }
    }

    for (from = 0; from < gm.getNumberOfVertexes(); from++)
    {
        for (to = 0; to<gm.getNumberOfVertexes(); to++)
        {
            pheromones[from][to] *= evaporationRate;
        }
    }

}

void ACO::menu()
{
    int curTime = 0;

    std::cout << "S-ACO:";
    std::cout << "MaxTime=" << maxTours * gm.getNumberOfVertexes();

    srand(static_cast<unsigned int>(time(nullptr)));

    init();

    while (curTime++ < maxTours * gm.getNumberOfVertexes())
    {
        if (simulateAnts() == 0)
        {
            updateTrails();

            if (curTime != maxTours * gm.getNumberOfVertexes())
                restartAnts();

            std::cout << "\n Time is " << curTime << "(" << best <<")";
        }
    }

    std::cout << "\nBest tour = " << best << std::endl << std::endl << std::endl;
}

void ACO::setNumberOfAnts(int antsPopulation) {
    ACO::antsPopulation = antsPopulation;
}
