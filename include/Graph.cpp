

#include "Graph.h"


int Graph::getDensity() {
    return density;
}

void Graph::countDensity() {
    if (vertexes != 0)
        density = 100*edges/((vertexes*vertexes - vertexes));
    else density = 0;
}

Graph::~Graph() {
    vertexes = 0;
    edges = 0;
    density = 0;
}

int Graph::getNumberOfVertexes() const {
    return vertexes;
}

int Graph::getNumberOfEdges() const {
    return edges;
}