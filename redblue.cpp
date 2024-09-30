#include "redblue.hpp"

vector <int> indexPi(const vector <int> pi)
{
	int n = pi.size();
	vector <int> resp(n+2);
	resp[0] = 0; resp[n+1] = n+1;
	for(int i = 0; i < n; i++) resp[abs(pi[i])] = i+1;
	return resp;
}

vector <int> positive(const vector <int> pi)
{
	vector <int> ans = pi;
	for(int i = 0; i < (int)ans.size(); i++) ans[i] = abs(ans[i]);
	return ans;
}

int redbluePath::recount(int x)	{return (x >= 0) ? x : abs(x) + range;}		//find the non-negative counterpart
int redbluePath::count(int x)  	{return (x < range) ? x : range - x;}		//find the original value
int redbluePath::c(int x) 		{return (x + range) % (2 * range);}			//find counterpart on the opposite side

void redbluePath::deleteRed(int i) {red[i] = -1; red[recount(i)] = -1;}		//delete r_i and r'_i
void redbluePath::checkAndDelete(int i)										//check and delete r_i and r'_i if they correspond to white isolated vertice
{
	if(i < 0 || i >= range - 1) {return;}
	int index_a = permutation.indexOf(abs(i));
	int index_b = permutation.indexOf(abs(i) + 1);

	int a = permutation.find(index_a), b = permutation.find(index_b);
	bool toDelete = (a >= 0) ? (index_b == index_a + 1 && b == a + 1) : (index_b == index_a - 1 && b == a - 1);

	if(!toDelete){return;}
	if(red[i] == i+1 || red[i] == c(i+1)) 
	{
		if(red[i] != -1){red[i] = -1; redEdgesCount--;}
		if(red[i + range] != -1){red[i + range] = -1; redEdgesCount--;}
	}
	if(red[i+1] == i || red[i+1] == c(i)) 
	{
		if(red[i+1] != -1){red[i+1] = -1; redEdgesCount--;}
		if(red[i + 1 + range] != -1){red[i + 1 + range] = -1; redEdgesCount--;}
	}
	assert(redEdgesCount >= 0);
}

redbluePath::redbluePath(const vector <int> pi) : blue(positive(pi)), DC(2 * (int)pi.size() + 4, 4 * (int)pi.size() + 4), permutation(pi){ 
	
	range = (int)pi.size() + 2; int n = 2 * range;
	red.resize(n, -1);
	redEdgesCount = n - 2;

	vector <int> posPi = positive(pi);
	vector <int> indexOf = indexPi(pi);
	
	for(int i = 1, a = 0, b = 0; i < range; i++)			//adding red edges to the red component and dynamic connectivity structure
	{
		if(i == range - 1) b = range - 1;
		else b = (pi[indexOf[i] - 1] > 0) ? i : i + range;
		red[a] = b; red[(a + range) % n] = (b + range) % n;
		DC.link(a, b); DC.link((a + range) % n, (b + range) % n);
		a = b;
	}
	for(int i = 0, a = 0, b = 0; i < range - 1; i++)		//adding blue edges that don't collide with red edges to the graph; two-colored edges are not added, but they are remembered in red component graph
	{
		b = (i == range - 2) ? range - 1 : posPi[i];
		if(red[a] != b && red[b] != a) DC.link(a, b);
		if(red[a + range] != b + range && red[b + range] != a + range) DC.link(a + range, b + range);
		a = b;
	}
	for(int i = 0; i < range - 1; i++) checkAndDelete(i);		//adding blue edges that don't collide with red edges to the graph; two-colored edges are not added, but they are remembered in red component graph
}

void redbluePath::printBlue(){blue.inOrderPrint();}
void redbluePath::reverse(int a, int b)
{
	int x = blue.find(a - 1), y = blue.find(a), p = blue.find(b), t = blue.find(b + 1);
	x = recount(x), y = recount(y), p = recount(p), t = recount(t);
	int u = c(x), v = c(y), s = c(p), q = c(t);

	if(red[x] != y && red[y] != x) DC.cut(x, y);
	if(red[u] != v && red[v] != u) DC.cut(u, v);
	if(red[p] != t && red[t] != p) DC.cut(p, t);
	if(red[s] != q && red[q] != s) DC.cut(s, q);

	if(red[x] != s && red[s] != x) DC.link(x, s);
	if(red[u] != p && red[p] != u) DC.link(u, p);
	if(red[v] != t && red[t] != v) DC.link(v, t);
	if(red[y] != q && red[q] != y) DC.link(y, q);

	blue.reverse(a, b);
	permutation.reverse(a, b);

	checkAndDelete(abs(permutation.find(a - 1))); checkAndDelete(abs(permutation.find(a)));
	checkAndDelete(abs(permutation.find(b))); checkAndDelete(abs(permutation.find(b + 1)));
}

vector <int> redbluePath::path(int u, int v) {return DC.lct.path(u, v);}

bool redbluePath::blueComponent(int a)
{
	if(a == 0 || a == range - 1) return true;
	if(a == range || a == 2 * range - 1) return false;

	int realVal = count(a);
	int index = blue.indexOf(abs(realVal));
	if(blue.find(index) == realVal) return true;
	return false;
}
bool redbluePath::sameBlueComponent(int a, int b)
{
	if(blueComponent(a) ^ blueComponent(b)) {return false;}
	return true;
}

pair <int, int> redbluePath::activeRed()	//find edge that connects points from different blue components - one of them should be in blue, the other should not
{
	if(!DC.connected(0, range)) return {0, 0};

	vector <int> path = redbluePath::path(0, range);

	vector<int>::iterator L = path.begin();
	vector<int>::iterator R = path.end() - 1;
	vector<int>::iterator M = L + (R - L)/2;
	while(L + 1 != R)
	{
		if(blueComponent(*M)) L = M;
		else R = M;
		M = L + (R - L)/2;
	}
	return {count(*L), count(*R)};
	return {0, 0};
}

pair <int, int> redbluePath::toggle(int i, int j)	//toggle vertice corresponding to edge (i, j) and return indexes of reversal
{
	int index_i = permutation.indexOf(i), index_j = permutation.indexOf(j);
	if(index_i > index_j) {swap(index_i, index_j); swap(i, j);}
	int a = permutation.find(index_i);
	bool minus_order = a >= 0, element_order = i < j;

	if(minus_order ^ element_order) {reverse(index_i, index_j - 1); return {index_i, index_j - 1};}
	else {reverse(index_i + 1, index_j); return{index_i + 1, index_j};}
}

bool redbluePath::sameSign(pair <int, int> w)
{
	int i = permutation.indexOf(abs(w.first));
	int j = permutation.indexOf(abs(w.second));
	int a = permutation.find(i);
	int b = permutation.find(j);
	
	if(a < 0 && b < 0) {return true;}
	if(a >= 0 && b >= 0) {return true;}
	return false;
}

bool redbluePath::emptyV()
{
	assert(redEdgesCount >= 0);
	return redEdgesCount == 0;
}