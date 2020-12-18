#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <streambuf>
#include "global.h"
#include "instructions/arithmetic_logic.h"
#include "instructions/comparison.h"
#include "instructions/flow_control.h"
#include "instructions/io.h"
#include "instructions/memory.h"
#include "instructions/multiply.h"
#include "instructions/shifting.h"
#include "base64.h"
#include <memory>

#include <cmath>
using namespace std;

std::unique_ptr<BaseInstruction> nSplit(const std::string &str);

int main(int argc, char *argv[])
{

    bool decompile = false;
    if (argc <= 1)
    {
        cout << "Please supply rom" << endl;
        return 0;
    }

    if (argc == 3)
    {
        decompile = true;
    }

    ifstream n(argv[1]);
    if (!n.is_open())
    {
        cout << "Could not open file: " << argv[1] << endl;
    }

    vector<std::unique_ptr<BaseInstruction>> instructions;
    int counter = 0;
    while (true)
    {
        char s[4] = {0};
        n.read(s, 4);
        if (n.eof())
        {
            break;
        }

        string decoded_instructions;
        macaron::Base64::Decode(string(s), decoded_instructions);

        string instruction = std::bitset<8>(decoded_instructions[0]).to_string() + std::bitset<8>(decoded_instructions[1]).to_string() + std::bitset<8>(decoded_instructions[2]).to_string();

        auto x = nSplit(instruction);
        counter++;
        if (decompile)
            cout << counter << "." << x->getAssembly() << " (" << x->getOp().to_ulong() << ")" << endl;
        instructions.push_back(std::move(x));
    }
    n.close();

    if (decompile)
        return 0;

    for (int i = 0; i < instructions.size(); i++)
    {
        // cout << instructions[i]->full_bin << endl;

        int jump = instructions[i]->run();
        if (print_instruct)
        {
            cout << i + 1 << "." << instructions[i]->getAssembly() << " " << instructions[i]->getA() << " " << instructions[i]->getB() << " " << instructions[i]->getC() << std::endl;
        }
        // cout << " jmp:  " << jump << endl;

        if (jump != 0)
        {

            int incr = (jump < 0) ? -1 : 1;
            while (true)
            {
                i += incr;
                if (i < 0)
                {
                    i = instructions.size() - 1;
                }

                if (i > instructions.size() - 1)
                {
                    i = 0;
                }
                if (instructions[i]->getOp().to_ulong() == lbl::getOpCode())
                {

                    if (instructions[i]->getExec() == '-' && MainMemory::getInstance().flag || instructions[i]->getExec() == '+' && !MainMemory::getInstance().flag)
                        continue;

                    std::bitset<12> lb(64 * instructions[i]->getA().to_ulong() + instructions[i]->getB().to_ulong());

                    if (std::bitset<18>(lb.to_string() + instructions[i]->getC().to_string()).to_ulong() == abs(jump))
                    {

                        break;
                    }
                }
            }
        }
    }
}

std::unique_ptr<BaseInstruction> nSplit(const std::string &str)
{
    int words = str.length() / WORD_SIZE;
    if (str.length() % WORD_SIZE != 0 || words != 4)
    {
        cout << "Instruction wasnt divisble by " << WORD_SIZE << endl;
        exit(-1);
    }

    char exec = ' ';
    int opc = nibble(str.substr(0, WORD_SIZE)).to_ulong();
    if (opc > 0)
    {
        opc -= 1;

        if (opc >= 42)
        {
            exec = '-';
            opc -= 42;
        }

        if (opc >= 21)
        {
            exec = '+';
            opc -= 21;
        }
    }
    else
    {
        return make_unique<hlt>(str,
                                nibble(opc),
                                nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                exec);
    }

    switch (opc)
    {
    case add::getOpCode():

        return std::make_unique<add>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;
    case addi::getOpCode():

        return std::make_unique<addi>(str,
                                      nibble(opc),
                                      nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                      nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                      nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                      exec);
        break;

    case sub::getOpCode():

        return std::make_unique<sub>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;

    case logic_or::getOpCode():

        return std::make_unique<logic_or>(str,
                                          nibble(opc),
                                          nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                          nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                          nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                          exec);
        break;

    case logic_ori::getOpCode():

        return std::make_unique<logic_ori>(str,
                                           nibble(opc),
                                           nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                           exec);
        break;

    case logic_xor::getOpCode():

        return std::make_unique<logic_xor>(str,
                                           nibble(opc),
                                           nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                           exec);
        break;

    case logic_xori::getOpCode():

        return std::make_unique<logic_xori>(str,
                                            nibble(opc),
                                            nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                            nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                            nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                            exec);
        break;

    case logic_and::getOpCode():

        return std::make_unique<logic_and>(str,
                                           nibble(opc),
                                           nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                           nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                           exec);
        break;

    case logic_andi::getOpCode():

        return std::make_unique<logic_andi>(str,
                                            nibble(opc),
                                            nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                            nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                            nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                            exec);
        break;
    case logic_shiftl::getOpCode():

        return std::make_unique<logic_shiftl>(str,
                                              nibble(opc),
                                              nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                              nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                              nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                              exec);
        break;
    case logic_shiftr::getOpCode():

        return std::make_unique<logic_shiftr>(str,
                                              nibble(opc),
                                              nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                              nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                              nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                              exec);
        break;
    case cmp::getOpCode():

        return std::make_unique<cmp>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;

    case shift::getOpCode():

        return std::make_unique<shift>(str,
                                       nibble(opc),
                                       nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                       nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                       nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                       exec);
        break;

    case ld::getOpCode():
        return std::make_unique<ld>(str,
                                    nibble(opc),
                                    nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                    exec);
        break;

    case st::getOpCode():
        return std::make_unique<st>(str,
                                    nibble(opc),
                                    nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                    exec);
        break;

    case multiply::getOpCode():
        return std::make_unique<multiply>(str,
                                          nibble(opc),
                                          nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                          nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                          nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                          exec);
        break;

    case lbl::getOpCode():
        return std::make_unique<lbl>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;
    case jup::getOpCode():
        return std::make_unique<jup>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;
    case jdn::getOpCode():
        return std::make_unique<jdn>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
        break;

    case io::getOpCode():
        return std::make_unique<io>(str,
                                    nibble(opc),
                                    nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                    nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                    exec);
        break;
    }

    return std::make_unique<unknown>(str,
                                     nibble(opc),
                                     nibble(str.substr(1 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(2 * WORD_SIZE, WORD_SIZE)),
                                     nibble(str.substr(3 * WORD_SIZE, WORD_SIZE)),
                                     exec);
}
