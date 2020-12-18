
#ifndef __MUL__
#define __MUL__

#include "../global.h"
#include <string>

class multiply : public BaseInstruction
{

private:
    int type;
    int pr;

public:
    multiply(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec)
    {
        type = std::bitset<3>(this->C.to_ulong() >> 3).to_ulong();
        pr = std::bitset<3>(this->C.to_string().substr(3)).to_ulong();
    }

    static constexpr int getOpCode()
    {
        return 023;
    }
    virtual std::string getAssembly()
    {
        std::string s = ((type) ? "s" : "u");
        return std::string(1, this->exec) + "fm" + s + std::to_string(pr) + " r" + std::to_string(this->A.to_ulong()) + " r" + std::to_string(this->B.to_ulong());
    }

    virtual int doOp()
    {
        if (!type) // Logical r shift
        {
            unsigned long internal = MainMemory::getInstance().readRegister(this->A).to_ulong();
            internal *= MainMemory::getInstance().readRegister(this->B).to_ulong();
            internal = internal >> pr;
            MainMemory::getInstance().writeRegister(this->B, nibble(internal));
            return 0;
        }
        unsigned long internal = MainMemory::getInstance().readRegister(this->A).to_ulong();
        internal *= MainMemory::getInstance().readRegister(this->B).to_ulong(); // fms never does a shift, so yay

        MainMemory::getInstance().writeRegister(this->B, nibble(internal));
        return 0;
    }
};

#endif