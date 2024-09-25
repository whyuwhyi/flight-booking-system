#pragma once

#ifndef NULL
#define NULL 0
#endif

class Edge{
private:
    int startVertex;
    int endVertex;
    int weight;
public:
    Edge();
    Edge(int startVertex,int endVertex,int weight);
    ~Edge();
    int getStartVertex() const;
    void setStartVertex(int startVertex);
    int getEndVertex() const;
    void setEndVertex(int endVertex);
    int getWeight() const;
    void setWeight(int weight);
    bool operator>(const Edge& edge);
    bool operator<(const Edge& edge);
};



Edge::Edge():startVertex(-1),endVertex(-1),weight(-1){}

Edge::Edge(int startVertex,int endVertex,int weight)
        :startVertex(startVertex),endVertex(endVertex),weight(weight){}

Edge::~Edge(){}

int Edge::getStartVertex() const{
    return startVertex;
}

void Edge::setStartVertex(int startVertex){
    this->startVertex = startVertex;
}

int Edge::getEndVertex() const{
    return endVertex;
}

void Edge::setEndVertex(int endVertex){
    this->endVertex = endVertex;
}

int Edge::getWeight() const{
    return weight;
}

void Edge::setWeight(int weight){
    this->weight = weight;
}

bool Edge::operator>(const Edge& edge){
    return this->weight > edge.weight;
}

bool Edge::operator<(const Edge& edge){
    return this->weight < edge.weight;
}