#ifndef DYNAMIC_CONNECTIVITY_HPP
#define DYNAMIC_CONNECTIVITY_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <cassert>
#include <random>
#include <chrono>
#include "link_cut_tree.hpp"

using namespace std;

#ifdef LOCAL_RUN
#define asser(x) do{if(1){assert(x);}}while(0)
#define asser2(x) do{if(1){assert(x);}}while(0)
#else
#define asser(x) do{if(0){assert(x);}}while(0)
#define asser2(x) do{if(0){assert(x);}}while(0)
#endif

struct Treap{
    struct Node{
		static mt19937 rng;
		static bool rng_init;
        Node*l, *r, *p;
        size_t y;
        unsigned int size;
        //int from, to;
        char mark, sub_mark; // used to find edges on current level
        Node():l(0), r(0), p(0), y(rng()), size(1), mark(0), sub_mark(0){if(!rng_init) rng = mt19937(std::chrono::duration_cast<std::chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()); rng_init=1;}
        //Node(int _from, int _to):Node(){from=_from; to=_to;}
        Node* recalc();
        Node* set_l(Node*);
        Node* set_r(Node*);
        Node* set_ch(Node*, Node*);
    };
    static Node* root(Node*);
    static unsigned int size(Node*);
    // splits tree into <c, =c, >c
    static pair<Node*, Node*> split(Node* );
    // splits tree into <c, >=c
    static pair<Node*, Node*> lower_split(Node*);
    static Node* join(Node*, Node*);
    // move a to the front
    static Node* evert(Node*);
    static Node* push_front(Node*, Node*);
    static Node* push_back(Node*, Node*);
    static Node* begin(Node*);
    static Node* end(Node*);
    static unsigned int lower_cnt(Node*);
	static Node* get(Node*, unsigned int);
    static void re_path(Node*);
};

struct Euler_Tour_Tree{
    vector<tuple<int, int, bool> > const&graph;
    vector<Treap::Node> edges;
    vector<Treap::Node> first_edge;
    Euler_Tour_Tree(int n, int m, vector<tuple<int, int, bool> >&_graph):graph(_graph), edges(2*m), first_edge(n){
        for(int i=0;i<n;++i){
            first_edge[i] = Treap::Node();
        }
    };
    bool is_single(int);
    bool connected(int, int);
    void reroot(int);
    void link(int, char);
    void cut(int);
    unsigned int size(int);
    void set_mark(int, char);
    void set_edge_mark(int, char);
    void set_edge_mark(Treap::Node*, char);
    template<char mark_mask, class OP>
    bool call_on_nodes(int a, OP &&op){
        Treap::Node*c = &first_edge[a];
        while(c->p && !(c->sub_mark&mark_mask)) c = c->p;
        while(c->sub_mark&mark_mask){
            if(c->mark&mark_mask){
                if(op(c)) return true;
            }
            // escape subtree if mark got unset
            while(c->p && !(c->sub_mark&mark_mask)) c = c->p;
            // find successor with mark_mask set in sub_mark
            if(c->r && c->r->sub_mark&mark_mask){
                c = c->r;
                while(c->l && c->l->sub_mark&mark_mask) c = c->l;
            } else {
                while(c->p && c == c->p->r) c = c->p;
                if(!c->p){
                    while(c->l && c->l->sub_mark&mark_mask) c = c->l;
                } else {
                    c = c->p;
                }
            }
        }
        asser(!(Treap::root(&first_edge[a])->sub_mark & mark_mask));
        return false;
    };
    unsigned int index_edge(Treap::Node* n){
        return (n-edges.data())/2;
    }
    unsigned int index_vertex(Treap::Node*n){
        return n - first_edge.data();
    }
};

struct Layer_Structure{
    int logn, n, m;
    vector<Euler_Tour_Tree> levels;
    vector<tuple<int, int, bool> > edges;
    vector<unsigned int> free_edges;
    LinkCutTree lct;
    vector<map<int, int> > graph;
    Layer_Structure(int, int);
    int calc_graph_pos(int, int);
    bool connected(int, int);
    void insert_edge(int, int);
    void remove_edge(unsigned int, int);
    void link(int,int);
    void push_edge(unsigned int, int);
    void cut(int a, int b);
};
#endif // SPLAY_TREE_HPP