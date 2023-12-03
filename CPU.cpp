#include "CPU.h"

instruction::instruction(bitset<32> fetch)
{
	//cout << fetch << endl;
	instr = fetch;
	//cout << instr << endl;
}

CPU::CPU()
{
	PC = 0; //set PC to 0
	for (int i = 0; i < 4096; i++) //copy instrMEM
	{
		dmemory[i] = (0);
	}
}

bitset<32> CPU::Fetch(bitset<8> *instmem) { // This function fetches 4 lines in the instruction memory and convert them to a 32-bit instruction
	bitset<32> instr = ((((instmem[PC + 3].to_ulong()) << 24)) + ((instmem[PC + 2].to_ulong()) << 16) + ((instmem[PC + 1].to_ulong()) << 8) + (instmem[PC + 0].to_ulong()));  //get 32 bit instruction
	PC += 4;//increment PC
	return instr;
}


bool CPU::dmemWrite(int tar, int val){
    if (tar > 4095) return false;
    else
    {dmemory[tar] = val; return true;}
}

int CPU::dmemRead(int tar){
    if (tar > 4095) return 0;
    else
        return dmemory[tar];
}


bool CPU::Decode(instruction* curr)
{
    if (curr->instr.to_ulong() == 0) return false;
    //cout<<curr->instr<<endl;
return true;
}

unsigned long CPU::readPC()
{
	return PC;
}

void CPU::changePC(unsigned long tar)
{
    PC = tar;
}

// Add other functions here ... 
