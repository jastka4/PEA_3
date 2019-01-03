
#pragma once


#include "Graph.h"

class GraphMatrix: public Graph {
private:
    int **matrix;
    void countEdges() override;


public:
    GraphMatrix();
    ~GraphMatrix();

    void createMatrix(int size);

    void loadFromFile(std::string filename) override;
    void print(std::ostream &str) const override;

    void makeBothWaysEqual() override;

    void setEdge(int start, int end, int length) override;

    int getEdgeLength(int start, int end) override;

};
