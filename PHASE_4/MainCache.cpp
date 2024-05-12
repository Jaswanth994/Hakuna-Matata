#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
#include <bits/stdc++.h>
using namespace std;

class cache
{
private:
    int cachesize;
    int blocksize;
    int numblocks;
    int associativity;
    int sets;
    vector<list<int>> v;
    vector<unordered_map<int, list<int>::iterator>> m;
    pair<int, int> split(int address)
    {
        int offset = (int)log2(sets);
        // address = address >> offset;
        int index = address % sets;
        int tag = address >> ((int)log2(sets));
        //cout << tag << " " << index << " " << offset << " " << sets << endl;
        return {tag, index};
    }

public:
    int lruhhit;
    int mruhit;
    int lrumiss;
    int mrumiss;
    cache(int csize, int bsize, int asize) : cachesize(csize), blocksize(bsize), associativity(asize)
    {
        numblocks = (cachesize / blocksize);
        sets = (cachesize / (blocksize * associativity));
        v.resize(sets);
        m.resize(sets);
        lruhhit = 0;
        mruhit = 0;
        lrumiss = 0;
        mrumiss = 0;
    }
    bool lruaccess(int address)
    {
        bool z;
        int tag, index;
        auto a = split(address);
        tag = a.first;
        index = a.second;
        if (m[index].find(tag) == m[index].end())
        {
            if (v[index].size() == associativity)
            {
                int back = v[index].back();
                v[index].pop_back();
                m[index].erase(back);
            }
            z = false;
        }
        else
        {
            v[index].erase(m[index][tag]);
            z = true;
        }

        v[index].push_front(tag);
        m[index][tag] = v[index].begin();
        return z;
    }

    bool mruaccess(int address)
    {
        bool z;
        int tag, index;
        auto a = split(address);
        tag = a.first;
        index = a.second;
        if (m[index].find(tag) == m[index].end())
        {
            if (v[index].size() == associativity)
            {
                int front = v[index].front();
                v[index].pop_front();
                m[index].erase(front);
            }
            z = false;
        }
        else
        {
            v[index].erase(m[index][tag]);
            z = true;
        }
        v[index].push_back(tag);
        m[index][tag] = v[index].end();
        return z;
    }
};

int main()
{
    cache c(64, 16, 1);
    cout << c.lruaccess(0) << endl;
    cout << c.lruaccess(1) << endl;
    cout << c.lruaccess(2) << endl;
    cout << c.lruaccess(3) << endl;
    cout << c.lruaccess(4) << endl;
    cout << c.lruaccess(4) << endl;
    cout << c.lruaccess(0) << endl;
    cout << c.lruaccess(1) << endl;
    cout << "---------------------" << endl;
}