//
// Created by jskalska on 30/12/2018.
//

#include "GA.h"

GA::GA() {
    this->populationSize = 10;
    this->realPopulationSize = 0;
    this->generations = 1000;
    this->mutationRate = 10;
    this->showPopulationEnabled = false;
}

// checks if is a valid solution, then return total cost of path else return -1
int GA::isValidSolution(std::vector<int> &solution) {
    int total_cost = 0;
    std::set<int> set_solution;

    // checks if not contains elements repeated
    for (int i = 0; i < gm.getNumberOfVertexes(); i++)
        set_solution.insert(solution[i]);

    if (set_solution.size() != (unsigned) gm.getNumberOfVertexes())
        return -1;

    // checks if connections are valid
    for (int i = 0; i < gm.getNumberOfVertexes(); i++) {
        if (i + 1 < gm.getNumberOfVertexes()) {
            int cost = gm.getEdgeLength(solution[i], solution[i + 1]);

            // checks if exists connection
            if (cost == -1)
                return -1;
            else
                total_cost += cost;
        } else {
            int cost = gm.getEdgeLength(solution[i], solution[0]);

            // checks if exists connection
            if (cost == -1)
                return -1;
            else
                total_cost += cost;
            break;
        }
    }
    return total_cost;
}


bool GA::existsChromosome(const std::vector<int> &v) {
    // checks if exists in the population
    for (std::vector<std::pair<std::vector<int>, int> >::iterator it = population.begin();
         it != population.end(); ++it) {
        const std::vector<int> &vec = (*it).first; // gets the std::vector
        if (equal(v.begin(), v.end(), vec.begin())) // compares std::vectors
            return true;
    }
    return false;
}


void GA::initialPopulation() // generates the initial population
{
    std::vector<int> parent;

    // inserts initial vertex in the parent
    parent.push_back(0);

    // creates the parent
    for (int i = 0; i < gm.getNumberOfVertexes(); i++) {
        if (i != 0)
            parent.push_back(i);
    }

    int total_cost = isValidSolution(parent);

    if (total_cost != -1) // checks if the parent is valid
    {
        population.push_back(make_pair(parent, total_cost)); // inserts in the population
        realPopulationSize++; // increments realPopulationSize
    }

    // makes random permutations "generations" times
    for (int i = 0; i < generations; i++) {
        // generates a random permutation
        random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (gm.getNumberOfVertexes() - 1) + 1));

        int total_cost = isValidSolution(parent); // checks if solution is valid

        // checks if permutation is a valid solution and if not exists
        if (total_cost != -1 && !existsChromosome(parent)) {
            population.push_back(make_pair(parent, total_cost)); // add in population
            realPopulationSize++; // increments realPopulationSize in the unit
        }
        if (realPopulationSize == populationSize) // checks size population
            break;
    }

    // checks if realPopulationSize is 0
    if (realPopulationSize == 0)
        std::cout << "\nEmpty initial population ;( Try again runs the algorithm...";
    else
        sort(population.begin(), population.end(), sort_pred()); // sort population
}

void GA::showPopulation() {
    std::cout << "\nShowing solutions...\n\n";
    for (std::vector<std::pair<std::vector<int>, int> >::iterator it = population.begin();
         it != population.end(); ++it) {
        const std::vector<int> &vec = (*it).first; // gets the std::vector

        for (int i = 0; i < gm.getNumberOfVertexes(); i++)
            std::cout << vec[i] << " ";
        std::cout << 0;
        std::cout << " | Cost: " << (*it).second << "\n\n";
    }
    std::cout << "\nPopulation size: " << realPopulationSize << std::endl;
}

// inserts in the std::vector using binary search
void GA::insertBinarySearch(std::vector<int> &child, int total_cost) {
    int imin = 0;
    int imax = realPopulationSize - 1;

    while (imax >= imin) {
        int imid = imin + (imax - imin) / 2;

        if (total_cost == population[imid].second) {
            population.insert(population.begin() + imid, make_pair(child, total_cost));
            return;
        } else if (total_cost > population[imid].second)
            imin = imid + 1;
        else
            imax = imid - 1;
    }
    population.insert(population.begin() + imin, make_pair(child, total_cost));
}

/*
	Makes the crossover
	This crossover selects two random points
	These points generates substrings in both parents
	The substring inverted of parent1 is placed in parent2 and
	the substring inverted of parent2 is placed in parent1

	Example:
		parent1: 1 2 3 4 5
		parent2: 1 2 4 5 3

		substring in parent1: 2 3 4
		substring in parent2: 2 4 5

		substring inverted in parent1: 4 3 2
		substring inverted in parent2: 5 4 2

		child1: 1 5 4 2 5
		child2: 1 4 3 2 3

		Children are invalids: 5 appears 2x in child1 and 3 appears 2x in child2
		Solution: map of genes that checks if genes are not used
*/
void GA::crossover(std::vector<int> &parent1, std::vector<int> &parent2) {
    std::vector<int> child1, child2;

    // map of genes, checks if already are selected
    std::map<int, int> genes1, genes2;

    for (int i = 0; i < gm.getNumberOfVertexes(); i++) {
        // initially the genes not are used
        genes1[parent1[i]] = 0;
        genes2[parent2[i]] = 0;
    }

    // generates random points
    int point1 = rand() % (gm.getNumberOfVertexes() - 1) + 1;
    int point2 = rand() % (gm.getNumberOfVertexes() - point1) + point1;

    // adjusts the points if they are equal
    if (point1 == point2) {
        if (point1 - 1 > 1)
            point1--;
        else if (point2 + 1 < gm.getNumberOfVertexes())
            point2++;
        else {
            // point1 or point2 ?? random...
            int point = rand() % 10 + 1; // number in the range 1 to 10
            if (point <= 5)
                point1--;
            else
                point2++;
        }
    }

    // generates childs

    // until point1, child1 receives genes of the parent1
    // and child2 receives genes of the parent2
    for (int i = 0; i < point1; i++) {
        // adds genes
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
        // marks genes
        genes1[parent1[i]] = 1;
        genes2[parent2[i]] = 1;
    }

    // marks remaining genes
    for (int i = point2 + 1; i < gm.getNumberOfVertexes(); i++) {
        genes1[parent1[i]] = 1;
        genes2[parent2[i]] = 1;
    }

    // here is the substring inverted
    // child1 receives genes of the parent2 and
    // child2 receives genes of the parent1
    for (int i = point2; i >= point1; i--) {
        if (genes1[parent2[i]] == 0) // if the gene is not used
        {
            child1.push_back(parent2[i]);
            genes1[parent2[i]] = 1; // marks the gene
        } else {
            // if the gene already is used, chooses gene that is not used
            for (std::map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it) {
                if (it->second == 0) // checks if is not used
                {
                    child1.push_back(it->first);
                    genes1[it->first] = 1; // marks as used
                    break; // left the loop
                }
            }
        }

        if (genes2[parent1[i]] == 0) // if the gene is not used
        {
            child2.push_back(parent1[i]);
            genes2[parent1[i]] = 1; // marks the gene
        } else {
            // if the gene already is used, chooses gene that is not used
            for (std::map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it) {
                if (it->second == 0) // checks if is not used
                {
                    child2.push_back(it->first);
                    genes2[it->first] = 1; // marks as used
                    break; // left the loop
                }
            }
        }
    }

    // ramaining genes: child1 receives genes of the parent1
    // and child2 receives genes of the parent2
    for (int i = point2 + 1; i < gm.getNumberOfVertexes(); i++) {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }

    // mutation
    int mutation = rand() % 100 + 1; // random number in [1,100]
    if (mutation <= mutationRate) // checks if the random number <= mutation rate
    {
        // makes a mutation: change of two genes

        int index_gene1, index_gene2;
        index_gene1 = rand() % (gm.getNumberOfVertexes() - 1) + 1;
        index_gene2 = rand() % (gm.getNumberOfVertexes() - 1) + 1;

        // makes for child1
        int aux = child1[index_gene1];
        child1[index_gene1] = child1[index_gene2];
        child1[index_gene2] = aux;

        // makes for child2
        aux = child2[index_gene1];
        child2[index_gene1] = child2[index_gene2];
        child2[index_gene2] = aux;
    }

    int total_cost_child1 = isValidSolution(child1);
    int total_cost_child2 = isValidSolution(child2);

    // checks if is a valid solution and not exists in the population
    if (total_cost_child1 != -1 && !existsChromosome(child1)) {
        // add child in the population
        insertBinarySearch(child1, total_cost_child1); // uses binary search to insert
        realPopulationSize++; // increments the realPopulationSize
    }

    // checks again...
    if (total_cost_child2 != -1 && !existsChromosome(child2)) {
        // add child in the population
        insertBinarySearch(child2, total_cost_child2); // uses binary search to insert
        realPopulationSize++; // increments the realPopulationSize
    }
}

// runs the genetic algorithm
void GA::geneticAlgorithm() {
    initialPopulation(); // gets initial population

    if (realPopulationSize == 0)
        return;

    for (int i = 0; i < generations; i++) {
        int old_populationSize = realPopulationSize;

        /* selects two parents (if exists) who will participate
            of the reproduction process */
        if (realPopulationSize >= 2) {
            if (realPopulationSize == 2) {
                // applying crossover in the parents
                crossover(population[0].first, population[1].first);
            } else {
                // realPopulationSize > 2

                int parent1, parent2;

                do {
                    // select two random parents
                    parent1 = rand() % realPopulationSize;
                    parent2 = rand() % realPopulationSize;
                } while (parent1 == parent2);

                // applying crossover in the two parents
                crossover(population[parent1].first, population[parent2].first);
            }

            // gets difference to check if the population grew
            int diff_population = realPopulationSize - old_populationSize;

            if (diff_population == 2) {
                if (realPopulationSize > populationSize) {
                    // removes the two worst parents of the population
                    population.pop_back();
                    population.pop_back();

                    // decrements the realPopulationSize in 2 units
                    realPopulationSize -= 2;
                }
            } else if (diff_population == 1) {
                if (realPopulationSize > populationSize) {
                    population.pop_back(); // removes the worst parent of the population
                    realPopulationSize--; // decrements the realPopulationSize in the unit
                }
            }
        } else // population contains only 1 parent
        {
            // applying crossover in the parent
            crossover(population[0].first, population[0].first);

            if (realPopulationSize > populationSize) {
                population.pop_back(); // removes the worst parent of the population
                realPopulationSize--; // decrements the realPopulationSize in the unit
            }
        }
    }

    if (showPopulationEnabled == true)
        showPopulation(); // shows the population

    std::cout << "\nBest solution: ";
    const std::vector<int> &vec = population[0].first;
    for (int i = 0; i < gm.getNumberOfVertexes(); i++)
        std::cout << vec[i] << " ";
    std::cout << 0;
    std::cout << " | Cost: " << population[0].second << std::endl;
}

int GA::getCostBestSolution() {
    if (realPopulationSize > 0)
        return population[0].second;
    return -1;
}

void GA::menu() {
    std::cout << "1. Change the population size. Actual: " << populationSize << ".\n"
                 "2. Choose the mutation rate. Actual: " << mutationRate << ".\n"
                 "4. Choose the mutation method. Actual: " << mutationMethod << ".\n"
                 "3. Choose the crossover rate. Actual: " << crossoverRate << ".\n"
                 "4. Choose the crossover method. Actual: " << crossoverMethod << ".\n"
                 "6. Perform search.\n"
                 "7. Exit.\n"
                 "Please enter the appropriate number: ";
    int chosen;
    std::string file_name;
    std::cin >> chosen;
    switch (chosen) {
        case 1:
            std::cout << "Please specify the population size (more than 1): \n";
            std::cin >> populationSize;
            break;
        case 2:
            std::cout << "Please specify the mutation rate (less than 100): \n";
            std::cin >> mutationRate;
            break;
        case 3:
            std::cout << "Please specify the crossover rate (less than 100): \n";
            std::cin >> crossoverRate;
            break;
        case 4:
            std::cout << "Please choose the crossover method:\n";
//                         "0 - swap\n"
//                         "1 - insert\n"
//                         "2 - invert\n";
            int mutation;
            std::cin >> mutation;
            switch(mutation){
                case 0:
                    mutationMethod = SWAP;
                    break;
                case 1:
                    mutationMethod = INSERT;
                    break;
                case 2:
                    mutationMethod = INVERT;
                    break;
            }
            break;
        case 5:
            std::cout << "Please choose the crossover method:\n";
//                         "0 - swap\n"
//                         "1 - insert\n"
//                         "2 - invert\n";
            int crossover;
            std::cin >> crossover;
            switch(crossover){
                case 0:
                    crossoverMethod = SWAP;
                    break;
                case 1:
                    crossoverMethod = INSERT;
                    break;
                case 2:
                    crossoverMethod = INVERT;
                    break;
            }
            break;
        case 6:
            geneticAlgorithm();
//            std::cout << "\n########################################\n" << simulatedAnnealing() << std::endl;
            break;
        case 7:
            return;
        default:
            std::cout << "Please enter the correct number!\n";
            std::cin.clear();
            std::cin.sync();
            break;
    }
    menu();
}

void GA::setPopulationSize(int populationSize) {
    GA::populationSize = populationSize;
}

void GA::setMutationRate(int mutationRate) {
    GA::mutationRate = mutationRate;
}

