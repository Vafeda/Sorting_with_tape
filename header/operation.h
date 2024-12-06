#ifndef OPERATION_H
#define OPERATION_H

#include <string>

// Интерфейс
class Operation {
public:

    // Фиксированная длинна одной ячейки ленты.
    const int size = 12;

    std::string config_file = "config.txt";
    int delay_write = 0;
    int delay_read = 0;
    int delay_rewinding_tape = 0;
    int delay_next_prev = 0;

    virtual bool next() = 0;
    virtual void prev() = 0;
    virtual int read() = 0;
    virtual void write(int number) = 0;
    virtual int rewindingTheTape(bool pos) = 0;
};

#endif // OPERATION_H