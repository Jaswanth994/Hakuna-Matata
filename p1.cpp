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
            core(){
                for(int i=0;i<32;i++){
                    registers[i]=0;
                }
                pc=0;
            }
};

class processor {
    
    public:
            string memory[4096];
            int clock;
            processor(){
                for(int i=0;i<4096;i++){
                    memory[i]="";
                }
                clock=0;
            } 
            core c[2];
            void execute(string s[]);
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
void processor::execute(string s[]){
    int n = s->size();
    int j=0;
    while(n-->0){
    int i=0;
    if(j%2==0){
        i=0;
    }
    else i=1;
    string t;
    stringstream s1(s[j]);
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
    j++;
   }
}

int main(){
    processor p;
    queue <string> s1;
    s1.push("ADD X1 X2 X3");
    s1.push("LW X4 5(X0)");
    s1.push("ADD X7 X8 X6");
    int i=0;
    while(!s1.empty()){
        p.memory[i]=s1.front();
        s1.pop();
        i++;
    }
   p.execute(p.memory);
    p.print();
}