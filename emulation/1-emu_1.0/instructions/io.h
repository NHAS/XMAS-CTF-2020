
#ifndef __IO__
#define __IO__

#include "../global.h"
#include <string>

static int timer;
std::string message = "";

class io : public BaseInstruction
{
private:
    const char lookup[64] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        ' ', '+', '-', '*', '/', '<', '=', '>', '(', ')', '[', ']', '{', '}', '#', '$', '_', '?', '|', '^', '&', '!', '~', ',', '.', ':',
        '\n', 0x00};

    char reverse(char c)
    {
        for (int i = 0; i < 63; i++)
        {
            if (lookup[i] == c)
            {
                return i;
            }
        }
        return -1;
    }

public:
    io(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 022;
    }

    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "io r" + std::to_string(this->A.to_ulong()) + " i" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong());
    }

    virtual int doOp()
    {
        //Lots of faking
        // send rs to device ix and receive rd
        // A = rd
        // B = ix
        // C = rs
        char c = lookup[MainMemory::getInstance().readRegister(this->C.to_ulong()).to_ulong()];

        switch (this->B.to_ulong())
        {

        case 0:
            std::cin >> message;
            message += '\n';
            std::cin.clear();

            MainMemory::getInstance().writeRegister(this->A, nibble(message.size()));

            break;
        case 1:
            if (message.size() > 0)
            {
                MainMemory::getInstance().writeRegister(this->A, nibble(reverse(message[0])));
                message.erase(message.begin());
            }
            else
            {
                MainMemory::getInstance().writeRegister(this->A, nibble(-1));
            }

            break;
        case 2:
            if (c < 0)
            {
                std::cout << "Unkown byte " << MainMemory::getInstance().readRegister(this->C.to_ulong()).to_ulong() << std::endl;
            }
            std::cout << c << std::flush;
            break;
        case 3:
            MainMemory::getInstance().writeRegister(this->A, nibble(timer));
            if (timer < 4095)
                timer++;
            break;

        case 4:
            MainMemory::getInstance().writeRegister(this->A, nibble(timer >> 6));
            if (timer < 4095)
                timer++;
            break;
        }

        return false;
    }
};

#endif