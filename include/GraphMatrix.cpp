
#include "GraphMatrix.h"
#include <fstream>

template <typename T, int sz> int size(T (&)[sz]) { return sz; }

GraphMatrix::GraphMatrix() {
    vertexes = 0;
    edges = 0;
    density = 0;
    matrix = NULL;
}

GraphMatrix::~GraphMatrix() {
    if (matrix != NULL){
        for (int i = 0; i < vertexes; i++) {
            if (matrix[i] != NULL) delete[] matrix[i];
        }
        delete[] matrix;
    }
}

void GraphMatrix::loadFromFile(std::string filename) {
    std::ifstream fin;
    fin.open(filename.c_str());
    fin >> edges;
    fin >> vertexes;
    this->countDensity();

    this->createMatrix(vertexes);

    int startVertex;
    int endVertex;
    int length;
    for (int i = 0; i < edges; i++) {
        fin >> startVertex;
        fin >> endVertex;
        fin >> length;
        if (matrix[startVertex][endVertex] == -1)
            matrix[startVertex][endVertex] = length;
    }

    this->countEdges();
}

void GraphMatrix::print(std::ostream &str) const {
    str.flags ( std::ios::right);
    str.width (4);
    str << "    ";
    for (int i = 0; i < vertexes; i++) {
        str.width (4);
        str << i << " ";
    }
    str << std::endl;
    for (int i = 0; i < vertexes; i++) {
        str.width (4);
        str << i << "  ";
        for (int j = 0; j < vertexes; j++) {
            str.width (4);
            str << matrix[i][j] << " ";
        }
        str << std::endl;
    }
}

void GraphMatrix::createMatrix(int v) {
    if (matrix != NULL) {
        for (int i = 0; i < vertexes; i++){
            if (matrix[i] != NULL) delete[] matrix[i];
        }
        delete[] matrix;
    }

    this->vertexes = v;
    matrix = new int *[v];
    for (int i = 0; i < v; i++) {
        matrix[i] = new int[v];
    }

    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            matrix[i][j] = -1;
        }
    }
}

void GraphMatrix::countEdges() {
    edges = 0;
    for (int i = 0; i < vertexes; i++){
        for (int j = 0; j < vertexes; j++) {
            if (matrix[i][j] != -1)
                edges++;
        }
    }
}

void GraphMatrix::makeBothWaysEqual() {
    for (int i = 0; i < this->vertexes; i++){
        for (int j = 0; j < this->vertexes; j++) {
            if (this->matrix[i][j] != -1){
                this->matrix[j][i] = this->matrix[i][j];
            }
        }
    }
    this->countEdges();
}

void GraphMatrix::setEdge(int start, int end, int length) {
    matrix[start][end] = length;
}

int GraphMatrix::getEdgeLength(int start, int end) {
//    int rows =  sizeof matrix / sizeof matrix[0];
//    int cols = sizeof matrix[0] / sizeof(int);
//
//    if (rows <= start && cols <= end) {
        return matrix[start][end];
//    }
//    return -1;
}





