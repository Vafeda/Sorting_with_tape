#include <filesystem>

#include "sort.h"

Sorting::Sorting(Tape* in, Tape* out) : input(in), output(out) {}

Sorting::~Sorting() {
    input = nullptr;
    output = nullptr;
    std::filesystem::remove_all(name_tmp_dir);
}

void Sorting::sort() {

    //  Cоздание временной директории.
    std::filesystem::create_directory(name_tmp_dir);

    //  Создание двух временных лент.
    std::string tmp1 = name_tmp_dir + "/tmp1.txt";
    Tape tape_tmp1(tmp1);
    std::string tmp2 = name_tmp_dir + "/tmp2.txt";
    Tape tape_tmp2(tmp2);

    //  Длина входной ленты, полученной при создании класса.
    int len_in = input->rewindingTheTape(1);
    input->rewindingTheTape(0);

    //  Cортировка каждой временной ленты.
    int half_len = len_in / 2;
    sortTmpTape(half_len, &tape_tmp1);
    sortTmpTape(len_in - half_len, &tape_tmp2);

    //  Длина каждой временной ленты.
    int len_tmp1 = tape_tmp1.rewindingTheTape(1);
    tape_tmp1.rewindingTheTape(0);
    int len_tmp2 = tape_tmp2.rewindingTheTape(1);
    tape_tmp2.rewindingTheTape(0);


    /*
        Цикл сравнивающий два числа взятых с временных лент.
        Если на одной из лент закончились числа цикл прекращается.
        Запись происходит на выходную ленту.
    */
    while (len_tmp1 > tape_tmp1.global_pos && len_tmp2 > tape_tmp2.global_pos) {
        int number_left = tape_tmp1.read(), number_right = tape_tmp2.read();
        if (number_left >= number_right) {
            output->write(number_right);
            tape_tmp2.next();
        }
        else {
            output->write(number_left);
            tape_tmp1.next();
        }
    }

    /*
        Циклы while дописывают числа на выходную ленту, 
        которые еще остались на временных лентах.
    */
    while (len_tmp1 > tape_tmp1.global_pos) {
        output->write(tape_tmp1.read());
        tape_tmp1.next();
    }

    while (len_tmp2 > tape_tmp2.global_pos) {
        output->write(tape_tmp2.read());
        tape_tmp2.next();
    }
}

/*
    Метод сортирует одну временную ленту (половина входной ленты).
    Происходит заполнение массива числами из входной ленты.
    Полученный массив сортируется и записывается на временную ленту.
*/

void Sorting::sortTmpTape(int n, Tape* file) {
    int* tmp = new int[n];
    for (int i = 0; i < n; i++) {
        tmp[i] = input->read();
        input->next();
    }
    mergeSortMas(tmp, n);
    for (int i = 0; i < n; i++) {
        file->write(tmp[i]);
    }
    delete[] tmp;
}

/*
   Сортировка массива с использованием сортировки слиянием.
   Временная сложность данного алгоритма O(N * log N).
   Используется N-1 байт памяти, что не противоречит заданию.
*/

void Sorting::mergeSortMas(int* a, int N) {
    if (N <= 1) { 
        return; 
    }

    int n = N / 2;
    int* al = a;
    int* ar = a + n;

    mergeSortMas(al, n);
    mergeSortMas(ar, N - n);

    int* at = new int[N - 1];
    int il = 0, ir = 0, it;

    for (it = 0; it < N - 1; it++) {
        if (il < n && ir < N - n) {
            if (al[il] < ar[ir]) {
                at[it] = al[il++];
            }
            else {
                at[it] = ar[ir++];
            }
        }
        else {
            if (il < n) {
                at[it] = al[il++];
            }
            if (ir < N - n) {
                at[it] = ar[ir++];
            }
        }
    }

    if (il < n) {
        a[N - 1] = al[n - 1];
    }
    else if (ir < N - n) {
        a[N - 1] = ar[N - n - 1];
    }

    for (it = 0; it < N - 1; it++) {
        a[it] = at[it];
    }

    delete[] at;
}