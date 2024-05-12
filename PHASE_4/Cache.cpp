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
    int cachesize;     // cache size
    int blocksize;     // block size
    int numblocks;     // number of blocks in cache
    int associativity; // number of blocks per set
    int sets;

    int L2_cachesize;     // L2 cache size
    int L2_blocksize;     // L2 block size
    int L2_associativity; // L2 number of blocks per set
    int L2_numblocks;     // L2 number of blocks in L2 cache
    int L2_sets;

    vector<list<int>> v;                               // Vector of list helps to track order of block access and each list says about a set in cache.
    vector<unordered_map<int, list<int>::iterator>> m; // Each map says about set in a cache.

    vector<list<int>> v1;
    vector<unordered_map<int, list<int>::iterator>> m2;

    bool missl1 = false;

    pair<int, int> split(int address)
    {
        int offset = (int)log2(sets);
        // address = address >> offset;
        int index = address % sets;
        int tag = address >> ((int)log2(sets));
        // cout << tag << " " << index << " " << offset << " " << sets << endl;
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

        L2_numblocks = (L2_cachesize / L2_blocksize);
        L2_sets = (L2_cachesize / (L2_blocksize * L2_associativity));
        v1.resize(L2_sets);
        m2.resize(L2_sets);

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
        // if tag is not present in cache
        if (m[index].find(tag) == m[index].end())
        { // if set is full it deletes the least recently used tag
            if (v[index].size() == associativity)
            {
                int back = v[index].back();
                v[index].pop_back();
                m[index].erase(back);
            }
            z = false;
        }

        { // if present to delete in the list
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

    bool l2lru(int address)
    {
        int tag;
        int index;
        bool z;
        auto a = split(address);
        tag = a.first;
        index = a.second;

        if (m2[index].find(tag) == m2[index].end())
        {
            if (v1[index].size() == L2_associativity)
            {
                int back = v1[index].back();
                v1.pop_back();
                m2[index].erase(back);
            }

            z = false;
            missl1 = true;
        }
        else
        {
            v1[index].erase(m2[index][tag]);
            z = true;
        }

        v1[index].push_front(tag);
        m2[index][tag] = v1[index].begin();
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