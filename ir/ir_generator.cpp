#include "ir_generator.h"
#include <iostream>

IRGenerator::IRGenerator()
{
    tempCount = 0;
    labelCount = 0;
}

std::string IRGenerator::newTemp()
{
    tempCount++;
    return "t" + std::to_string(tempCount);
}

std::string IRGenerator::newLabel()
{
    labelCount++;
    return "L" + std::to_string(labelCount);
}

void IRGenerator::emit(const std::string& instruction)
{
    instructions.push_back(instruction);
}

void IRGenerator::print()
{
    std::cout << "\n===== Intermediate Representation =====\n";

    for (const auto& instruction : instructions)
    {
        std::cout << instruction << std::endl;
    }

    std::cout << "=======================================\n";
}

void IRGenerator::clear()
{
    instructions.clear();
    tempCount = 0;
    labelCount = 0;
}

IRGenerator::Checkpoint IRGenerator::createCheckpoint() const
{
    return {
        instructions.size(),
        tempCount,
        labelCount
    };
}

void IRGenerator::rollback(const Checkpoint& checkpoint)
{
    if (checkpoint.instructionCount <= instructions.size())
    {
        instructions.resize(checkpoint.instructionCount);
    }

    tempCount = checkpoint.tempCount;
    labelCount = checkpoint.labelCount;
}