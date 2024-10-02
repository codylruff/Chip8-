
#include <iostream>
#include <stdint.h>
#include <vector>
#include <map>

#include "chip8.h"

// region Constructor
chip8::chip8()
{
    LoadOperations();
    soundTimer = 0;
    delayTimer = 0;
}
// region Operations    
void chip8::StartEmulation()
{
    uint16_t nextInstruction;
    delayTimer = 60;
    soundTimer = 60;
    for (uint16_t a = startAddr; a < endAddr; a += 2)
    {
        nextInstruction = (uint16_t)((Memory[PC] << 8) | Memory[PC+1]);
        ExecuteOpCode(nextInstruction);
        --delayTimer;
        --soundTimer;
        PC += 2;
    }
}
void chip8::LoadRom(const std::vector<uint8_t> romData)
{
    PC = startAddr;
    endAddr = sizeof(romData);
    // Load romdata into memory before execution
    for (uint16_t a = startAddr; a < endAddr; a++)
    {
        std::cout << romData[a] << std::endl;
        Memory[a] = romData[a];
    }
    std::cout << "Rom loaded succesfully." << std::endl;
}
void chip8::LoadOperations()
{
    
    switch(opCode){
    0x0: chip8::ClearOrReturn,
    operations[0x1] = &chip8::Jump;
    operations[0x2] = &chip8::CallSubroutine;
    operations[0x3] = &chip8::SkipIfXEqual;
    operations[0x4] = &chip8::SkipIfXNotEqual;
    operations[0x5] = &chip8::SkipIfXEqualY;
    operations[0x6] = &chip8::SetX;
    operations[0x7] = &chip8::AddX;
    operations[0x8] = &chip8::Arithmetic;
    operations[0x9] = &chip8::SkipIfXNotEqualY;
    operations[0xA] = &chip8::SetI;
    operations[0xB] = &chip8::JumpWithOffSet;
    operations[0xC] = &chip8::Rnd;
    operations[0xD] = &chip8::DrawSprite;
    operations[0xE] = &chip8::SkipOnKey;
    operations[0xF] = &chip8::Execute_0xF;
}
void chip8::ExecuteCpuCycle(uint16_t opCode)
{
    chip8::INSTRUCTION data = {
        opCode, // OpCode
        opCode & 0xF000 >> 0xC, // OpId
        opCode & 0x0FFF, // NNN
        opCode & 0x00FF, // KK
        opCode & 0x000F, // N
        opCode & 0x0F00 >> 8, // X
        opCode & 0x00F0 >> 4, // Y
    };
    // Look up the OpCode using  the first nibble and execute.

    operations[&data->OpId](&data);
}

void ExecuteOpCode(uint16_t opCode){
    // todo implement function. change to return error codes.
}

void chip8::Execute_Subroutine(uint16_t addr)
{
    // TODO: Implement subroutine functionality
    // push current address on stack
    //Stack.Push(PC);
    lastAddr = PC;
    ExecuteOpCode(Memory[addr]);
}
void chip8::Execute_ClearScreen()
{
    // TODO: Implement clear screen function
}
void chip8::Execute_ReturnFromSubroutine()
{
    // 00EE	Return from a subroutine
    // TODO: Execute address at top of stack
    PC = lastAddr;
}
void chip8::ClearOrReturn(INSTRUCTION data)
{
    switch(data.OpCode)
    {
        case 0x00E0:
            // 00E0: Clear the screen
            Execute_ClearScreen();
            break;
        case 0x00EE:
            Execute_ReturnFromSubroutine();
            break;
        default:
            // 0NNN: Execute machine language subroutine at address NNN
            Execute_Subroutine(Memory[data.NNN]);
            break;
    }          
}
void chip8::Jump(INSTRUCTION data)
{
    // 1NNN	Jump to address NNN
    PC = data.NNN;
}
void chip8::CallSubroutine(INSTRUCTION data)
{
    // 2NNN	Execute subroutine starting at address NNN
    Execute_Subroutine(Memory[data.NNN]);
}
void chip8::SkipIfXEqual(INSTRUCTION data)
{
    // 3XNN	Skip the following instruction 
    // if the value of register VX equals NN
    
    if (V[data.X] == data.KK)
    {
        PC += 2;
    }
}
void chip8::SkipIfXNotEqual(INSTRUCTION data)
{
    // 4XNN	Skip the following instruction if the value of 
    // register VX is not equal to NN

    if (V[data.X] != data.KK)
    {
        PC += 2;
    }
}
void chip8::SkipIfXEqualY(INSTRUCTION data)
{
    // 5XY0	Skip the following instruction if the 
    // value of register VX is equal to the value of register VY

    if (V[data.X] == V[data.Y])
    {
        PC += 2;
    }
}
void chip8::SetX(INSTRUCTION data)
{
    // 6XNN	Store number NN in register VX
    V[data.X] = data.KK;
}
void chip8::AddX(INSTRUCTION data)
{
    // 7XNN	Add the value NN to register VX
    V[data.X] += data.KK;
}
void chip8::Arithmetic(INSTRUCTION data)
{
    uint16_t sum, diff;
    
    switch(data.N)
    {
        // 8XY0	Store the value of register VY in register VX
        case 0:
            V[data.X] = V[data.Y];
            break;
        // 8XY1	Set VX to VX OR VY
        case 1:
            V[data.X] = (V[data.X] | V[data.Y]);
            break;
        // 8XY2	Set VX to VX AND VY
        case 2:
            V[data.X] = (V[data.X] & V[data.Y]);
            break;
        // 8XY3	Set VX to VX XOR VY
        case 3:
            V[data.X] = (V[data.X] ^ V[data.Y]);
            break;
        // 8XY4 Add the value of register VY to register VX
        // Set VF to 01 if a carry occurs
        // Set VF to 00 if a carry does not occur
        case 4:
            sum = (V[data.X] + V[data.Y]);
            
            if(sum > sizeof(uint8_t))
            {
                V[data.X] = 0xFF; 
                V[0xF] = 0x1;
                break;
            }else
            {
                V[data.X] += V[data.Y];
                V[0xF] = 0x0;
                break;
            }
        // 8XY5	Subtract the value of register VY from register VX
        // Set VF to 00 if a borrow occurs
        // Set VF to 01 if a borrow does not occur
        case 5:
            diff = (V[data.X] - V[data.Y]);
            V[data.X] -= V[data.Y];
            if(diff < sizeof(uint8_t))
            {
                V[0xF] = 0x0;
                break;
            }else
            {
                V[0xF] = 0x1;
                break;
            }
        // 8XY6	Store the value of register VY shifted right one bit in register VX
        // Set register VF to the least significant bit prior to the shift
        case 6:
            uint8_t leastSig = (V[data.Y] & 1);
            V[data.X] = (V[data.Y] >> 1);
            V[0xF] = leastSig;
            break;
        // 8XY7 Set register VX to the value of VY minus VX
        // Set VF to 00 if a borrow occurs
        // Set VF to 01 if a borrow does not occur
        case 7:
            diff = (V[data.X] - (V[data.Y] - V[data.X]));
            V[data.X] -= (V[data.Y] - V[data.X]);
            if(diff < sizeof(uint8_t))
            {
                V[0xF] = 0x0;
                break;
            }else
            {
                V[0xF] = 0x1;
                break;
            }
        // 8XYE Store the value of register VY shifted left one bit in register VX
        // Set register VF to the most significant bit prior to the shift
        case 8:
            uint8_t mostSig = (V[data.Y] & 0x8); // TODO: determine most significant bit
            V[data.X] = (V[data.Y] << 1);
            V[0xF] = mostSig;
            break;
        default:
            // TODO: Implement default functionality
            break;
    }
}
void chip8::SkipIfXNotEqualY(INSTRUCTION data)
{
    
    // 9XY0	Skip the following instruction if the value of register VX is
    // not equal to the value of register VY
    if(V[data.X] != V[data.Y])
    {
        PC += 2;
    }

}
void chip8::SetI(INSTRUCTION data)
{
    // ANNN	Store memory address NNN in register I
    I = data.NNN;
}
void chip8::JumpWithOffSet(INSTRUCTION data)
{
    // BNNN	Jump to address NNN + V0
    PC = (uint16_t)(data.NNN + V[0x0]);
}
void chip8::Rnd(INSTRUCTION data)
{
    // CXNN	Set VX to a random number with a mask of NN

    // Create random number and mask with NN
}
void chip8::DrawSprite(INSTRUCTION data)
{
    // DXYN	Draw a sprite at position VX, VY 
    // with N uint8_ts of sprite data starting at the address stored in I
    // Set VF to 01 if any set pixels are changed to unset, and 00 otherwise

    // TODO: Draw sprites to screen
}
void chip8::SkipOnKey(INSTRUCTION data)
{
    // EX9E	Skip the following instruction if the key corresponding
    // to the hex value currently stored in register VX is pressed

    // TODO: Implement input functionality
    if(keyPressed == V[data.X])
    {
        // Skip next instruction
    }
}
void chip8::Execute_0xF(INSTRUCTION data)
{
    switch(data.KK)
    {
    // FX07	Store the current value of the delay timer in register VX
    case 0x07:
        V[data.X] = (uint8_t)delayTimer;
        break;
    // FX0A	Wait for a keypress and store the result in register VX
    case 0x0A:
        // TODO: no idea how to do this yet.
        break;
    // FX15	Set the delay timer to the value of register VX
    case 0x15:
        delayTimer = V[data.X];
        break;
    // FX18	Set the sound timer to the value of register VX
    case 0x18:
        soundTimer = V[data.X];
        break;
    // FX1E	Add the value stored in register VX to register I
    case 0x1E:
        I += V[data.X];
        break;
    // FX29	Set I to the memory address of the sprite data 
    // corresponding to the hexadecimal digit stored in register VX
    case 0x29:
        I = sprites[V[data.X]];
        break;
    // FX33	Store the binary-coded decimal equivalent of the value
    // stored in register VX at addresses I, I+1, and I+2
    case 0x33:
        Memory[I] = (uint8_t)V[data.X]; 
        Memory[I+1] = (uint8_t)V[data.X];
        Memory[I+2] = (uint8_t)V[data.X];
        break;
    // FX55	Store the values of registers V0 to VX inclusive in memory
    // starting at address I, I is set to I + X + 1 after operation
    case 0x55:
        // TODO: ????
        break;
    // FX65	Fill registers V0 to VX inclusive with the values stored in 
    // memory starting at address I, I is set to I + X + 1 after operation
    case 0x65:
        // TODO: ????
        break;
    default:
        // TODO: implement default case
        break;
    }
}
// endregion
