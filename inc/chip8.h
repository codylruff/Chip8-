#pragma once
#include <stdint.h>
#include <vector>
#include <map>
#define startAddr 0x0200

class chip8{
public:
    chip8();
    ~chip8();
private:
    std::vector<uint8_t> Memory;
    std::vector<uint8_t> V; // 16 8-bit registers V0-F
    std::vector<uint16_t> sprites;
    uint16_t endAddr; // usually 0xFFF
    uint16_t lastAddr;
    uint16_t I;
    uint16_t PC;
    uint8_t soundTimer;
    uint8_t delayTimer;
    uint16_t keyPressed;
    struct INSTRUCTION
    {
        uint16_t OpCode;
        char OpId;
        uint16_t NNN;
        uint8_t KK;
        char N;
        char X;
        char Y;
    };
    typedef void *(operation)(INSTRUCTION* data);
    std::map<char,operation> operations;
private:
    void LoadOperations();
    void LoadRom(const std::vector<uint8_t> romData);
    void StartEmulation();
    void ExecuteCpuCycle(uint16_t opCode);
    
    // OPERATIONS
    void ExecuteOpCode(uint16_t opCode);
    void Execute_Subroutine(uint16_t addr);
    void Execute_ClearScreen();
    void Execute_ReturnFromSubroutine();
    void ClearOrReturn(INSTRUCTION data);
    void Jump(INSTRUCTION data);
    void CallSubroutine(INSTRUCTION data);
    void SkipIfXEqual(INSTRUCTION data);
    void SkipIfXNotEqual(INSTRUCTION data);
    void SkipIfXEqualY(INSTRUCTION data);
    void SetX(INSTRUCTION data);
    void AddX(INSTRUCTION data);
    void Arithmetic(INSTRUCTION data);
    void SkipIfXNotEqualY(INSTRUCTION data);
    void SetI(INSTRUCTION data);
    void JumpWithOffSet(INSTRUCTION data);
    void Rnd(INSTRUCTION data);
    void DrawSprite(INSTRUCTION data);
    void SkipOnKey(INSTRUCTION data);
    void Execute_0xF(INSTRUCTION data);

};