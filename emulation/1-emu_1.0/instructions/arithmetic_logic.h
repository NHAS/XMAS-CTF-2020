#ifndef __ARITH__
#define __ARITH__

#include "../global.h"
#include <string>

class add : public BaseInstruction
{
public:
    add(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 0;
    }
    virtual std::string getAssembly()
    {
        return std::string(1, this->exec) + "add r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() + rb.to_ulong()));

        return 0;
    }
};

class addi : public BaseInstruction
{
public:
    addi(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 01;
    }
    virtual std::string getAssembly()
    {
        return std::string(1, this->exec) + "addi r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() + this->C.to_ulong()));
        return 0;
    }
};

class sub : public BaseInstruction
{
public:
    sub(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 02;
    }
    virtual std::string getAssembly()
    {
        return std::string(1, this->exec) + "sub r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() - rb.to_ulong()));
        return 0;
    }
};

class logic_or : public BaseInstruction
{
public:
    logic_or(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 04;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "or r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() | rb.to_ulong()));
        return 0;
    }
};

class logic_ori : public BaseInstruction
{
public:
    logic_ori(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 05;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "ori r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() | this->C.to_ulong()));
        return 0;
    }
};

class logic_xor : public BaseInstruction
{
public:
    logic_xor(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 06;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "xor r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);
        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() ^ rb.to_ulong()));
        return 0;
    }
};

class logic_xori : public BaseInstruction
{
public:
    logic_xori(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 07;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "ixor r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);

        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() ^ this->C.to_ulong()));
        return 0;
    }
};

class logic_and : public BaseInstruction
{
public:
    logic_and(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 010;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "and r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);

        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() & rb.to_ulong()));

        return 0;
    }
};

class logic_andi : public BaseInstruction
{
public:
    logic_andi(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 011;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "andi r" + std::to_string(this->B.to_ulong()) + " i" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);

        MainMemory::getInstance().writeRegister(this->A, nibble(ra.to_ulong() & this->C.to_ulong()));

        return 0;
    }
};

class logic_shiftl : public BaseInstruction
{
public:
    logic_shiftl(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 013;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "shl r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        nibble ra = MainMemory::getInstance().readRegister(this->B);
        nibble rb = MainMemory::getInstance().readRegister(this->C);

        int rd = ra.to_ulong() << rb.to_ulong();
        MainMemory::getInstance().writeRegister(this->A, nibble(rd));

        return 0;
    }
};

class logic_shiftr : public BaseInstruction
{
public:
    logic_shiftr(const std::string &binary, nibble _OP, nibble _A, nibble _B, nibble _C, char _exec) : BaseInstruction(binary, _OP, _A, _B, _C, _exec) {}

    static constexpr int getOpCode()
    {
        return 014;
    }
    virtual std::string getAssembly()
    {

        return std::string(1, this->exec) + "shr r" + std::to_string(this->B.to_ulong()) + " r" + std::to_string(this->C.to_ulong()) + " -> r" + std::to_string(this->A.to_ulong());
    }

    virtual int doOp()
    {
        return false;
    }
};

#endif