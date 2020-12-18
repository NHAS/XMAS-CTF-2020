#ifndef __GLBL__
#define __GLBL__

#define WORD_SIZE 6

#include <bitset>
#include <string>
#include <exception>

typedef std::bitset<WORD_SIZE> nibble;

class MainMemory
{
public:
    static MainMemory &getInstance()
    {
        static MainMemory instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
        return instance;
    }

private:
    MainMemory() {} // Constructor? (the {} brackets) are needed here.

    nibble registers[64];

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    MainMemory(MainMemory const &) = delete;
    void operator=(MainMemory const &) = delete;

    bool flag = false;

    bool writeRegister(nibble reg, nibble value)
    {
        if (reg.to_ulong() == 0)
        {
            return true; // Writing to r0 does nothing
        }

        if (reg.to_ulong() < 0 || reg.to_ulong() > 63)
        {
            throw std::runtime_error("Instruction wrote into invalid memory space: " + std::to_string(reg.to_ulong()));
        }

        registers[reg.to_ulong()] = value;

        return true;
    }

    nibble readRegister(nibble reg)
    {
        if (reg.to_ulong() == 0)
        {
            return nibble(0);
        }

        if (reg.to_ulong() < 0 || reg.to_ulong() > 63)
        {
            throw std::runtime_error("Instruction wrote into invalid memory space: " + std::to_string(reg.to_ulong()));
        }

        return registers[reg.to_ulong()];
    }
};

class BaseInstruction
{

public:
    BaseInstruction() {}

    BaseInstruction(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : full_bin(binary)
    {
        this->OP = _OP;
        this->A = _A;
        this->B = _B;
        this->C = _C;
        this->exec = _exec;
    }

    virtual std::string getAssembly() = 0;

    virtual int doOp() = 0;

    nibble getOp()
    {
        return OP;
    }

    int run()
    {
        if (this->exec == '-' && MainMemory::getInstance().flag || this->exec == '+' && !MainMemory::getInstance().flag)
            return 0;

        return this->doOp();
    }

    nibble getA()
    {
        return A;
    }

    nibble getB()
    {
        return B;
    }

    nibble getC()
    {
        return C;
    }

    char getExec()
    {
        return exec;
    }

    const std::string full_bin;

protected:
    nibble OP;
    nibble A;
    nibble B;
    nibble C;

    char exec;
};

class unknown : public BaseInstruction
{
public:
    unknown(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    virtual std::string getAssembly()
    {
        return "unknown instruction: " + this->full_bin;
    }
    virtual int doOp()
    {
        exit(0);
        return false;
    }
};

class hlt : public BaseInstruction
{
public:
    hlt(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    virtual std::string getAssembly()
    {
        return "hlt";
    }
    virtual int doOp()
    {
        exit(0);
        return false;
    }
};

bool print_instruct = false;

#endif