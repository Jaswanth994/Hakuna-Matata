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
class core
{
public:
    int registers[32];
    int pc;
    void execute(string memory[]);

    core()
    {
        for (int i = 0; i < 32; i++)
        {
            registers[i] = 0;
        }
        pc = 0;
    }
};

void core::execute(string memory[])
{
    processor p;
    // core c;
    int n = memory->size();
    int j = 0;
    while (n-- > 0)
    {
        string t;
        stringstream s1(memory[j]);
        queue<string> s2;
        while (getline(s1, t, ' '))
        {
            s2.push(t);
        }
        int rd, r1, r2;
        if (s2.front() == "add")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int r2 = stoi(s2.front().substr(1));
            // p.cores[0].registers[r1] = 7;
            // p.cores[0].registers[r2] = 8;
            p.cores[0].registers[rd] = p.cores[0].registers[r1] + p.cores[0].registers[r2];
            p.cores[0].pc++;
        }
        if (s2.front() == "sub")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int r2 = stoi(s2.front().substr(1));
            p.cores[0].registers[r1] = 26;
            p.cores[0].registers[r2] = 9;
            p.cores[0].registers[rd] = p.cores[0].registers[r1] - p.cores[0].registers[r2];
            p.cores[0].pc++;
        }
        if (s2.front() == "lw")
        {
            int y;
            s2.pop();
            if (!isdigit(s2.front()[2]))
            {
                rd = int(s2.front()[1] - '0');
            }
            else
                rd = int((s2.front()[1] - '0') * 10 + s2.front()[2] - '0');
            s2.pop();
            if (!isdigit(s2.front()[0]))
            {
                if (!isdigit(s2.front()[2]))
                {
                    r1 = int(s2.front()[1] - '0');
                }
                else
                {
                    r1 = int((s2.front()[1] - '0') * 10 + (s2.front()[2] - '0'));
                }
                s2.pop();
                p.cores[0].registers[r1] = 1000;
                p.memory[p.cores[0].registers[r1]] = "72";
                y = stoi(p.memory[p.cores[0].registers[r1]]);
                p.cores[0].registers[rd] = y;
            }
            else if (isdigit(s2.front()[0]))
            {
                int z = 0;
                int x = 0, r1;
                if (isdigit(s2.front()[1]))
                {
                    x = int((s2.front()[0] - '0') * 10 + (s2.front()[1] - '0'));
                    z = 4;
                }
                else
                {
                    x = int(s2.front()[0] - '0');
                    z = 3;
                }
                if (isdigit(s2.front()[z + 1]))
                {
                    r1 = int((s2.front()[z] - '0') * 10 + (s2.front()[z] - '0') + x);
                }
                else
                    r1 = int((s2.front()[z] - '0') + x);
                p.cores[0].registers[r1] = 1000;
                p.memory[p.cores[0].registers[r1]] = "72";
                y = stoi(p.memory[p.cores[0].registers[r1]]);
                p.cores[0].registers[rd] = y;
            }
            p.cores[0].pc++;
        }
        if (s2.front() == "addi")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int value = stoi(s2.front().substr(0));
            p.cores[0].registers[r1] = 6;
            p.cores[0].registers[rd] = p.cores[0].registers[r1] + value;
            // cout<< p.c[i].registers[rd]<<endl;
            p.cores[0].pc++;
        }
        if (s2.front() == "bne")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            if (rd != r1)
            {
            }
            p.cores[0].pc++;
        }
        j++;
    }
    // cout<<p.cores[0].pc;
    print1(p);
}
class processor
{
public:
    string memory[4096];
    int clock;
    processor()
    {
        for (int i = 0; i < 4096; i++)
        {
            memory[i] = "";
        }
        clock = 0;
    }
    core cores[2] = {core(), core()};
    
    void run();
    void load_Program(string filename, int start_addr);
};

void processor::load_Program(string filename, int start_addr) {
    ifstream file(filename);
    string line;

    int i = start_addr;
    if (file.is_open()) {
        while (getline(file, line) && i < 2048+start_addr) {
            if(line!="")
                memory[i++] = line;
        }
    }
    if(clock<i)
        clock=i;
    file.close();
}

void processor::run(){
    
    


}



void print1(processor p)
{
    cout << "[ ";
    for (int i = 0; i < 32; i++)
    {
        cout << p.cores[0].registers[i] << " ";
    }
    cout << "]" << endl;
}
void print2(processor p)
{
    cout << "[ ";
    for (int i = 0; i < 32; i++)
    {
        cout << p.cores[1].registers[i] << " ";
    }
    cout << "]" << endl;
}

void processor::run()
{
}

int main()
{
    processor sim;

    sim.load_Program("a.txt", 0);
    sim.load_Program("b.txt", 2048);

}