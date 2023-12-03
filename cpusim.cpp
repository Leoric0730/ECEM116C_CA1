#include "CPU.h"

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include<fstream>
#include <sstream>
using namespace std;

/*
Add all the required standard and developed libraries here
*/

/*
Put/Define any helper function/definitions you need here
*/
int main(int argc, char* argv[])
{
	/* This is the front end of your project.
	You need to first read the instructions that are stored in a file and load them into an instruction memory.
	*/

	/* Each cell should store 1 byte. You can define the memory either dynamically, or define it as a fixed size with size 4KB (i.e., 4096 lines). Each instruction is 32 bits (i.e., 4 lines, saved in little-endian mode).
	Each line in the input file is stored as an unsigned char and is 1 byte (each four lines are one instruction). You need to read the file line by line and store it into the memory. You may need a mechanism to convert these values to bits so that you can read opcodes, operands, etc.
	*/

	bitset<8> instMem[4096];

	if (argc < 2) {
		cout << "No file name entered. Exiting...";
		return -1;
	}

    // string filename = "/Users/haoyuluo/Desktop/ECE　M116C/Project/Traces/23instMem-all.txt";
	ifstream infile(argv[1]); //open the file
	if (!(infile.is_open() && infile.good())) {
		cout<<"error opening file\n";
		return 0; 
	}
	string line; 
	int i = 0;
	while (infile) {
			infile>>line;
			stringstream line2(line);
			int x; 
			line2>>x;
			instMem[i] = bitset<8>(x);
			i++;
		}
	int maxPC= i; 
    
    int registers[32];              // Declare 32 registers with initial values of 0.
    for (int i = 0; i < 32; i++)
        registers[i] = 0;
	/* Instantiate your CPU object here.  CPU class is the main class in this project that defines different components of the processor.
	CPU class also has different functions for each stage (e.g., fetching an instruction, decoding, etc.).
	*/

	CPU myCPU;  // call the approriate constructor here to initialize the processor...  
	// make sure to create a variable for PC and resets it to zero (e.g., unsigned int PC = 0); 

	/* OPTIONAL: Instantiate your Instruction object here. */
	//Instruction myInst; 
	bitset<32> curr;
	instruction instr = instruction(curr);
	bool done = true;
    int rtypecount = 0;
    int cyclecount = 0; // Use to count the number of cycle (while loop) executed.
	while (done == true) // processor's main loop. Each iteration is equal to one clock cycle.  
    {
        
        //fetch
        curr = myCPU.Fetch(instMem); // fetching the instruction
        instr = instruction(curr);
        
        // decode
        done = myCPU.Decode(&instr);
        if (done ==false) // break from loop so stats are not mistakenly updated
            break;
        
        // the rest should be implemented here ...
        if ((instr.instr.to_ulong() & 0b1111111) == 0b0110011) {        // Opcode stands for R type instruction
            rtypecount++;
            if (instr.instr[14] == 0) {                                  // either add or sub
                if (instr.instr[30] == 0) {                              // add
                    int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
                    int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
                    int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                    registers[rd_index] = registers[rs1_index] + registers[rs2_index];     // rd = rs1 + rs2
                }
                else                                                      // sub
                {int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
                    int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
                    int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                    registers[rd_index] = registers[rs1_index] - registers[rs2_index];}   // rd = rs1 - rs2
            }
            
            else if(instr.instr[12] == 0){                                // xor
                int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
                int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
                int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                registers[rd_index] = registers[rs1_index] ^ registers[rs2_index];
            }
            else
            {                                                             // SRA
                int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
                int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
                int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                registers[rd_index] = registers[rs1_index] >> (registers[rs2_index] & 0b11111);
            }
        }
        
        else if ((instr.instr.to_ulong() & 0b1111111) == 0b0010011) {   // Opcode stands for I type instruction
            if (instr.instr[14] == 0) {                                 // addi
                int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;
                int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                int imm = static_cast<int>((instr.instr.to_ulong() >> 20) | ((instr.instr[31] ? 0xFFFFF000 : 0x00000000)));
                registers[rd_index] = registers[rs1_index] + imm;
            }
            else {                                                      // andi
                int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;
                int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111;   // xk(rd)
                int imm = static_cast<int>((instr.instr.to_ulong() >> 20) | ((instr.instr[31] ? 0xFFFFF000 : 0x00000000)));
                registers[rd_index] = registers[rs1_index] & imm;
            }
        }
        
        else if ((instr.instr.to_ulong() & 0b1111111) == 0b0000011){
            int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;
            int imm  = (instr.instr.to_ulong() >> 20) & 0b00000111111111111;
            int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111; //  lw
            int mem_tar = imm + registers[rs1_index];
            registers[rd_index] = myCPU.dmemRead(mem_tar);
        }
        
        else if ((instr.instr.to_ulong() & 0b1111111) == 0b0100011){        // sw
            int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
            int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
            int imm1 = (instr.instr.to_ulong() >> 7) & 0b11111;
            int imm2 = static_cast<int>(((instr.instr.to_ulong() >> 25) & 0b1111111) << 5);
            int finImm = imm1 + imm2 + registers[rs1_index];
            myCPU.dmemWrite(finImm,registers[rs2_index]);
        }
        
        else if ((instr.instr.to_ulong() & 0b1111111) == 0b1100111){      //JAlR
            int rd_index  = (instr.instr.to_ulong() >> 7) & 0b00000000000011111; // rd index
            int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;// rs1 index
            int imm  = (instr.instr.to_ulong() >> 20) & 0b00000111111111111;
            registers[rd_index] = static_cast<int>(myCPU.readPC());
            myCPU.changePC(registers[rs1_index]+imm);
        }
        
        else if ((instr.instr.to_ulong() & 0b1111111) == 0b1100011){     //blt
            int rs1_index = (instr.instr.to_ulong() >> 15) & 0b00000000000011111;   // xi(rs1)
            int rs2_index = (instr.instr.to_ulong() >> 20) & 0b00000000000011111;   // xj(rs2)
            int imm1 = static_cast<int>((instr.instr.to_ulong() >> 7) & 0b1) << 11;
            int imm2 = static_cast<int>(((instr.instr.to_ulong() >> 8) & 0b1111) << 1);
            int imm3 = static_cast<int>(((instr.instr.to_ulong() >> 25) & 0b111111) << 5);
            int immOff = imm1 + imm2 + imm3;
            if (registers[rs1_index] < registers[rs2_index]) {myCPU.changePC(myCPU.readPC()+immOff - 4);}
            else {;}
        }
        cyclecount++;
        // sanity check
        if (myCPU.readPC() > maxPC)
            break;
        
    }
	int a0 = registers[10];
	int a1 = registers[11];
	// print the results (you should replace a0 and a1 with your own variables that point to a0 and a1)
	  cout << "(" << a0 << "," << a1 << ")" << endl;
//    cout <<"Total amount of cycles are " << cyclecount << endl;
//    cout <<"R-type instructions number is" << rtypecount << endl;
	return 0;

}
