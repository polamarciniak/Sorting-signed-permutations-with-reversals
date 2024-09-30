#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <iostream>
#include <vector>
#include <algorithm>

using std::swap;
using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::vector;

const int MINVAL = 0;
const int MAXVAL = INT_MAX;

struct node {
	int key;
	int subtreeSize;
	bool flag;
	node* parent;
	node* left;
	node* right;

	node(int k) : key(k), subtreeSize(1), flag(true), parent(NULL), left(NULL), right(NULL){};
};

class splayTree {
	node* root;
	vector <node*> indexes;

	void clearFlag(node*);

	void zig(node*);
	void zigzig(node*);
	void zigzag(node*);

	void splay(node*);

	public:
		splayTree();
		splayTree(const vector <int>);
		int find(int);
		node* insert(int);
		void reverse(int, int);
		int indexOf(int);
		
		friend pair <splayTree, splayTree> split(splayTree, int);
		friend splayTree join(splayTree, splayTree);
		
		void inOrderPrint();
};


#endif // SPLAY_TREE_HPP
