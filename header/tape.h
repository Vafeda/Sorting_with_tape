#ifndef TAPE_H
#define TAPE_H

#include <fstream>

#include "operation.h"

class Tape final : public Operation
{
public:

    Tape(std::string file_name);
    ~Tape();


    int global_pos;

    bool Operation::next() override;
    void Operation::prev() override;
    int Operation::read() override;
    void Operation::write(int number) override;
    int Operation::rewindingTheTape(bool pos) override;

private:

    std::fstream file;

    int lenTape();
    void setDelay(int milliseconds);
};

#endif // TAPE_H