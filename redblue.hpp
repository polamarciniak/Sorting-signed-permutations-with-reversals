#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

#include "dynamic_connectivity.hpp"
#include "splay.hpp"

class redbluePath{
	Layer_Structure DC;	
	splayTree blue;
	vector <int> red;

	int range;
	int redEdgesCount;
	int recount(int);
	int count(int);
	int c(int);
	bool blueComponent(int);
	bool sameBlueComponent(int, int);

	public:
		splayTree permutation;

		redbluePath(const vector <int>);

		void deleteRed(int);
		void checkAndDelete(int);
		void printBlue();

		void reverse(int, int);

		vector <int> path(int, int);
		pair <int, int> activeRed();

		pair <int, int> toggle(int, int);

		bool sameSign(pair <int, int>);
		bool emptyV();
};