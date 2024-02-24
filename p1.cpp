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
    void execute(string memory);

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
    core cores[2] = {core(), core()};
    
    void run(string memory[]);
    void load_Program(string filename, int start_addr);
    void print();
};
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

void core::execute(string memory)
{   
    int i;
    processor p;
    int j=0;
        string t;
        stringstream s1(memory);
        queue<string> s2;
        while (getline(s1, t, ' '))
        {
            s2.push(t);
        }
        int rd, r1, r2;
        if(j<2048){
            i=0;
        }
        else i=1;
        if (s2.front() == "add")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int r2 = stoi(s2.front().substr(1));
            registers[r1] = 7;
            registers[r2] = 8;
            registers[rd] = registers[r1] + registers[r2];
            p.cores[i].pc++;
        }
        if (s2.front() == "sub")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int r2 = stoi(s2.front().substr(1));
            registers[r1] = 26;
            registers[r2] = 9;
            registers[rd] = registers[r1] - registers[r2];
            pc++;
        }
        if (s2.front() == "lw")
        {
            int y;
            s2.pop();
            /*if (!isdigit(s2.front()[2]))
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
                registers[r1] = 1000;
                p.memory[registers[r1]] = "72";
                y = stoi(p.memory[registers[r1]]);
                registers[rd] = y;
            }*/
            if(isdigit(s2.front()[0]))
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
                registers[r1] = 1000;
                p.memory[registers[r1]] = "72";
                y = stoi(p.memory[registers[r1]]);
                registers[rd] = y;
            }
            p.cores[i].pc++;
        }
        if (s2.front() == "addi")
        {
            s2.pop();
            int rd = stoi(s2.front().substr(1));
            s2.pop();
            int r1 = stoi(s2.front().substr(1));
            s2.pop();
            int value = stoi(s2.front().substr(0));
            registers[r1] = 6;
            registers[rd] =registers[r1] + value;
            // cout<< p.c[i].registers[rd]<<endl;
            p.cores[i].pc++;
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
            p.cores[i].pc++;
        }
}

    


/*class processor
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
};*/

void processor::load_Program(string filename, int start_addr) {
    ifstream file(filename);
    string line;
    int i = start_addr;
    if (file.is_open()) {
        while (getline(file, line) && i < 2048+start_addr) {  
                memory[i] = line;
                i++;
        }
    }
    if(clock<i)
        clock=i;
    file.close();
}

void processor::run(string m[]){
int j=0; 
     while(( j<2048 && memory[j]!="" && memory[j+2048]!="") || (j>=2048 && memory[j]!="" && memory[j-2048]!="")){
        
        if(m[j]!="" && j<2048){
            cores[0].execute(m[j]);
        }
       else if(m[j]!="" && j>=2048){
            cores[1].execute(m[j]);
        }
         if(j<2048){
                j=j+2047;
            }
            else j=j-2048;
            j++;
            }
        /*for(int k=0;k<3;k++){
            cores[0].execute(m[k]);
        }*/
}



int main()
{
    processor sim;
    core c;
    sim.load_Program("a.txt", 0);
    sim.load_Program("b.txt", 2048);
    sim.run(sim.memory);
    cout<<"[";
    for(int i=0;i<32;i++){
        cout<<sim.cores[0].registers[i]<<" ";
    }
    cout<<"]";
    cout<<endl;
    cout<<"[";
    for(int i=0;i<32;i++){
        cout<<sim.cores[1].registers[i]<<" ";
    }
    cout<<"]";
    cout<<endl;
    
    //sim.print();
}