#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct LinkCutTree{
    struct Node{
        Node*p=0, *pp=0, *c[2]={0, 0};
        int id;
        bool ev=0;
        Node(int _id=-1):id(_id){};
        void recalc();
        void push();
        void unlink(int);
        int up();
        void rot();
        Node*splay();
        Node*first();
    };

    vector<Node> g;
    LinkCutTree(size_t);
    bool connected(int, int);
    void link(int, int);
    void cut_up(int);
    void cut(int, int);
    void make_root(Node*);
    Node*access(Node*);
    int lca(int, int);
    vector <int> path(int, int);
};
#endif