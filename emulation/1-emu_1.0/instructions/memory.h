
#ifndef __MEM__
#define __MEM__

#include "../global.h"
#include <string>

class ld : public BaseInstruction
{
public:
    ld(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 015;
    }
    virtual std::string getAssembly()
    {
        return std::string(1, this->exec) + "ld r" + std::to_string(this->A.to_ulong()) + " [r" + std::to_string(this->B.to_ulong()) + "+" + std::to_string(this->C.to_ulong()) + "]";
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);

        int index = (ra.to_ulong() + this->C.to_ulong()) % 64;

        nibble value = MainMemory::getInstance().readRegister(nibble(index));

        MainMemory::getInstance().writeRegister(this->A, value);

        return 0;
    }
};

class st : public BaseInstruction
{
public:
    st(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 016;
    }
    virtual std::string getAssembly()
    {
        return std::string(1, this->exec) + "st [r" + std::to_string(this->B.to_ulong()) + "+" + std::to_string(this->C.to_ulong()) + "] r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble rs = MainMemory::getInstance().readRegister(this->A);

        nibble ra = MainMemory::getInstance().readRegister(this->B);

        int index = (ra.to_ulong() + this->C.to_ulong()) % 64;

        MainMemory::getInstance().writeRegister(nibble(index), rs);

        return 0;
    }
};

#endif