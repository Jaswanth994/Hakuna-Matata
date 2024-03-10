# Hakuna-Matata


## Our Implementation Proccess for Phase 2
_We Took a New Class Name Pipe_Line_control for pipeling the give code_

---->The Above class Contains functions like Write Back, Memory, Execution, Instruction decode and Instruction_fetch.

---->And in the Inst_Decode function itself we called     Detect_Data_hazard Function to check for any RAW Hazard or WAR hazard etc

---->So If there is any Hazard we assigned Stalls according to the Dependency and Opcode.

_We took another class named Buffer which is used to store all the data that is extracted from different stages of pipelininng like Opcode, rd, rs1, rs2 , offset etc._

_And We Took Input from user for Data Forwarding required or not_

---->If they choose data Forwarding in Buffer class we used a reg_result_mem, reg_result_Ex etc to forward the content with out any stalls..

_Coming to the Pridiction We always took Branch was not Taken and if it was false then it stashes the current executing instruction and Goes to the branch Part_

--->This Prediciton is not deeply implemented in this phase  it will be  executed in the next Phase.

#### Finally Successfully Completed PHASE 2 with minor Exceptions


## Date:10/03/2024

```
Members: Jaswanth,Nishchith
Decisions:We decided to fix the code together for data hazards and data forwarding and other minor errors in Pipe lining.
    Implemented the pipelining and data forwarding as much as possible 
    wrote the  output files and memory files
    _Finally Completed Phase2 Together _ 
```

## Date:09/03/2024

```
Members:Jaswanth,Nishchith
Decisions:Jaswanth was assigned for implementing Data Hazards.
Nishchith was assigned to implement the Data Forwarding

Started Working on it .....
```

## Date:08/03/2024

```
Members:Jaswanth,Nishchith
Decisions:
Jaswanth was assigned to implement the Fetch and Decode Part in the Pipelining.
Nishchith Assigned to implement the Execution, Memory and Write back.

We both succesfully wrote the code with minor errors .
```

## Date:06/03/2024

```
Members:Jaswanth,Nishchith
Decisions: it took us very long time to break the code into different files, but with having many errors, we thought to continue in the same file for this PHASE. 
Discussed about Data Forwarding and pipelinig again for clear understanding.
```

## Date:03/03/2024

```
Memebers:Jaswanth,Nishchith
Decisions: Discussed the ideas on Pipe Line Implementation and thought to break the code into different files for readeability.
```
