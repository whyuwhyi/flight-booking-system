#pragma once
#include <Graph.h>


class MatrixGraph:public Graph{
private:
    int** matrix;
public:
    MatrixGraph(int vertexNumber);
    virtual ~MatrixGraph();
    bool isEdge(Edge edge);
    Edge firstEdge(int vertex);
    Edge nextEdge(Edge pre);
    void setEdge(int start,int end,int weight);
    void deleteEdge(int start,int end);
    void dumpGraph();
};




MatrixGraph::MatrixGraph(int verNumber):Graph(verNumber),matrix(NULL){
    matrix = (int**) new int*[verNumber];
    for(int  i = 0;i < verNumber;i++)
        matrix[i] = new int[verNumber];
    for(int i = 0;i < verNumber;i++)
        for(int j = 0;j < verNumber;j++)
            matrix[i][j] = 0;
}

MatrixGraph::~MatrixGraph(){
    if(matrix != NULL){
        for(int i = 0;i < getVertexNumber();i++){
            if(matrix[i] != NULL){
                delete[] matrix[i];
                matrix[i] = NULL;
            }
        }
        delete[] matrix;
        matrix = NULL;
    }
}

bool MatrixGraph::isEdge(Edge edge){
    if(edge.getEndVertex() >= 0&&edge.getEndVertex() < getVertexNumber()&&
       edge.getStartVertex() >= 0&&edge.getStartVertex() < getVertexNumber())
        return matrix[edge.getStartVertex()][edge.getEndVertex()] != 0;
    return 0;
}

Edge MatrixGraph::firstEdge(int vertex){
    Edge edge;
    edge.setStartVertex(vertex);
    for(int i = 0;i < getVertexNumber();i++)
        if(matrix[vertex][i]){
            edge.setEndVertex(i);
            edge.setWeight(matrix[vertex][i]);
            break;
        } 
    return edge;
}

Edge MatrixGraph::nextEdge(Edge pre){
    Edge edge;
    edge.setStartVertex(pre.getStartVertex());
    for(int i = pre.getEndVertex()+1;i < getVertexNumber();i++)
        if(matrix[pre.getStartVertex()][i]){
            edge.setEndVertex(i);
            edge.setWeight(matrix[pre.getStartVertex()][i]);
            break;
        }
    return edge;
}

void MatrixGraph::setEdge(int start,int end,int weight){
    if(matrix[start][end] <= 0){
        increaseEdgeNumber();
        increaseIndegree(end);
    }
    matrix[start][end] = weight;
}

void MatrixGraph::deleteEdge(int start,int end){
    if(matrix[start][end] > 0){
        decreaseEdgeNumber();
        decreaseIndegree(end);
    }
    matrix[start][end] = 0;
}

void MatrixGraph::dumpGraph(){};
