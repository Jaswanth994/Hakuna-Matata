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
class Buffer
{
public:
    string Instruction;
    string opcode;
    int rd;
    int rs1, rs2;
    string label;
    int value;
    string str;
    int offset;
    int result_exe;
    int result_mem;
    bool branch_proceed;
    Buffer()
    {
        branch_proceed = false;
    }
};

class core
{
public:
    int registers[32];                                 // Array to hold the registers
    int pc;                                            // Program counter
    void execute(string instruction, string memory[]); // Method to execute instructions
    void labels(int start_addr, string memory[]);      // Method to extract labels and their addresses
    map<string, int> label_addr;                       // Map to store label-address mappings
    int num_Instructions;
    bool forwarding;
    int num_stalls;
    int num_Clock_Cycles;
    int temp_stall;
    int num_data_hazards;
    int num_control_hazards;
    int num_WB;
    int i1,i2,i3,i4,i5;
    bool b1, b2, b3, b4; 
    // Constructor to initialize registers and program counter
    core()
    {
        for (int i = 0; i < 32; i++)
        {
            registers[i] = 0;
        }
        pc = 0;
        num_Instructions=0;
        forwarding = false;
        num_stalls = 0;
        num_Clock_Cycles=0;
        num_data_hazards = 0;
        num_control_hazards = 0;
        num_WB=0;
        b1 = b2 = b3 = b4 = false;
        temp_stall = 0;
        i1=i2=i3=i4=i5=0;
    }
    Buffer Buffers[100];
};
class processor
{
public:
    string memory[4096]; // Array to store program and data memory
    int clock;           // Clock cycles

    core cores[2] = {core(), core()}; // Two cores in the processor

    // Constructor to initialize memory and clock cycles
    processor()
    {
        for (int i = 0; i < 4096; i++)
        {
            memory[i] = "";
        }
        clock = 0;
    }

    void run();                                         // Method to execute the loaded programs
    void load_Program(string filename, int start_addr); // Method to load program from file into memory
    void load_data(string memory[], int start_addr);    // Method to load data into memory
};

class pipe_line_control
{
public:
    void Write_Back(core &core, string memory[])
    {
        if (core.b4 )
        {
           // std::cout << "wb  ";
            string opcode = core.Buffers[core.i5].opcode;
            if (opcode == "add")
            {
                core.registers[core.Buffers[core.i5].rd] = core.Buffers[core.i5].result_exe;
            }
            if (opcode == "sub")
            {
                core.registers[core.Buffers[core.i5].rd] = core.Buffers[core.i5].result_exe;
            }
            if (opcode == "addi")
            {
                core.registers[core.Buffers[core.i5].rd] = core.Buffers[core.i5].result_exe;
            }
            if (opcode == "lw")
            {
                core.registers[core.Buffers[core.i5].rd] = core.Buffers[core.i5].result_mem;
            }
            if (opcode == "sw")
            {
                memory[core.Buffers[core.i5].result_exe] = core.Buffers[core.i5].result_mem;
            }
            if (opcode == "bne" || opcode == "beq" || opcode == "blt")
            {
                if (core.Buffers[core.i5].branch_proceed)
                {
                    core.pc = core.label_addr[core.Buffers[core.i5].label];
                   // core.Buffers[core.i2]=core.Buffers[core.pc];
                }
            }
            core.num_WB++;
            core.i5++;
        }
    }
    void Mem(core &core, string memory[])
    {
        if (core.b3)
        {
           // std::cout << "mem  ";
            core.b4 = true;
            string opcode = core.Buffers[core.i4].opcode;
            if (opcode == "lw")
            {   if(isdigit(memory[core.Buffers[core.i4].result_mem][0]))
                core.Buffers[core.i4].result_mem = stoi(memory[core.Buffers[core.i4].result_mem]);
                else core.Buffers[core.i4].result_mem=0;  }
            if (opcode == "sw")
            {
                core.Buffers[core.i4].result_mem = core.registers[core.Buffers[core.i4].rd];
            }
            core.i4++;
        }
        else{
            core.b4=false;
        }
    }
    void Execution(core &core)
    {
        if (core.b2)
        {
           // std::cout << "exe  ";
            core.b3 = true;
            string opcode = core.Buffers[core.i3].opcode;
            if (opcode == "add")
            {
                core.Buffers[core.i3].result_exe = core.registers[core.Buffers[core.i3].rs1] + core.registers[core.Buffers[core.i3].rs2];
            }
            else if (opcode == "sub")
            {
                core.Buffers[core.i3].result_exe = core.registers[core.Buffers[core.i3].rs1] - core.registers[core.Buffers[core.i3].rs2];
            }
            else if (opcode == "addi")
            {
                //cout<<core.registers[core.Buffers[core.i3].rs1]<<" kkkkkkkk"<<endl;
                core.Buffers[core.i3].result_exe = core.registers[core.Buffers[core.i3].rs1] + core.Buffers[core.i3].value;
            }
            else if (opcode == "lw")
            {
                core.Buffers[core.i3].result_exe = core.registers[core.Buffers[core.i3].rs1] + core.Buffers[core.i3].offset;
            }
            else if (opcode == "sw")
            {
                core.Buffers[core.i3].result_exe = core.registers[core.Buffers[core.i3].rs1] + core.Buffers[core.i3].offset;
            }
            else if (opcode == "bne")
            {
                if (core.registers[core.Buffers[core.i3].rs1] != core.registers[core.Buffers[core.i3].rs2])
                {
                    core.Buffers[core.i3].branch_proceed = true;
                }
            }
            else if (opcode == "blt")
            {
                if (core.registers[core.Buffers[core.i3].rs1] < core.registers[core.Buffers[core.i3].rs2])
                {
                    core.Buffers[core.i3].branch_proceed = true;
                }
            }
            else if (opcode == "beq")
            {
                if (core.registers[core.Buffers[core.i3].rs1] == core.registers[core.Buffers[core.i3].rs2])
                {
                    core.Buffers[core.i3].branch_proceed = true;
                }
            }
            else if (opcode == "j")
            {
                core.pc = core.label_addr[core.Buffers[core.i3].label];
                
            }
            else if (opcode == "jal")
            {
                core.registers[core.Buffers[core.i3].rd] = core.pc + 1;
                core.label_addr.insert(make_pair(core.Buffers[core.i3].str, core.registers[core.Buffers[core.i3].rd]));
                core.pc = core.label_addr[core.Buffers[core.i3].label];
            }
            else if (opcode == "la")
            {
                core.registers[core.Buffers[core.i3].rd] = core.label_addr[core.Buffers[core.i3].label];
            }
            else if (opcode == "li")
            {
                core.registers[core.Buffers[core.i3].rd] = core.Buffers[core.i3].value;
            }
            core.i3++;
        }
        else{
            core.b3=false;
        }
    }

    void Ins_decode(core &core)
    {
        //std::cout<<core.b2<<"frarae"<<core.x<<endl;
        if (core.temp_stall != 0)
        {
            core.temp_stall--;
           /// std::cout<<core.temp_stall<<" ................ "<<core.b1<<endl;
            if(core.b1==false && core.temp_stall==0)
               {
                    core.b1=true;
                   
               } 
            core.b2=false;
        }
       
        if (core.b1 && core.temp_stall == 0)
        {
           // std::cout << "id  " ;
            core.b2 = true;
            string Instruction = core.Buffers[core.i2].Instruction;
            string word;
            if(Instruction!=""){
            stringstream ss(Instruction);
            queue<string> tokens;
            while (ss >> word)
            {
                tokens.push(word);
            }
            string opcode = tokens.front();
            core.Buffers[core.i2].opcode = opcode;
            if (opcode == "add" || opcode == "sub")
            {
                tokens.pop();
                core.Buffers[core.i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].rs2 = stoi(tokens.front().substr(1));
            }
            else if (opcode == "addi")
            {
                tokens.pop();
                core.Buffers[core.i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].value = stoi(tokens.front().substr(0));
            }
            else if (opcode == "bne" || opcode == "blt" || opcode == "beq")
            {
                tokens.pop();
                core.Buffers[core.i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].rs2 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].label = tokens.front();
            }
            else if (opcode == "j")
            {
                tokens.pop();
              //  cout<<tokens.front()<<" ....... "<<endl;
                core.Buffers[core.i2].label = tokens.front();
            }
            else if (opcode == "jal")
            {
                tokens.pop();
                core.Buffers[core.i2].str = tokens.front();
                core.Buffers[core.i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].label = tokens.front();
            }
            else if (opcode == "la")
            {
                tokens.pop();
                core.Buffers[core.i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].label = tokens.front();
            }
            else if (opcode == "li")
            {
                tokens.pop();
                core.Buffers[core.i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[core.i2].value = stoi(tokens.front().substr(0));
            }
            else if (opcode == "lw" || opcode == "sw")
            {
                int y;
                tokens.pop();
                if (!isdigit(tokens.front()[2]))
                {
                    core.Buffers[core.i2].rd = int(tokens.front()[1] - '0');
                }
                else
                    core.Buffers[core.i2].rd = int((tokens.front()[1] - '0') * 10 + tokens.front()[2] - '0');
                tokens.pop();
                if (isdigit(tokens.front()[0]))
                {
                    int z = 0;
                    int x = 0;
                    if (isdigit(tokens.front()[1]))
                    {
                        core.Buffers[core.i2].offset = int((tokens.front()[0] - '0') * 10 + (tokens.front()[1] - '0'));
                        z = 4;
                    }
                    else
                    {
                        core.Buffers[core.i2].offset = int(tokens.front()[0] - '0');
                        z = 3;
                    }
                    if (isdigit(tokens.front()[z + 1]))
                    {
                        core.Buffers[core.i2].rs1 = int((tokens.front()[z] - '0') * 10 + (tokens.front()[z + 1] - '0'));
                    }
                    else
                        core.Buffers[core.i2].rs1 = int((tokens.front()[z] - '0'));
                }
            }
            detect_data_Hazard(core);
            detect_control_Hazard(core);
           /// cout<<core.temp_stall<<" NNNNNNNN"<<endl;
            core.i2++; 
            }
            
            
        }
        else 
        {
            core.b2=false;
        }
    }

    void Fetch(core &core, string memory[])
    {
        stringstream ss(memory[core.pc]);
        string firstword;
        ss >> firstword;
        if(firstword.back()==':' || firstword.front()=='.'){
            core.b1 = true;
            core.pc++;
        }

      if (memory[core.pc] != "" && core.temp_stall == 0 )
        {
           // std::cout << "fetch  " ;
            string Instruction = memory[core.pc];
            core.Buffers[core.i1].Instruction = Instruction;
            cout<<Instruction<<endl;
            core.i1++;
            core.b1 = true;
            core.num_Instructions++;
            core.pc++;
        }
        else{
            core.b1=false;
        }
    
    }
    void detect_control_Hazard(core &core){
      if(core.Buffers[core.i2].opcode=="bne" && !core.forwarding){
                        core.temp_stall=3;
                       // cout<<"Hiii"<<endl;
                        core.num_stalls+=core.temp_stall;
                        core.b2=false;
                    }


    }
    void detect_data_Hazard(core &core)
              {
                int i=core.i2-1;
                
                    if(core.Buffers[core.i2].opcode=="la" && !core.forwarding){
                        core.temp_stall=3;
                        //cout<<"hi.......... "<<endl;core
                        core.num_stalls+=core.temp_stall;   
                    }
                    else if(core.Buffers[core.i2].opcode=="la" && core.forwarding){

                        core.temp_stall=1;
                        core.num_stalls+=core.temp_stall;
                    }

                   
                   else if(i>=0 ){
                    ////cout<<"hi........./////. "<<core.i2<<" "<<i<<endl;
                    cout<<core.Buffers[core.i2].rs1<<" "<<core.Buffers[core.i2].rs2<<" .."<<core.Buffers[i].rd<<endl;
                         if(core.Buffers[core.i2].rs1 == core.Buffers[i].rd || core.Buffers[core.i2].rs2== core.Buffers[i].rd){
            
                            if(!core.forwarding) {
                             ///  cout<<"hhhhhhh"<<endl;
                                core.temp_stall=2;
                                core.num_stalls+=core.temp_stall;
                                core.b2=false;
                            }
                            else if(core.forwarding && core.Buffers[i].opcode=="lw"){
                                core.temp_stall=1;
                                core.num_stalls+=core.temp_stall;
                            }
                            else{
                                if(core.Buffers[core.i2].rs1 == core.Buffers[i].rd){
                                    core.registers[core.Buffers[core.i2].rs1] = core.Buffers[core.i3-1].result_exe;
                                }
                                else if(core.Buffers[core.i2].rs2== core.Buffers[i].rd ){
                                    core.registers[core.Buffers[core.i2].rs2] = core.Buffers[core.i3-1].result_exe; 
                                }
                            }
                        }
                        else if(i>0 && (core.Buffers[core.i2].rs1 == core.Buffers[i-1].rd || core.Buffers[core.i2].rs2== core.Buffers[i-1].rd)){
                            if(!core.forwarding) {
                            core.temp_stall=1;
                            core.num_stalls+=core.temp_stall;
                            core.b2=false;
                            }

                            else if(core.Buffers[core.i2].rs1 == core.Buffers[i-1].rd){
                                core.registers[core.Buffers[core.i2].rs1] = core.Buffers[core.i4-1].result_mem;
                            }

                            else if(core.Buffers[core.i2].rs2== core.Buffers[i-1].rd){
                                core.registers[core.Buffers[core.i2].rs2] = core.Buffers[core.i4-1].result_mem;
                            }

                        }
                   }
                }
};



// Method to load program from file into memory
void processor::load_Program(string filename, int start_addr)
{
    ifstream file(filename);
    string line;
    int i = start_addr;

    // file handling
    if (!file.is_open())
    {
        std::cout << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    if (file.is_open())
    {
        while (getline(file, line) && i < 2048 + start_addr)
        {
            if (line != "")
            {
                memory[i] = line; // Storing each line into the memory
                i++;
            }
        }
    }
    file.close();
}

// Method to load data like strings and arrays into memory
void processor::load_data(string memory[], int start_addr)
{
    // dividing memory from 1000 to 2047 for datasegment part for core1
    int i = 1 + start_addr;
    int j = 1001 + start_addr; // similary parsing 3048 to 4096 memory  for datasegment for core2
    stringstream ss(memory[i]);
    string firstword, secondword;
    ss >> firstword;
    ss >> secondword;

    // Loop to parse data section and load data into memory
    while (firstword != ".text" && memory[i] != "")
    {
        if (secondword == ".word")
        {
            // Loading word data
            stringstream ss(memory[i]);
            string token;
            while (ss >> token)
            {
                if (token != ".word")
                {
                    memory[j] = token;
                    j++;
                }
            }
        }
        else if (secondword == ".string")
        {
            // Loading string data
            stringstream ss(memory[i]);
            string firstword;
            ss >> firstword;
            memory[j] = firstword;
            j++;
            string str = memory[i].substr(memory[i].find_first_of('"') + 1, memory[i].find_last_of('"') - memory[i].find_first_of('"') - 1);
            memory[j] = str;
            j++;
        }
        i++;
        stringstream ss(memory[i]);
        ss >> firstword;
        ss >> secondword;
    }
}

// Method to extract labels and their addresses
void core::labels(int start_addr, string memory[])
{
    for (int i = start_addr; i < start_addr + 2048; i++)
    {
        stringstream ss(memory[i]);
        string firstword;
        ss >> firstword;
        if (!firstword.empty() && firstword.back() == ':')
        {
            // Extracting label name
            firstword = firstword.substr(0, firstword.size() - 1);
            auto it = label_addr.find(firstword);
            if (it != label_addr.end())
            {
                it->second = i + 1; // Storing label address
            }
            else
                label_addr.insert(make_pair(firstword, i + 1)); // Inserting new label-address pair
        }
    }
}

// Method to execute the loaded programs
void processor::run()
{

     std::cout << "IN core 1 \nEnter 1 for data_Forwarding and 0 for NOT_Forwarding : ";
     int forwarding;
     cin >> forwarding;
     cores[0].forwarding=forwarding;
     std::cout << "IN core 2 \nEnter 1 for data_Forwarding and 0 for NOT_Forwarding : ";
     cin>>forwarding;
     cores[1].forwarding=forwarding;
 
    cores[0].labels(0, memory);    // Extract labels and their addresses for core 1
    cores[1].labels(2048, memory); // Extract labels and their addresses for core 2
    cores[1].pc = 2048; // Set program counter for core 2

    pipe_line_control pipe_line;
    int flag1=1,flag2=1;
    // Loop to execute instructions until max instructions among both of the cores
    while (flag1==1 || flag2==1)
    {
        //  Loop through cores to execute instructions in parallel
        if(flag1==1){
            cores[0].num_Clock_Cycles=clock+1;
            //std::cout<<endl;
           // std::cout<<"clock "<< clock+1<<" ===================  " << " core " << 0<<endl;
          // cout<<cores[0].pc<< " kkkkkkkkkkkkk"<<endl;
            pipe_line.Write_Back(cores[0], memory);
            pipe_line.Mem(cores[0], memory);
            pipe_line.Execution(cores[0]);
            pipe_line.Ins_decode(cores[0]);
            pipe_line.Fetch(cores[0], memory);
            if(cores[0].num_Instructions==cores[0].num_WB)
                flag1=0;
        }
        if(flag2==1){
            cores[1].num_Clock_Cycles=clock+1;
            
            pipe_line.Write_Back(cores[1], memory);
            pipe_line.Mem(cores[1], memory);
            pipe_line.Execution(cores[1]);
            pipe_line.Ins_decode(cores[1]);
            pipe_line.Fetch(cores[1], memory);
            if(cores[1].num_Instructions==cores[1].num_WB)
                flag2=0;
        }
        //std::cout<<endl;
        clock++; // Increment clock cycle
    }
}

// For printing the content present in the memory
void writeStringArrayToFile(const string filename, string array[], int size)
{
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
    {
        if (array[i] != "")
            outputFile << "Memory Index--->" << i << "    ----> " << array[i] << endl;
    }
    outputFile.close();
}

// printing the Executed bubblesort in core1 and selection sort in core2 and contetnts of the both registers
void print(const string filename, int a[], int b[], string memory[], int size)
{
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    outputFile << "  Registers of Core 1" << endl;
    outputFile << "[";
    for (int i = 0; i < size; i++)
    {
        outputFile << a[i] << " ";
    }
    outputFile << "]" << endl;
    outputFile << "\n  Registers of Core 2" << endl;
    outputFile << "[";
    for (int i = 0; i < size; i++)
    {
        outputFile << b[i] << " ";
    }
    outputFile << endl;
    outputFile.close();
}

void print1(const string filename,int c0_stall,int c1_stall,int c0_cycles,int c1_cycles,int c0_in,int c1_in,bool c0_fwd,bool c1_fwd){
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    float x;
    outputFile<<"----------------CORE-1---------------"<<endl;
    if(c0_fwd){
        outputFile<<"With Data Forwarding\n";
    }
    else outputFile<<"Without Data Forwarding\n";
    x=c0_in/float(c0_cycles);
    outputFile<<"Num_Stalls in core 1 : "<< c0_stall << endl;
    outputFile<<"Num_clock_Cycles in core 1 : "<< c0_cycles << endl;
    outputFile<<"Num_Instructions in core 1 : "<<c0_in<<endl;
    outputFile<<"Instruction per cycle core 1: "<<x<<endl;
    outputFile<<"-------------------------------------"<<endl;
    outputFile<<endl;
    outputFile<<"----------------CORE-2---------------"<<endl;
    if(c1_fwd){
        outputFile<<"With Data Forwarding\n";
    }
    else outputFile<<"Without Data Forwarding\n";
    x=c1_in/float(c1_cycles);
    outputFile<<"Num_Stalls in core 2 : "<< c1_stall << endl;
    outputFile<<"Num_clock_Cycles in core 2 : "<< c1_cycles << endl;
    outputFile<<"Num_Instructions in core 2 : "<<c1_in<<endl;
    outputFile<<"Instruction per cycle core 2: "<<x<<endl;
    outputFile<<"-------------------------------------"<<endl;
    outputFile << endl;
    outputFile.close();
}

int main()
{
    processor sim;

    // Load programs into memory
    sim.load_Program("test1.txt", 0);
    sim.load_Program("test2.txt", 2048);

    // load data into memory like arrays and strings
    sim.load_data(sim.memory, 0);
    sim.load_data(sim.memory, 2048);

     
    sim.run();
    // printing the Executed bubblesort in core1 and selection sort in core2 and contetnts of the both registers
    print("output.txt", sim.cores[0].registers, sim.cores[1].registers, sim.memory, 32);
    print1("Cores-info-output.txt",sim.cores[0].num_stalls,sim.cores[1].num_stalls,sim.cores[0].num_Clock_Cycles,sim.cores[1].num_Clock_Cycles,sim.cores[0].num_Instructions,sim.cores[1].num_Instructions,sim.cores[0].forwarding,sim.cores[1].forwarding);
    // For printing the content present in the memory
    writeStringArrayToFile("memory.txt", sim.memory, 4096);
}