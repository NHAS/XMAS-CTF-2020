
#ifndef __SHFT__
#define __SHFT__

#include "../global.h"
#include <string>

class shift : public BaseInstruction
{
private:
    int type, immediate;

public:
    shift(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec)
    {
        this->type = std::bitset<3>(this->C.to_ulong() >> 3).to_ulong();
        this->immediate = std::bitset<3>(this->C.to_string().substr(3)).to_ulong();
    }

    static constexpr int getOpCode()
    {
        return 012;
    }
    virtual std::string getAssembly()
    {

        std::string assembly = std::string(1, this->exec);

        switch (type)
        {
        case 0:
            assembly += "shl";
            break;
        case 8:
            assembly += "shr";
            break;
        case 16:
            assembly += "sar";
            break;

        case 24:
            assembly += "rol";
            break;

        default:
            assembly += "shifting UN (" + std::to_string(this->C.to_ulong()) + ")";
        }

        return assembly + " r" + std::to_string(this->A.to_ulong()) + " r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(immediate);
    }

    virtual int doOp()
    {

        if (type != 0)
        {
            // std::cout << getAssembly() << " " << A << " " << B << " " << C << " " << std::bitset<3>(type) << " " << std::bitset<3>(immediate) << std::endl;
            //Only shl is used, despite a unknown thing being there
            int ra = MainMemory::getInstance().readRegister(this->B).to_ulong();

            nibble result(ra >> this->immediate);

            MainMemory::getInstance().writeRegister(this->A, result);
            return 0;
        }
        //Only shl is used, despite a unknown thing being there
        int ra = MainMemory::getInstance().readRegister(this->B).to_ulong();

        nibble result(ra << this->immediate);

        MainMemory::getInstance().writeRegister(this->A, result);
        return 0;
    }
};

#endif