#ifndef __CMP__
#define __CMP__

#include "../global.h"
#include <string>

class cmp : public BaseInstruction
{

private:
    int type;
    int comparison;

public:
    cmp(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec)
    {
        this->comparison = std::bitset<3>(this->A.to_string().substr(3)).to_ulong();
        this->type = (this->A[3]) ? 3 : (this->A[4]) ? 2 : 0;
    }

    static constexpr int getOpCode()
    {
        return 03;
    }

    virtual std::string getAssembly()
    {
        std::string assembly = std::string(1, this->exec) + "cmp";
        switch (comparison)
        {
        case 0:
            assembly += "tr";
            break;
        case 1:
            assembly += "fa";
        case 2:
            assembly += "eq";
            break;
        case 3:
            assembly += "ne";
            break;
        case 4:
            assembly += "sl";
            break;
        case 5:
            assembly += "sg";
            break;
        case 6:
            assembly += "ul";
            break;
        case 7:
            assembly += "ug";
            break;
        default:
            assembly += "UN";
        }

        if (this->A[3]) // 030 immediate ia rb
        {

            assembly += " i" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong());
            return assembly;
        }

        if (this->A[4]) // 020 immediate ra ib
        {
            assembly += " r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(this->C.to_ulong());
            return assembly;
        }

        // 000 registers
        assembly += " r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong());

        return assembly;
    }

    virtual int doOp()
    {

        int first, second;

        switch (type)
        {
        case 0:

            first = MainMemory::getInstance().readRegister(this->B).to_ulong();
            second = MainMemory::getInstance().readRegister(this->C).to_ulong();

            break;
        case 2:
            first = MainMemory::getInstance().readRegister(this->B).to_ulong();
            second = this->C.to_ulong();

            break;

        case 3:

            first = this->B.to_ulong();
            second = MainMemory::getInstance().readRegister(this->C).to_ulong();

            break;
        }

        switch (comparison)
        {
        case 2: // EQ
            MainMemory::getInstance().flag = first == second;
            break;
        case 3: // NEQ
            MainMemory::getInstance().flag = first != second;
            break;
        case 4: // SLT
            MainMemory::getInstance().flag = first < second;
            break;
        case 5: // SGT
            MainMemory::getInstance().flag = first > second;
            break;
        case 6: // SGT, ugh this probably isnt right
            MainMemory::getInstance().flag = first < second;
            break;
        case 7: // SGT
            MainMemory::getInstance().flag = first > second;
            break;
        }

        return false;
    }
};

#endif