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
    void execute(string instruction, string memory[]);
    void labels(int start_addr, string memory[]);
    map<string, int> label_addr;
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

    void run();
    void load_Program(string filename, int start_addr);
};

void core::execute(string instruction, string memory[])
{
    string word;
    stringstream s1(instruction);
    queue<string> s2;
    while (s1 >> word)
    {
        s2.push(word);
    }
    int rd, r1, r2;
    std::cout << s2.front() << " /////////" << endl;
    if (s2.front() == "add")
    {
        s2.pop();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        int r1 = stoi(s2.front().substr(1));
        s2.pop();
        int r2 = stoi(s2.front().substr(1));
        registers[rd] = registers[r1] + registers[r2];
        pc++;
    }
    else if (s2.front() == "sub")
    {
        s2.pop();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        int r1 = stoi(s2.front().substr(1));
        s2.pop();
        int r2 = stoi(s2.front().substr(1));
        registers[rd] = registers[r1] - registers[r2];
        pc++;
    }

    else if (s2.front() == "addi")
    {
        s2.pop();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        int r1 = stoi(s2.front().substr(1));
        s2.pop();
        int value = stoi(s2.front().substr(0));
        registers[rd] = registers[r1] + value;
        pc++;
    }
    
    else if (s2.front() == "bne")
    {
        s2.pop();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        int r1 = stoi(s2.front().substr(1));
        s2.pop();
        if (registers[rd] != registers[r1])
        {
            string label = s2.front();
            pc = label_addr[label];
            return;
        }
        pc++;
    }
    else if (s2.front() == "lw")
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
        if (isdigit(s2.front()[0]))
        {
            int z = 0;
            int x = 0;
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
                r1 = int((s2.front()[z] - '0') * 10 + (s2.front()[z+1] - '0'));
            }
            else
            r1 = int((s2.front()[z] - '0'));
            std::cout<<memory[1004]<<endl;
            std::cout<<r1<<endl;
            int i = registers[r1]+x;
            std::cout<<i;
            if(memory[i]!=""){
            registers[rd]=stoi(memory[i]);}
            else registers[rd]=0;
        }
        pc++;
    }
     else if(s2.front()=="sw"){
            int y;
            s2.pop();
            if (!isdigit(s2.front()[2]))
            {
                r1 = int(s2.front()[1] - '0');
            }
            else
                r1 = int((s2.front()[1] - '0') * 10 + s2.front()[2] - '0');
            s2.pop();
            if (isdigit(s2.front()[0]))
        {
            int z = 0;
            int x = 0;
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
                rd = int((s2.front()[z] - '0') * 10 + (s2.front()[z+1] - '0'));
            }
            else
                rd = int((s2.front()[z] - '0'));
                memory[registers[rd]+x]=to_string(registers[r1]);
        }
        pc++;
    }
    else if (s2.front() == "beq")
    {
        s2.pop();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        int r1 = stoi(s2.front().substr(1));
        s2.pop();
        if (registers[rd] == registers[r1])
        {
            string label = s2.front();
            pc = label_addr[label];
            return;
        }
        pc++;
    }

    else if (s2.front() == "j")
    {
        s2.pop();
        string label = s2.front();
        pc = label_addr[label];
        return;
    }
    else if (s2.front() == "jal")
    {
        s2.pop();
        string str=s2.front();
        int rd = stoi(s2.front().substr(1));
        s2.pop();
        registers[rd] = pc + 1;
        label_addr.insert(make_pair(str,registers[rd]));
        string label = s2.front();
        pc = label_addr[label];
        return;
    }
    else
    {
        pc++;
    }
}

void core::labels(int start_addr, string memory[])
{
    for (int i = start_addr; i < start_addr + 2048; i++)
    {
        stringstream ss(memory[i]);
        string firstword;
        ss >> firstword;
        // std::cout<<"hiiii"<<"    firstword"<<endl;
        if (!firstword.empty() && firstword.back() == ':')
        {
            // std::cout << firstword << endl;
            firstword = firstword.substr(0, firstword.size() - 1);
            // std::cout << firstword << " 99999999   "<<i<<endl;
            label_addr.insert(make_pair(firstword, i + 1));
        }
    }
}

void processor::load_Program(string filename, int start_addr)
{
    ifstream file(filename);
    string line;
    int i = start_addr;
    if (file.is_open())
    {
        while (getline(file, line) && i < 2048 + start_addr)
        {
            if (line != "")
            {
                memory[i] = line;
                i++;
            }
        }
    }
    file.close();
}

void processor::run()
{
    cores[0].labels(0, memory);
    cores[1].labels(2048, memory);
    int flag = 1;
    while (clock < 2048 && flag == 1)
    {
        flag = 0;
        for (int i = 0; i < 2; i++)
        {
            if (memory[cores[0].pc] != "" && i == 0)
            {
                cores[0].execute(memory[cores[0].pc], memory);
                flag = 1;
            }
            else if (memory[cores[1].pc + 2048] != "" && i == 1)
            {
                cores[1].execute(memory[cores[1].pc + 2048], memory);
                flag = 1;
            }
        }
        clock++;
    }
}

int main()
{
    processor sim;
    sim.load_Program("a.txt", 0);
    sim.load_Program("b.txt", 2048);
    sim.cores[0].registers[1] = 3;
    sim.cores[0].registers[2] = 1;
    sim.cores[1].registers[10] = 72;
    sim.cores[1].registers[11] = 2;
    sim.cores[0].registers[27] = 2000;
    sim.memory[2000] = "333";
    sim.run();
    std::cout << "[";
    for (int i = 0; i < 32; i++)
    {
        std::cout << sim.cores[0].registers[i] << " ";
    }
    std::cout << "]";
    std::cout << endl;
    std::cout << "[";
    for (int i = 0; i < 32; i++)
    {
        std::cout << sim.cores[1].registers[i] << " ";
    }
    std::cout << "]";
    std::cout << endl;

    // sim.print();
}