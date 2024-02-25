# Hakuna-Matata

`Designing a Simulator which is capable of executing multi-core environment with two processors under Prof Raghavendra.`

## Our Implementation Proccess for Phase 1
_we Assumed Registers as Integers and memory as String Storing Blocks_

_Divided the memory of 4096 elemnts into Two equal Parts one for each core i.e from 0 to 2047 core1 and 2048 to 4096 core2_

_In the memory allocated for each core also divide into two parts one for storing all instructions and other for storing data segment_

_So for core1 we took 0 to 1000 places in memory for storing instructions and 1000 to 2047 for storing data segment, lly for core2 also_

So in the proccessor class we declared memory and inherited classes like :

load_program ----> to load all instructions into memory

load_data    ----> to load all data segments like array strings into memory

Run         ----> to execute the loaded instructions _paralley from core1 and core2_

In the core We declared 32 registers and inherited classes like :

labels      ----> used a hashmap to store all the labels and their address

execute     ----> to execute all the instructions stored in memory 

Finally printing the memory and sorted output in the memory.txt and output.txt respectively.

## Date:25/02/2024

```
Members: Jaswanth,Nishchith
Decisions:We decided to write the code together for sorting methods.
    Implemented the sorting methods, debugged the code , added output files and memory files
    _Finally Completed Phase1 Together_ 
```

## Date:24/02/2024

```
Members:Jaswanth,Nishchith
Decisions:Jaswanth was assigned for implementing the j, jal, bne instructions and Nishchith was assigned for finding the bugs in code and loading the data.
Jaswanth was successfully implemented the code.
Nishchith was found some bugs in lw and sw implementions and cleared them.
```

## Date:23/02/2024

```
Members:Jaswanth,Nishchith
Decisions:Nishchith Assigned to add run function  in the code.
Jaswanth was assigned for any modifications in the code.
We both are succesfully implented it.
```

## Date:20/02/2024

```
Members:Jaswanth,Nishchith
Decisions:We both discussed about the ideas how to write the code and what functions to implement. Done some modifications in the code to reduce complexity.
```

## Date:18/02/2024

```
Memebers:Jaswanth,Nishchith
Decisions:Started writing code implementing some basic instruction like add sub lw
```
