
#ifndef __FLOW__
#define __FLOW__

#include "../global.h"
#include <string>

class lbl : public BaseInstruction
{

public:
    lbl(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 017;
    }

    virtual std::string getAssembly()
    {
        int lab = 64 * this->A.to_ulong() + this->B.to_ulong();
        return std::string(1, this->exec) + "lbl " + std::to_string(lab) + " l" + std::to_string(this->C.to_ulong());
    }

    virtual int doOp()
    {
        return false;
    }
};

class jup : public BaseInstruction
{
private:
    int lab;

public:
    jup(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec)
    {
        lab = 64 * this->A.to_ulong() + this->B.to_ulong();
    }

    static constexpr int getOpCode()
    {
        return 020;
    }

    virtual std::string getAssembly()
    {
        unsigned long lc = MainMemory::getInstance().readRegister(this->C).to_ulong();

        return std::string(1, this->exec) + "jup " + std::to_string(lab) + " d" + std::to_string(lc);
    }

    virtual int doOp()
    {
        std::bitset<12> lb(lab);

        return -std::bitset<18>(lb.to_string() + MainMemory::getInstance().readRegister(this->C).to_string()).to_ulong();
    }
};

class jdn : public BaseInstruction
{
private:
    int lab;

public:
    jdn(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec)
    {
        lab = 64 * this->A.to_ulong() + this->B.to_ulong();
    }

    static constexpr int getOpCode()
    {
        return 021;
    }

    virtual std::string getAssembly()
    {
        std::string lc = std::to_string(MainMemory::getInstance().readRegister(this->C).to_ulong());
        return std::string(1, this->exec) + "jdn " + std::to_string(lab) + " d" + lc;
    }

    virtual int doOp()
    {
        std::bitset<12> lb(lab);

        return std::bitset<18>(lb.to_string() + MainMemory::getInstance().readRegister(this->C).to_string()).to_ulong();
    }
};

#endif