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
    core()
    {
        for (int i = 0; i < 32; i++)
        {
            registers[i] = 0;
        }
        pc = 0;
    }
};

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
    core c[2];
    void execute(string s[]);
    void print();
};
void processor::print()
{
    cout << "[ ";
    for (int i = 0; i < 32; i++)
    {
        cout << c[0].registers[i] << " ";
    }
    cout << "]" << endl;
    cout << "[ ";
    for (int i = 0; i < 32; i++)
    {
        cout << c[1].registers[i] << " ";
    }
    cout << "]" << endl;
}
void processor::execute(string s[])
{
    int n = s->size();
    int j = 0;
    while (n-- > 0)
    {
        int i = 0;
        if (j % 2 == 0)
        {
            i = 0;
        }
        else
            i = 1;
        string t;
        stringstream s1(s[j]);
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
            c[i].registers[r1] = 7;
            c[i].registers[r2] = 8;
            c[i].registers[rd] = c[i].registers[r1] + c[i].registers[r2];

        }
        if (s2.front() == "sub")
        {
              s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int r2 = stoi(s2.front().substr(1));
            c[i].registers[r1] = 26;
            c[i].registers[r2] = 9;
            c[i].registers[rd] = c[i].registers[r1] - c[i].registers[r2];

        }
        if (s2.front() == "LW")
        {   int y;
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
                c[i].registers[r1] =1000;
                memory[c[i].registers[r1]]="72";
                y = stoi(memory[c[i].registers[r1]]);
                c[i].registers[rd] = y;
            }
            else if (isdigit(s2.front()[0]))
            {   int z=0;
                int x = 0,r1;
                if (isdigit(s2.front()[1]))
                {
                    x = int((s2.front()[0] - '0') * 10 + (s2.front()[1] - '0'));
                    z=4;
                }
                else
                {
                    x = int(s2.front()[0] - '0');
                    z=3;
                }
                if(isdigit(s2.front()[z+1])){
                    r1 = int((s2.front()[z]-'0')*10 + (s2.front()[z]-'0')+x); 
                }
                else  r1 = int((s2.front()[z]-'0')+x);
                //cout<<r1;
                c[i].registers[r1] =1000;
                memory[c[i].registers[r1]]="72";
                y = stoi(memory[c[i].registers[r1]]);
                c[i].registers[rd] = y;
            }
        }
        if(s2.front()=="addi"){
             s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int value = stoi(s2.front().substr(0));
            c[i].registers[r1] = 6;
            c[i].registers[rd] = c[i].registers[r1] + value;
        }


        if(s2.front()=="bne"){
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            if(rd!=r1){

            }
        }
        j++;
    }
}

int main()
{
    processor p;
    queue<string> s1;
    s1.push("add X1 X2 X3");
    s1.push("lw X4 X2"); // lw x1 x3
    s1.push("sub X15 X17 X16");
    s1.push("addi X9 x10 111");
    int i = 0;
    while (!s1.empty())
    {
        p.memory[i] = s1.front();
        s1.pop();
        i++;
    }
    p.execute(p.memory);
    p.print();
}