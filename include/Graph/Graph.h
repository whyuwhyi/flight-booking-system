#pragma once
#include <Edge.h>
#include <iostream>
#include <MinHeap.h>
#include <ParTree.h>
#include <queue>

#ifndef INF
#define INF 0x3FFFFFFF
#endif

struct Dist{
    int vertex;
    int length;
    int preVertex;
    bool operator<(const Dist& dist){return this->length < dist.length;}
    bool operator>(const Dist& dist){return this->length > dist.length;}
};

class Graph{
private:
    int vertexNumber;
    int edgeNumber;
    bool* visited;
    int* indegree;
public:
    Graph(int verNumber);
    virtual ~Graph();
    int getVertexNumber() const;
    int getEdgeNumber() const;
    virtual Edge firstEdge(int vertex) = 0;
    virtual Edge nextEdge(Edge pre) = 0;
    virtual void setEdge(int start,int end,int weight) = 0;
    virtual void deleteEdge(int start,int end) = 0;
    virtual bool isEdge(Edge edge) = 0;
    void increaseEdgeNumber();
    void decreaseEdgeNumber();
    void increaseIndegree(int vertex);
    void decreaseIndegree(int vertex);
    void clearVisited();
    void topSort();
    bool isRing(bool flag);
    void printTopSort();
    void dfsTopSort(std::vector<int>& ans,int& count);
    static void deepFirstTraverseGraph(Graph& graph);
    static void broadFirstTraverseGraph(Graph& graph);
    static void deepFirstSearch(Graph& graph,int v);
    static void broadFirstSearch(Graph& graph,int v);
    static void visit(Graph& graph,int v);
    void Dijkstra(int s,Dist*& dist);
    int minVertex(Dist*& dist);
    void Floyd(Dist**& dist);
    void Prim(int s,Edge*& mst);
    void Kruskal(Edge*& mst);
};





Graph::Graph(int verNumber):vertexNumber(verNumber),edgeNumber(0),visited(NULL),indegree(NULL){
    visited = new bool[verNumber];
    indegree = new int[verNumber];
    for(int i = 0;i < verNumber;i++){
        visited[i] = 0;
        indegree[i] = 0;
    }
}

Graph::~Graph(){
    if(visited != NULL){
        delete visited;
        visited = NULL;
    }
    if(indegree != NULL){
        delete indegree;
        indegree =NULL;
    }
}

int Graph::getVertexNumber() const{
    return vertexNumber;
}

int Graph::getEdgeNumber() const{
    return edgeNumber;
}

void Graph::increaseEdgeNumber(){
    edgeNumber++;
}

void Graph::decreaseEdgeNumber(){
    edgeNumber--;
}

void Graph::increaseIndegree(int vertex){
    indegree[vertex]++;
}

void Graph::decreaseIndegree(int vertex){
    indegree[vertex]--;
}

void Graph::clearVisited(){
    for(int i = 0;i < vertexNumber;i++)
        visited[i] = 0;
}

void Graph::deepFirstTraverseGraph(Graph& graph){
    graph.clearVisited();
    for(int i = 0;i < graph.vertexNumber;i++)
        if(graph.visited[i] == 0)
            Graph::deepFirstSearch(graph,i);
}

void Graph::deepFirstSearch(Graph& graph,int v){
    graph.visited[v] = 1;
    Graph::visit(graph,v);
    for(Edge e = graph.firstEdge(v);graph.isEdge(e);e = graph.nextEdge(e))
        if(graph.visited[e.getEndVertex()] == 0)
            Graph::deepFirstSearch(graph,e.getEndVertex());
}

void Graph::broadFirstTraverseGraph(Graph& graph){
    for(int i = 0;i < graph.vertexNumber;i++)
        graph.visited[i] = 0;
    for(int i = 0;i < graph.vertexNumber;i++)
        if(graph.visited[i] == 0)
            Graph::broadFirstSearch(graph,i);
}

void Graph::broadFirstSearch(Graph& graph,int v){
    std::queue<int> vertexQueue;
    Graph::visit(graph,v);
    vertexQueue.push(v);
    while(!vertexQueue.empty()){
        int u = vertexQueue.front();
        vertexQueue.pop();
        for(Edge e = graph.firstEdge(u);graph.isEdge(e);e = graph.nextEdge(e))
            if(graph.visited[e.getEndVertex()] == 0){
                Graph::visit(graph,e.getEndVertex());
                graph.visited[e.getEndVertex()] = 1;
                vertexQueue.push(e.getEndVertex());
            }
    }
}

//有向图flag置0，无向图flag置1；
//根据拓扑排序，有向图每次删掉入度为0的顶点（直到没有顶点入度为0),
//若此时还有顶点未删除，则代表有回路
//类似的，无向图每次删掉入度小于等于1的顶点即可
bool Graph::isRing(bool flag){
    clearVisited();
    int* indegreeCopy = new int[vertexNumber];
    std::queue<int> vertexQueue;
    for(int i = 0;i < vertexNumber;i++)
        indegreeCopy[i] = indegree[i];
    for(int i = 0;i < vertexNumber;i++)
        if(indegreeCopy[i] <= flag)
            vertexQueue.push(i);
    while(!vertexQueue.empty()){
        int v = vertexQueue.front();
        vertexQueue.pop();
        visited[v] = 1;
        for(Edge e = firstEdge(v);isEdge(e);e = nextEdge(e)){
            //无向图为了避免死循环，跳过已访问顶点,有向图不可能访问到已访问的顶点，无需判断
            if(!visited[e.getEndVertex()]){
                indegreeCopy[e.getEndVertex()]--;
                if(indegreeCopy[e.getEndVertex()] <= flag)
                    vertexQueue.push(e.getEndVertex());
            }
        }
    }
    for(int i = 0;i < vertexNumber;i++)
        if(visited[i] == 0){
            return 1;
        }
    return 0;
}

void Graph::topSort(){
    clearVisited();
    int* indegreeCopy = new int[vertexNumber];
    std::queue<int> vertexQueue;
    for(int i = 0;i < vertexNumber;i++)
        indegreeCopy[i] = indegree[i];
    for(int i = 0;i < vertexNumber;i++)
        if(indegreeCopy[i] == 0)
            vertexQueue.push(i);
    while(!vertexQueue.empty()){
        int v = vertexQueue.front();
        vertexQueue.pop();
        Graph::visit(*this,v);
        visited[v] = 1;
        for(Edge e = firstEdge(v);isEdge(e);e = nextEdge(e)){
            indegreeCopy[e.getEndVertex()]--;
            if(indegreeCopy[e.getEndVertex()] == 0)
                vertexQueue.push(e.getEndVertex());
        }
    }
    for(int i = 0;i < vertexNumber;i++)
        if(visited[i] == 0){
            std::cerr << "The Graph has a ring." << std::endl;
            break;
        }
}

void Graph::printTopSort(){
    clearVisited();
    int count = 0;
    std::vector<int> order;
    int* indegree_copy = new int[vertexNumber];
    for(int i = 0;i < vertexNumber;i++)
        indegree_copy[i] = indegree[i];
    dfsTopSort(order,count);
    std::cout << count << std::endl;
    for(int i = 0;i < vertexNumber;i++)
        indegree[i] = indegree_copy[i];
    delete[] indegree_copy;
}

void Graph::dfsTopSort(std::vector<int>& ans,int& count){
    if(ans.size() == vertexNumber){
        for(int i = 0;i < vertexNumber;i++)
            std::cout << "v" << ans[i] << " ";
        std::cout << std::endl;
        ans.pop_back();
        count++;
        return ;
    }
    for(int i = 0;i < vertexNumber;i++){
        if(indegree[i] == 0&&!visited[i]){
            ans.push_back(i);
            visited[i] = 1;
            for(Edge edge= firstEdge(i);isEdge(edge);edge = nextEdge(edge))
                if(!visited[edge.getEndVertex()])
                    indegree[edge.getEndVertex()]--;
            dfsTopSort(ans,count);
            visited[i] = 0;
            for(Edge edge= firstEdge(i);isEdge(edge);edge = nextEdge(edge))
                if(!visited[edge.getEndVertex()])
                    indegree[edge.getEndVertex()]++;  
        }
    }
    ans.pop_back();
}

void Graph::visit(Graph& graph,int v){
    std::cout << "The vertex of " << v << " is visited." << std::endl;
}

void Graph::Dijkstra(int s,Dist*& dist){
    dist = new Dist[vertexNumber];
    for(int i = 0;i < vertexNumber;i++){
        visited[i] = 0;
        dist[i].vertex = i;
        dist[i].preVertex = s;
        dist[i].length = INF;
    }
    dist[s].length = 0;
    MinHeap<Dist> mh(vertexNumber);
    mh.insert(dist[s]);
    for(int i = 0;i < vertexNumber;i++){
        Dist tempDist = mh.removeMin();
        if(tempDist.length == INF)
            break;
        int v = tempDist.vertex;
        visited[v] = 1;
        Graph::visit(*this,v);
        for(Edge e = firstEdge(v);isEdge(e);e = nextEdge(e))
            if(dist[e.getEndVertex()].length > (dist[v].length+e.getWeight())){
                dist[e.getEndVertex()].length = dist[v].length+e.getWeight();
                dist[e.getEndVertex()].preVertex = v;
                mh.insert(dist[e.getEndVertex()]);
            }
    }
}

// void Graph::Dijkstra(int s,Dist*& dist){
//     dist = new Dist[vertexNumber];
//     for(int i = 0;i < vertexNumber;i++){
//         visited[i] = 0;
//         dist[i].vertex = i;
//         dist[i].preVertex = s;
//         dist[i].length = INF;
//     }
//     dist[s].length = 0;
//     for(int i = 0;i < vertexNumber;i++){
//         int min = minVertex(dist);
//         Dist tempDist = dist[min];
//         if(tempDist.length == INF)
//             break;
//         int v = tempDist.vertex;
//         visited[v] = 1;
//         Graph::visit(*this,v);
//         for(Edge e = firstEdge(v);isEdge(e);e = nextEdge(e))
//             if(dist[e.getEndVertex()].length > (dist[v].length+e.getWeight())){
//                 dist[e.getEndVertex()].length = dist[v].length+e.getWeight();
//                 dist[e.getEndVertex()].preVertex = v;
//             }
//     }
// }

int Graph::minVertex(Dist*& dist){
    int min = 0;
    for(int i = 0;i < vertexNumber;i++)
        if(visited[i] == 0){
            min = i;
            break;
        }
    for(int i = 0;i < vertexNumber;i++)
        if(visited[i] == 0&&dist[i] < dist[min])
            min = i;
    return min;
}

void Graph::Floyd(Dist**& dist){
    dist = (Dist**) new Dist*[vertexNumber];
    for(int i = 0;i < vertexNumber;i++)
        dist[i] = new Dist[vertexNumber];
    for(int i = 0;i < vertexNumber;i++)
        for(int j = 0;j < vertexNumber;j++)
            if(i == j){
                dist[i][j].length = 0;
                dist[i][j].preVertex = i;
            }
            else{
                dist[i][j].length = INF;
                dist[i][j].preVertex = i;
            }
    for(int i = 0;i < vertexNumber;i++)
        for(Edge e = firstEdge(i);isEdge(e);e = nextEdge(e)){
            dist[i][e.getEndVertex()].length = e.getWeight();
            dist[i][e.getEndVertex()].preVertex = i;
        }
    for(int v = 0;v < vertexNumber;v++)
        for(int i = 0;i < vertexNumber;i++)
            for(int j = 0;j < vertexNumber;j++)
                if(dist[i][j].length > dist[i][v].length+dist[v][j].length){
                    dist[i][j].length = dist[i][v].length+dist[v][j].length;
                    dist[i][j].preVertex = dist[v][j].preVertex;
                }
}

void Graph::Prim(int s,Edge*& mst){
    int mstTag = 0;
    mst = new Edge[vertexNumber-1];
    Dist* dist = new Dist[vertexNumber];
    for(int i = 0;i < vertexNumber;i++){
        visited[i] = 0;
        dist[i].vertex = i;
        dist[i].preVertex = s;
        dist[i].length = INF;
    }
    dist[s].length = 0;
    visited[s] = 1;
    int v = s;
    for(int i = 0;i < vertexNumber-1;i++){
        if(dist[v].length == INF)
            return ;
        for(Edge e = firstEdge(v);isEdge(e);e = nextEdge(e))
            if(visited[e.getEndVertex()] == 0&&dist[e.getEndVertex()].length > e.getWeight()){
                dist[e.getEndVertex()].length = e.getWeight();
                dist[e.getEndVertex()].preVertex = v; 
            }
        v = minVertex(dist);
        visited[v] = 1;
        Edge edge(dist[v].preVertex,dist[v].vertex,dist[v].length);
        mst[mstTag++] = edge;
    }
}

void Graph::Kruskal(Edge*& mst){
    mst = new Edge[vertexNumber-1];
    ParTree<int> aclass(vertexNumber);
    for(int i = 0;i < vertexNumber;i++)
        (aclass.getArray()+i)->setValue(i);
    MinHeap<Edge> mh(edgeNumber);
    int mstTag = 0;
    for(int i = 0;i < vertexNumber;i++)
        for(Edge e = firstEdge(i);isEdge(e);e = nextEdge(e))
            if(e.getStartVertex() < e.getEndVertex())
                mh.insert(e);
    int equalNumber = vertexNumber;
    while(equalNumber > 1){
        Edge e = mh.removeMin();
        if(e.getWeight() >= INF){
            std::cerr << "The minimum spanning tree does not exist" << std::endl;
            delete[] mst;
            return;
        }
        int start = e.getStartVertex();
        int end = e.getEndVertex();
        if(aclass.Different(start,end)){
            aclass.Union(start,end);
            mst[mstTag++] = e;
            equalNumber--;
        }
    }
}


