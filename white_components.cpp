#include "white_components.hpp"

UnionFind::UnionFind(int size) : parent(size), rank(size, 0), handle(size) {
    for (int i = 0; i < size; i++) {            
        parent[i] = i;            
        handle[i] = {-1, -1};
    }    
}

int UnionFind::Find(int x) {
    if (parent[x] != x) {
        parent[x] = Find(parent[x]);
    }
    return parent[x];
}

grayEdge UnionFind::FindHandle(int x) {
    return handle[Find(x)];
}

int UnionFind::Union(int x, int y) {
    int rootX = Find(x);
    int rootY = Find(y);
    if(rootX == rootY) return rootX;

    if (rank[rootX] > rank[rootY]) 
    {
        parent[rootY] = rootX;
        handle[rootX] = (handle[rootX].E > handle[rootY].E) ? handle[rootX] : handle[rootY];
        handle[rootY] = handle[rootX];
        return rootX;
    } 
    if (rank[rootX] < rank[rootY]) 
    {
        parent[rootX] = rootY;
        handle[rootY] = (handle[rootX].E > handle[rootY].E) ? handle[rootX] : handle[rootY];
        handle[rootX] = handle[rootY];
        return rootY;
    }
    parent[rootY] = rootX;
    handle[rootX] = (handle[rootX].E > handle[rootY].E) ? handle[rootX] : handle[rootY];
    handle[rootY] = handle[rootX];
    rank[rootX]++;
    return rootX;
}

void UnionFind::setHandle(int x, grayEdge h) {
    int rootX = Find(x);
    handle[rootX] = h;
}


vector <int> indexArr(const vector <int> arr)
{
    vector <int> ans((int)arr.size(), -1);
    for(int i = 0; i < (int)arr.size(); i++) ans[abs(arr[i])] = i;
    return ans;
}

interleavingGraph::interleavingGraph(const vector <int> pi) :   extendedPi(2 * (int)pi.size() + 2, -1),
                                                                uf(2 * (int)pi.size() + 2), 
                                                                edge(2 * (int)pi.size() + 2, -1),
                                                                piPrim((int)pi.size() + 2),
                                                                connectedComponent(2 * (int)pi.size() + 2, {{}, 0, -1})
{
    piPrim[0] = 0;
    for(int i = 0; i < (int)pi.size(); i++) piPrim[i + 1] = pi[i];
    piPrim[(int)pi.size() + 1] = (int)pi.size() + 1;
    extendedRange = 2 * (int)pi.size() + 2;
    extendedPi[0] = 0; extendedPi[extendedRange - 1] = extendedRange - 1;

    for(int i = 0, j = 1; i < (int)pi.size(); i++)
    {
        int a = pi[i];
        if(a >= 0) {
            extendedPi[j] = 2 * a - 1; j++;
            extendedPi[j] = 2 * a; j++;
        }
        else {
            extendedPi[j] = 2 * abs(a); j++;
            extendedPi[j] = 2 * abs(a) - 1; j++;
        }
    }
    vector <int> extendedIndexes = indexArr(extendedPi);
    indexes = indexArr(piPrim);

    for(int i = 0; i < extendedRange - 1; i += 2)
    {
        edge[extendedIndexes[i]] = extendedIndexes[i + 1];
        edge[extendedIndexes[i + 1]] = extendedIndexes[i];
    }
}

void interleavingGraph::reverse(int a, int b)
{
    if(a > b) swap(a, b);
    for(int i = 0; i <= (b - a)/2; i++)
    {
        swap(indexes[abs(piPrim[a + i])], indexes[abs(piPrim[b - i])]);
        swap(piPrim[a + i], piPrim[b - i]);
    }
    for(int i = a; i <= b; i++) piPrim[i] *= -1;
}

void interleavingGraph::countComponent()
{
    stack <int> S;
    for(int i = 0; i < extendedRange; i++)
    {
        int j = edge[i];
        if(i < j)
        {
            grayEdge handle = {i, j};
            uf.setHandle(i, handle);
            uf.setHandle(j, handle);
            uf.Union(i, j);
            S.push(i);
        }
        else
        {
            int C = uf.Find(i); 
            int s = uf.FindHandle(C).B;
            while(!S.empty() && S.top() >= s)
            {
                int T = uf.Find(S.top());
                S.pop();
                uf.Union(T, C);
                C = uf.Find(T);
            }
            grayEdge e = uf.FindHandle(C);
            if(e.E > i) S.push(e.B);
        }
    }
    componentId.resize(extendedRange);
    for(int i = 0; i < extendedRange; i++) componentId[i] = uf.Find(i);
}

void interleavingGraph::setComponents()
{
    for(int i = 0; i < extendedRange; i++)
    {
        int comp = componentId[i];
        int e = extendedPi[i];
        connectedComponent[comp].size++;
        if(i % 2 == 0 && componentId[i] == componentId[i - 1]) connectedComponent[comp].vertices.push_back((e+1)/2);
        if(connectedComponent[comp].direction == 2) continue;
        if(piPrim[(i+1)/2] >= 0) connectedComponent[comp].direction = (connectedComponent[comp].direction == 0) ? 2 : 1;
        else connectedComponent[comp].direction = (connectedComponent[comp].direction == 1) ? 2 : 0;
    }
}

void interleavingGraph::countHurdles()
{
    list <pair <int, int> > CR;
    pair <int, int> current_comp = {-1, 0};
    for(int i = 0; i < extendedRange; i++){
        int comp = componentId[i];
        if(connectedComponent[comp].size > 2 && (connectedComponent[comp].direction == 0 || connectedComponent[comp].direction == 1))
        {
            if(comp == current_comp.first) current_comp.second++;
            else
            {
                if(current_comp.first != -1) CR.push_back(current_comp);
                current_comp = {comp, 1};
            }
        }
    }
    if(current_comp.first != -1) CR.push_back(current_comp);

    if(CR.size() == 0) return;

    for(int i = 0; i < (int)CR.size(); i++)
    {
        if(CR.front().first == CR.back().first && CR.size() > 1)
        {
            pair <int, int> fr = CR.front(), bck = CR.back();
            assert(fr.first == bck.first);
            CR.pop_front(); CR.pop_back();
            CR.push_back({fr.first, fr.second + bck.second});
        }
        else break;
    }
  
    int size = CR.size();

    if(size < 2) {
        for(pair <int, int> i: CR) hurdles.push_back({i.first, -1});
        return;
    }
    pair <int, int> mid, left, right;
    for(int i = 0; i < size; i++)
    {
        left = CR.front(); CR.pop_front(); CR.push_back(left);
        mid = CR.front(); CR.pop_front();
        right = CR.front(); 
        CR.push_front(mid);
        if(connectedComponent[mid.first].size != mid.second) continue;
        if(left.first == right.first && left.second + right.second == connectedComponent[left.first].size) hurdles.push_back({mid.first, left.first});
        else hurdles.push_back({mid.first, -1});
    }
}

pair <int, int> interleavingGraph::mergeHurdles(int id1, int id2)
{
    int i = indexes[connectedComponent[id1].vertices[0]], j = indexes[connectedComponent[id2].vertices[0]];
    if(i > j) swap(i, j);
    reverse(i,j);
    pair <int, int> ret = {i, j};
    return(ret);
}

vector <pair <int, int> > interleavingGraph::mergeNonconsecutive(int k)
{
    assert((int)hurdles.size() >= 2 * k);

    vector <pair <int, int> > S;
    hurdle left, mid, right;
    for(int i = 0; i < k; i++)
    {        
        right = hurdles.back(); hurdles.pop_back();
        mid = hurdles.back(); hurdles.pop_back();
        left = hurdles.back(); hurdles.pop_back();
        hurdles.push_back(mid);
        S.push_back(mergeHurdles(left.id, right.id));
    }
    return S;
}

vector <pair <int, int> > interleavingGraph::clearWhiteComponents()
{
    countComponent();
    setComponents();
    countHurdles();

    vector <pair <int, int> > S;
    if(hurdles.size() == 0) return S;
    if(hurdles.size() == 1)
    {
        hurdle h = hurdles.back(); hurdles.pop_back();
        S.push_back(mergeHurdles(h.id, h.id));
        return S;
    }
    int s = hurdles.size();
    int k = s / 2;
    if(s % 2 == 0)
    {
        S = mergeNonconsecutive(k - 1);
        assert(hurdles.size() == 2);
        hurdle right = hurdles.back(); hurdles.pop_back();
        hurdle left = hurdles.back(); hurdles.pop_back();
        S.push_back(mergeHurdles(left.id, right.id));
        return S;
    }
    else if(s % 2 == 1)
    {
        for(int i = 0; i < s; i++)
        {
            hurdle h = hurdles.back(); hurdles.pop_back();
            if(h.neighbour_id == -1)
            {
                S.push_back(mergeHurdles(h.id, h.id));
                vector <pair <int, int> > T = mergeNonconsecutive(k - 1);
                for(pair <int, int> i : T) S.push_back(i);
                assert(hurdles.size() == 2);
                hurdle right = hurdles.back(); hurdles.pop_back();
                hurdle left = hurdles.back(); hurdles.pop_back();
                S.push_back(mergeHurdles(left.id, right.id));
                return S;
            }
            hurdles.push_front(h);
        }
        //fortress
        S = mergeNonconsecutive(k - 1);
        assert(hurdles.size() == 3);
        hurdle right = hurdles.back(); hurdles.pop_back();
        hurdle left = hurdles.back(); hurdles.pop_back();
        S.push_back(mergeHurdles(left.id, right.id));
        left = hurdles.back();
        hurdles.pop_back();
        right = {left.neighbour_id, -1};
        S.push_back(mergeHurdles(left.id, right.id));
        return S;
    }
    return S;
}

vector <int> interleavingGraph::returnPi()
{
    vector <int> ret;
    for(int i = 1; i < (int)piPrim.size() - 1; i++) ret.push_back(piPrim[i]);
    return ret;
}