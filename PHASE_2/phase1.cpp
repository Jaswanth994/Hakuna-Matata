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
    int registers[32];                                 // Array to hold the registers
    int pc;                                            // Program counter
    void execute(string instruction, string memory[]); // Method to execute instructions
    void labels(int start_addr, string memory[]);      // Method to extract labels and their addresses
    map<string, int> label_addr;                       // Map to store label-address mappings
    bool forwarding;
    int num_stalls;
    int temp_stall;
    int num_data_hazards;
    int num_control_hazards;
    
    Buffer* Buffers=new Buffer[100];
    // vector<string>IF_ID,ID_Ex,Ex_MEM,MEM_WB;
    bool b1, b2, b3, b4;
    // Constructor to initialize registers and program counter
    core()
    {
        for (int i = 0; i < 32; i++)
        {
            registers[i] = 0;
        }
        pc = 0;
        forwarding = false;
        num_stalls = 0;
        num_data_hazards = 0;
        num_control_hazards = 0;
        b1 = b2 = b3 = b4 = false;
        temp_stall=0;
        
    }
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
    int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
    void Write_Back(core core, string memory[])
    {
        if (core.b4 && core.temp_stall==0)
        {
            string opcode = core.Buffers[i5].opcode;
            if (opcode == "add")
            {
                core.registers[core.Buffers[i5].rd] = core.Buffers[i5].result_exe;
            }
            if (opcode == "sub")
            {
                core.registers[core.Buffers[i5].rd] = core.Buffers[i5].result_exe;
            }
            if (opcode == "addi")
            {
                core.registers[core.Buffers[i5].rd] = core.Buffers[i5].result_exe;
            }
            if (opcode == "lw")
            {
                core.registers[core.Buffers[i5].rd] = core.Buffers[i5].result_mem;
            }
            if (opcode == "sw")
            {
                memory[core.Buffers[i5].result_exe] = core.Buffers[i5].result_mem;
            }
            if (opcode == "bne" || opcode == "beq" || opcode == "blt")
            {
                if (core.Buffers[i5].branch_proceed)
                {
                    core.pc = core.label_addr[core.Buffers[i5].label];
                }
            }
            i5++;
        }
    }
    void Mem(core core, string memory[])
    {
        if (core.b3 && core.temp_stall==0)
        {
            core.b4 = true;
            string opcode = core.Buffers[i4].opcode;
            if (opcode == "lw")
            {
                core.Buffers[i4].result_mem = stoi(memory[core.Buffers[i4].result_mem]);
            }
            if (opcode == "sw")
            {
                core.Buffers[i4].result_mem = core.registers[core.Buffers[i4].rd];
            }
            i4++;
        }
    }
    void Execution(core core)
    {
        if (core.b2 && core.temp_stall==0)
        {
            core.b3 = true;
            string opcode = core.Buffers[i3].opcode;
            if (opcode == "add")
            {
                core.Buffers[i3].result_exe = core.registers[core.Buffers[i3].rs1] + core.registers[core.Buffers[i3].rs2];
            }
            else if (opcode == "sub")
            {
                core.Buffers[i3].result_exe = core.registers[core.Buffers[i3].rs1] - core.registers[core.Buffers[i3].rs2];
            }
            else if (opcode == "addi")
            {
                core.Buffers[i3].result_exe = core.registers[core.Buffers[i3].rs1] + core.Buffers[i3].value;
            }
            else if (opcode == "lw")
            {
                core.Buffers[i3].result_exe = core.registers[core.Buffers[i3].rs1] + core.Buffers[i3].offset;
            }
            else if (opcode == "sw")
            {
                core.Buffers[i3].result_exe = core.registers[core.Buffers[i3].rs1] + core.Buffers[i3].offset;
            }
            else if (opcode == "bne")
            {
                if (core.registers[core.Buffers[i3].rs1] != core.registers[core.Buffers[i3].rs2])
                {
                    core.Buffers[i3].branch_proceed = true;
                }
            }
            else if (opcode == "blt")
            {
                if (core.registers[core.Buffers[i3].rs1] < core.registers[core.Buffers[i3].rs2])
                {
                    core.Buffers[i3].branch_proceed = true;
                }
            }
            else if (opcode == "beq")
            {
                if (core.registers[core.Buffers[i3].rs1] == core.registers[core.Buffers[i3].rs2])
                {
                    core.Buffers[i3].branch_proceed = true;
                }
            }
            else if (opcode == "j")
            {
                core.pc = core.label_addr[core.Buffers[i3].label];
            }
            else if (opcode == "jal")
            {
                core.registers[core.Buffers[i3].rd] = core.pc + 1;
                core.label_addr.insert(make_pair(core.Buffers[i3].str, core.registers[core.Buffers[i3].rd]));
                core.pc = core.label_addr[core.Buffers[i3].label];
            }
            else if (opcode == "la")
            {
                core.registers[core.Buffers[i3].rd] = core.label_addr[core.Buffers[i3].label];
            }
            else if (opcode == "li")
            {
                core.registers[core.Buffers[i3].rd] = core.Buffers[i3].value;
            }
            i3++;
        }
    }

    void Fetch(core core, string memory[])
    {

        if (memory[core.pc] != "" && core.temp_stall==0)
        {
            string Instruction = memory[core.pc];
            core.Buffers[i1].Instruction = Instruction;
            i1++;
            core.b1 = true;
            core.pc++;
        }
    }

    void Ins_decode(core core)
    {

        if(core.temp_stall!=0){
            core.temp_stall--;
        }

        if (core.b1 && core.temp_stall==0)
        {
            core.b2 = true;
            string Instruction = core.Buffers[i2].Instruction;
            string word;
            stringstream ss(Instruction);
            queue<string> tokens;
            while (ss >> word)
            {
                tokens.push(word);
            }
            string opcode = tokens.front();
            core.Buffers[i2].opcode = opcode;
            if (opcode == "add" || opcode == "sub")
            {
                tokens.pop();
                core.Buffers[i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].rs2 = stoi(tokens.front().substr(1));
            }
            else if (opcode == "addi")
            {
                tokens.pop();
                core.Buffers[i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].value = stoi(tokens.front().substr(0));
            }
            else if (opcode == "bne" || opcode == "blt" || opcode == "beq")
            {
                tokens.pop();
                core.Buffers[i2].rs1 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].rs2 = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].label = tokens.front();
            }
            else if (opcode == "j")
            {
                tokens.pop();
                core.Buffers[i2].label = tokens.front();
            }
            else if (opcode == "jal")
            {
                tokens.pop();
                core.Buffers[i2].str = tokens.front();
                core.Buffers[i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].label = tokens.front();
            }
            else if (opcode == "la")
            {
                tokens.pop();
                core.Buffers[i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].label = tokens.front();
            }
            else if (opcode == "li")
            {
                tokens.pop();
                core.Buffers[i2].rd = stoi(tokens.front().substr(1));
                tokens.pop();
                core.Buffers[i2].value = stoi(tokens.front().substr(0));
            }
            else if (opcode == "lw" || opcode == "sw")
            {
                int y;
                tokens.pop();
                if (!isdigit(tokens.front()[2]))
                {
                    core.Buffers[i2].rd = int(tokens.front()[1] - '0');
                }
                else
                    core.Buffers[i2].rd = int((tokens.front()[1] - '0') * 10 + tokens.front()[2] - '0');
                tokens.pop();
                if (isdigit(tokens.front()[0]))
                {
                    int z = 0;
                    int x = 0;
                    if (isdigit(tokens.front()[1]))
                    {
                        core.Buffers[i2].offset = int((tokens.front()[0] - '0') * 10 + (tokens.front()[1] - '0'));
                        z = 4;
                    }
                    else
                    {
                        core.Buffers[i2].offset = int(tokens.front()[0] - '0');
                        z = 3;
                    }
                    if (isdigit(tokens.front()[z + 1]))
                    {
                        core.Buffers[i2].rs1 = int((tokens.front()[z] - '0') * 10 + (tokens.front()[z + 1] - '0'));
                    }
                    else
                        core.Buffers[i2].rs1 = int((tokens.front()[z] - '0'));
                }
            }
            detect_data_Hazard(core);
            i2++;
        }
    }
    void detect_data_Hazard(core core)
              {
                int i=i2-1;
                while(i==i2-4 && i>=0){
                    if(core.Buffers[i2].rs1 == core.Buffers[i].rd || core.Buffers[i2].rs2== core.Buffers[i].rd){
                        core.temp_stall=3-(i2-i-1);
                        core.num_stalls+=core.temp_stall;
                        break;
                    }
                    i--;
                }
             }
};

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
    Buffer(){
        branch_proceed=false;
    }
};

// Method to execute instructions in the core
void core::execute(string instruction, string memory[])
{
    string word;
    stringstream ss(instruction);
    queue<string> tokens;
    while (ss >> word)
    {
        tokens.push(word);
    }
    int rd, rs1, rs2;
    string opcode = tokens.front();

    // Instruction execution based on opcode
    if (opcode == "add")
    {
        // ADD instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs2 = stoi(tokens.front().substr(1));
        registers[rd] = registers[rs1] + registers[rs2];
        pc++;
    }
    else if (opcode == "sub")
    {
        // SUB instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs2 = stoi(tokens.front().substr(1));
        registers[rd] = registers[rs1] - registers[rs2];
        pc++;
    }

    else if (opcode == "addi")
    {
        // addi instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        int value = stoi(tokens.front().substr(0));
        registers[rd] = registers[rs1] + value;
        pc++;
    }

    else if (opcode == "bne")
    {
        // bne instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        if (registers[rd] != registers[rs1])
        {
            string label = tokens.front();
            pc = label_addr[label];
            return;
        }
        pc++;
    }
    else if (opcode == "blt")
    {
        // blt (branch less than ) instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        if (registers[rd] < registers[rs1])
        {
            string label = tokens.front();
            pc = label_addr[label];
            return;
        }
        pc++;
    }
    else if (opcode == "beq")
    {
        // beq (branch equal to) Instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int rs1 = stoi(tokens.front().substr(1));
        tokens.pop();
        if (registers[rd] == registers[rs1])
        {
            string label = tokens.front();
            pc = label_addr[label];
            return;
        }
        pc++;
    }
    else if (opcode == "lw")
    {
        // load word instruction
        int y;
        tokens.pop();
        if (!isdigit(tokens.front()[2]))
        {
            rd = int(tokens.front()[1] - '0');
        }
        else
            rd = int((tokens.front()[1] - '0') * 10 + tokens.front()[2] - '0');
        tokens.pop();
        if (isdigit(tokens.front()[0]))
        {
            int z = 0;
            int x = 0;
            if (isdigit(tokens.front()[1]))
            {
                x = int((tokens.front()[0] - '0') * 10 + (tokens.front()[1] - '0'));
                z = 4;
            }
            else
            {
                x = int(tokens.front()[0] - '0');
                z = 3;
            }
            if (isdigit(tokens.front()[z + 1]))
            {
                rs1 = int((tokens.front()[z] - '0') * 10 + (tokens.front()[z + 1] - '0'));
            }
            else
                rs1 = int((tokens.front()[z] - '0'));
            int i = registers[rs1] + x;
            if (memory[i] != "")
            {
                registers[rd] = stoi(memory[i]);
            }
        }
        pc++;
    }
    else if (opcode == "sw")
    {
        int y, x;
        tokens.pop();
        if (!isdigit(tokens.front()[2]))
        {
            rs1 = int(tokens.front()[1] - '0');
        }
        else
            rs1 = int((tokens.front()[1] - '0') * 10 + tokens.front()[2] - '0');
        tokens.pop();
        if (isdigit(tokens.front()[0]))
        {
            int z = 0;
            x = 0;
            if (isdigit(tokens.front()[1]))
            {
                x = int((tokens.front()[0] - '0') * 10 + (tokens.front()[1] - '0'));
                z = 4;
            }
            else
            {
                x = int(tokens.front()[0] - '0');
                z = 3;
            }
            if (isdigit(tokens.front()[z + 1]))
            {
                rd = int((tokens.front()[z] - '0') * 10 + (tokens.front()[z + 1] - '0'));
            }
            else
                rd = int((tokens.front()[z] - '0'));
            memory[registers[rd] + x] = to_string(registers[rs1]);
        }
        pc++;
    }

    else if (opcode == "j")
    {
        // jump instruction
        tokens.pop();
        string label = tokens.front();
        pc = label_addr[label];
        return;
    }
    else if (opcode == "jal")
    {
        // jump and link instruction
        tokens.pop();
        string str = tokens.front();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        registers[rd] = pc + 1;
        label_addr.insert(make_pair(str, registers[rd]));
        string label = tokens.front();
        pc = label_addr[label];
        return;
    }
    else if (opcode == "la")
    {
        // load address instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        string label = tokens.front();
        registers[rd] = label_addr[label];
        pc++;
    }
    else if (opcode == "li")
    {
        // load immmediate instruction
        tokens.pop();
        int rd = stoi(tokens.front().substr(1));
        tokens.pop();
        int value = stoi(tokens.front().substr(0));
        registers[rd] = value;
        pc++;
    }
    else
    {
        pc++;
    }
}

// Method to load program from file into memory
void processor::load_Program(string filename, int start_addr)
{
    ifstream file(filename);
    string line;
    int i = start_addr;

    // file handling
    if (!file.is_open())
    {
        cout << "Error: Unable to open file " << filename << endl;
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
    cores[0].labels(0, memory);    // Extract labels and their addresses for core 1
    cores[1].labels(2048, memory); // Extract labels and their addresses for core 2
    int flag = 1;
    cores[1].pc = 2048; // Set program counter for core 2

    pipe_line_control pipe_line;

    // Loop to execute instructions until max instructions among both of the cores
    while (clock < 2048 && flag == 1)
    {
        flag = 0;
        // Loop through cores to execute instructions in parallel
        for (int i = 0; i < 2; i++)
        {
            pipe_line.Write_Back(cores[i], memory);
            pipe_line.Mem(cores[i], memory);
            pipe_line.Execution(cores[i]);
            pipe_line.Ins_decode(cores[i]);
            pipe_line.Fetch(cores[i], memory);

            /*if (memory[cores[0].pc] != "" && i == 0)
            {
                cores[0].execute(memory[cores[0].pc], memory); // Execute instruction in core 1
                flag = 1;
            }
            else if (memory[cores[1].pc] != "" && i == 1)
            {
                cores[1].execute(memory[cores[1].pc], memory); // Execute instruction in core 2
                flag = 1;
            }
            */

        }
        clock++; // Increment clock cycle
    }
    cout<<cores[0].num_stalls<<" ;l;;;;;; "<<endl;
    cout<<cores[1].num_stalls<<" ............ "<<endl;
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
    outputFile << "]" << endl;
    outputFile << "\n   Bubble Sort  " << endl;
    for (int i = 1003; i < 1015; i++)
    {
        outputFile << memory[i] << " ";
    }
    outputFile << endl;

    outputFile << "\n   Selection Sort  " << endl;
    for (int i = 3049; i < 3070; i++)
    {
        outputFile << memory[i] << " ";
    }
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

   /* cout << "Enter 1 for data_Forwarding and 2 for NOT_Forwarding : ";
    int forwarding;
    cin >> forwarding;
*/
    sim.run();
    // printing the Executed bubblesort in core1 and selection sort in core2 and contetnts of the both registers
    print("output.txt", sim.cores[0].registers, sim.cores[1].registers, sim.memory, 32);
    // For printing the content present in the memory
    writeStringArrayToFile("memory.txt", sim.memory, 4096);
}