#ifndef WHITE_COMPONENTS_HPP
#define WHITE_COMPONENTS_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <list>

using std::stack;
using std::queue;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::min;
using std::max;
using std::list;
using std::swap;

struct grayEdge{
    int B, E;
};

struct hurdle{
    int id, neighbour_id;   //id of the component; if hurdle is not simple, then neighbour_id is set to id of the component merged after deletion, otherwise set to -1
};

class UnionFind{
    vector<int> parent;
    vector<int> rank;
    vector<grayEdge> handle;

    public:    
        UnionFind(int);
        int Find(int);
        grayEdge FindHandle(int);
        int Union(int, int);
        void setHandle(int, grayEdge);
};

struct component {
    vector <int> vertices;
    int size;
    int direction;             //-1 default, 0 if component is negative, 1 if it's positive and 2 if component is directed (it has elements of both signs)
};

class interleavingGraph{
    int extendedRange;         //2n + 2

    vector <int> piPrim;
    vector <int> extendedPi;    //0 and 2n + 1 added, each internal node doubled to represent + and -
    vector <int> indexes;
    vector <int> edge;
    vector <int> componentId;   //id of connected component for each node
    list <hurdle> hurdles;

    UnionFind uf;

    vector <component> connectedComponent;

    pair<int, int> mergeHurdles(int, int);
    vector <pair <int, int> > mergeNonconsecutive(int);
    void reverse(int, int);
    void countComponent();
    void setComponents();
    void countHurdles();

    public:

        interleavingGraph(const vector <int>);

        vector <pair <int, int> > clearWhiteComponents();
        vector <int> returnPi();
};

#endif // SPLAY_TREE_HPP
