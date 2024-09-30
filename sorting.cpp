#include "white_components.hpp"
#include "redblue.hpp"

bool ToggleAndAppend (redbluePath* V, vector <pair <int, int> > &S1, vector <pair <int, int> > &Reversals)
{
    pair <int, int> v = V -> activeRed();
    if(v.first == 0 && v.second == 0){return false;}

    int i = abs(v.first), j = abs(v.second);
    pair <int, int> revIndexes = V -> toggle(i, j);
    pair <int, int> vertIndexes = {min(i, j), max(i, j)};

    S1.push_back(vertIndexes);
    Reversals.push_back(revIndexes);

    return true;
}

pair <int, int> UndoAndRemove(redbluePath* V, vector <pair <int, int> > &S1, vector <pair <int, int> > &Reversals)
{
    pair <int, int> v = V -> activeRed();
    if(v.first != 0 || v.second != 0){return {0, 0};}

    assert((int)S1.size() > 0);
    pair <int, int> w = S1.back(); S1.pop_back();
    pair <int, int> r = Reversals.back(); Reversals.pop_back();

    V->reverse(r.first, r.second);
    return w;
}

vector <pair <int, int> > solve(vector <int> pi)
{
    //PREPROCESS WHITE COMPONENTS
    interleavingGraph G = interleavingGraph(pi);
    vector <pair <int, int> > S = G.clearWhiteComponents();
    pi = G.returnPi();

    vector <pair <int, int> > S2, S1, Reversals;
    redbluePath V = redbluePath(pi);

    //WHILE THERE IS A BLACK NODE IN V DO
    while(ToggleAndAppend(&V, S1, Reversals));

    pair <int, int> a = V.activeRed();
    //WHILE V IS NON-EMPTY
    while(!V.emptyV())
    {
        //WHILE THERE IS NO BLACK NODE IN V DO
        while(a.first == 0 && a.second == 0)
        {
            pair <int, int> w = UndoAndRemove(&V, S1, Reversals);
            S2.push_back(w);
            a = V.activeRed();
        }
        //WHILE THERE IS A BLACK NODE IN V DO
        while(ToggleAndAppend(&V, S1, Reversals));

        //IF S2[1] IS WHITE THEN
        if(!S1.empty()) 
        {
            if(V.sameSign(S2.back()))
            {
                UndoAndRemove(&V, S1, Reversals);
            }
        }
        a = V.activeRed();
    }

    for(pair <int, int> i : Reversals) S.push_back(i);
    while(!S2.empty())
    {
        pair <int, int> l = S2.back();
        S.push_back(V.toggle(l.first, l.second)); S2.pop_back();
    }
    return S;
}

int main()
{
   int t;
   cin >> t;
   while(t--)
   {
        vector <int> pi;
        int n;
        cin >> n;
        while(n--)
        {
            int a;
            cin >> a;
            pi.push_back(a);
        }
        vector <pair <int, int> > s = solve(pi);
        for(pair <int, int> i: s) cout << '(' << i.first << ',' << i.second << ") ";
        cout << '\n';

   }
}