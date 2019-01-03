#pragma once

#include <iostream>


class Graph {
protected:
    int vertexes;//liczba wierzcholkow
    int edges; //liczba krawedzi
    int density; //gestosc

    virtual void countDensity();
    virtual void countEdges() = 0;

public:
    virtual ~Graph();

    int getDensity();
    int getNumberOfVertexes() const;
    int getNumberOfEdges() const;


    virtual void loadFromFile(std::string filename) = 0;
    virtual void print(std::ostream& str)const = 0;
    virtual void makeBothWaysEqual() = 0;
    virtual void setEdge(int start, int end, int length) = 0;
    virtual int getEdgeLength(int start, int end) = 0;

    friend std::ostream& operator<<(std::ostream& str, Graph const& g){
        g.print(str);
        return str;
    }
};
