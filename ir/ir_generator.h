#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include <string>
#include <vector>
#include <cstddef>

class IRGenerator
{
private:
    std::vector<std::string> instructions;
    int tempCount;
    int labelCount;

public:
    struct Checkpoint
    {
        std::size_t instructionCount;
        int tempCount;
        int labelCount;
    };

    IRGenerator();

    std::string newTemp();
    std::string newLabel();

    void emit(const std::string& instruction);
    void print();
    void clear();

    // Save the current IR state
    Checkpoint createCheckpoint() const;

    // Restore the IR to a previously saved state
    void rollback(const Checkpoint& checkpoint);
};

#endif