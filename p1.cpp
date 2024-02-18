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
class core{
    
    public:
            int registers[32];
            int pc;
            stack <string> s;
            core(){
                for(int i=0;i<32;i++){
                    registers[i]=0;
                }
                pc=0;
                stack <string> s;
            }
};

class processor : core{
    
    public:
            int memory[4096];
            int clock;
            processor(){
                for(int i=0;i<4096;i++){
                    memory[i]=0;
                }
                clock=0;
            } 
            core c[2];
            //void run();
            void execute(string s,int i);
            void print();
};
void processor::print(){
    cout<<"[ ";
    for(int i=0;i<32;i++){
        cout<<c[0].registers[i]<<" ";
    }
    cout<<"]"<<endl;
    cout<<"[ ";
    for(int i=0;i<32;i++){
        cout<<c[1].registers[i]<<" ";
    }
    cout<<"]"<<endl;
    
}
void processor::execute(string s,int i){
    string t;
    stringstream s1(s);
    queue <string> s2;
     while (getline(s1, t,' '))
   {
        s2.push(t);   
   }
    if(s2.front()=="ADD"){
        s2.pop();
        int rd = int(s2.front()[1]-'0');
        s2.pop();
        int r1  = int(s2.front()[1]-'0');
        s2.pop();
        int r2= int(s2.front()[1]-'0');
        c[i].registers[r1]=7;
        c[i].registers[r2]=8;
        c[i].registers[rd]= c[i].registers[r1]+ c[i].registers[r2];
    }
    if(s2.front()=="LW"){
        s2.pop();
        int rd = int(s2.front()[1]-'0');
        s2.pop();
       // cout<<s2.front();
       if(!isdigit(s2.front()[0])){
        int r1 = int(s2.front()[1]-'0');
        s2.pop();
        c[i].registers[r1]=7;
        c[i].registers[rd]= c[i].registers[r1];
        }
        else if(isdigit(s2.front()[0])){
            int x = int(s2.front()[0]-'0');
            int r1 = int(s2.front()[3]-'0')+x;
            s2.pop();
            c[i].registers[r1]=7;
            
            c[i].registers[rd]= c[i].registers[r1];
        }
    }
}

int main(){
    processor p;
    queue <string> s1;
    s1.push("ADD X1 X2 X3");
    s1.push("LW X4 5(X0)");
    p.c[0].s.push(s1.front());
    s1.pop();
    p.c[1].s.push(s1.front());
    p.execute(p.c[0].s.top(),0);
     p.execute(p.c[1].s.top(),1);
     p.print();
}