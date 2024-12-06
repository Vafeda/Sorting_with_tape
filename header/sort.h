#ifndef SORT_H
#define SORT_H

#include "tape.h"

// Класс сортировки
class Sorting
{
public:

    Sorting(Tape* in, Tape* out);
    ~Sorting();

    void sort();

private:

    std::string name_tmp_dir = "tmp";

    Tape* input;
    Tape* output;

    void sortTmpTape(int n, Tape* file);
    void mergeSortMas(int* a, int N);

};
#endif // SORT_H
