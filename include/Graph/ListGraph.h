#pragma once
#include <Graph.h>
#include <LinkedList.h>


struct ListUnit{
    int vertex;
    int weight;
};

class ListGraph:public Graph{
private:
    LinkedList<ListUnit>* graphList;
public:
    ListGraph(int verNumber);
    ~ListGraph();
    Edge firstEdge(int vertex);
    Edge nextEdge(Edge pre);
    void setEdge(int start,int end,int weight);
    void deleteEdge(int start,int end);
    bool isEdge(Edge edge);
};






ListGraph::ListGraph(int verNumber):Graph(verNumber),graphList(NULL){
    graphList = new LinkedList<ListUnit>[verNumber];
}

ListGraph::~ListGraph(){
    for(int i = 0;i < getVertexNumber();i++)
        graphList[i].destroy();
    graphList = NULL;
}

Edge ListGraph::firstEdge(int vertex){
    Edge edge;
    edge.setStartVertex(vertex);
    Link<ListUnit>* temp = graphList[vertex].getHead();
    if(temp->getNext() != NULL){
        edge.setEndVertex(temp->getNext()->getElement().vertex);
        edge.setWeight(temp->getNext()->getElement().weight);
    }
    return edge;
}

Edge ListGraph::nextEdge(Edge pre){
    Edge edge;
    edge.setStartVertex(pre.getStartVertex());
    Link<ListUnit>* temp = graphList[pre.getStartVertex()].getHead();
    while(temp->getNext() != NULL&&temp->getNext()->getElement().vertex <= pre.getEndVertex())
        temp = temp->getNext();
    if(temp->getNext() != NULL){
        edge.setEndVertex(temp->getNext()->getElement().vertex);
        edge.setWeight(temp->getNext()->getElement().weight);
    }
    return edge;
}

void ListGraph::setEdge(int start,int end,int weight){
    Link<ListUnit>* temp = graphList[start].getHead();
    while(temp->getNext() != NULL&&temp->getNext()->getElement().vertex < end)
        temp = temp->getNext();
    if(temp->getNext() == NULL){
        increaseEdgeNumber();
        increaseIndegree(end);
        temp->setNext(new Link<ListUnit>({end,weight}));
    }
    else if(temp->getNext()->getElement().vertex == end)
        temp->getNext()->getElement().weight = weight;
    else{
        increaseEdgeNumber();
        increaseIndegree(end);
        Link<ListUnit>* next = temp->getNext();
        temp->setNext(new Link<ListUnit>({end,weight}));
        temp->getNext()->setNext(next);
    }
}

void ListGraph::deleteEdge(int start,int end){
    Link<ListUnit>* temp = graphList[start].getHead();
    while(temp->getNext() != NULL&&temp->getNext()->getElement().vertex < end)
        temp = temp->getNext();
    if(temp->getNext() != NULL&&temp->getNext()->getElement().vertex == end){
        decreaseEdgeNumber();
        decreaseIndegree(end);
        Link<ListUnit>* next = temp->getNext();
        delete temp->getNext();
        temp->setNext(next);
    }
}

bool ListGraph::isEdge(Edge edge){
    if(edge.getStartVertex() < 0||edge.getStartVertex() >= getVertexNumber()||
       edge.getEndVertex() < 0||edge.getEndVertex() >= getVertexNumber())
        return 0;
    Link<ListUnit>* temp = graphList[edge.getStartVertex()].getHead();
    while(temp != NULL&&temp->getElement().vertex < edge.getEndVertex()){
        temp = temp->getNext();
    }
    if(temp != NULL&&temp->getElement().vertex == edge.getEndVertex())
        return 1;
    return 0;
}