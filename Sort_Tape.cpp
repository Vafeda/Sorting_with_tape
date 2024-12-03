#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdio>
#include <sys/stat.h>
#include <filesystem>
using namespace std;

// Интерфейс ленты.
class Operation {
public:

    // Фиксированная длинна одной ячейки в ленте.
    const int size = 12;

    string config_file = "config.txt";
    int delay_Write = 0;
    int delay_Read = 0;
    int delay_Rewinding_tape = 0;
    int delay_Next_Prev = 0;

    virtual int Read() = 0;
    virtual void Write(int number) = 0;
    virtual bool Next() = 0;
    virtual bool Prev() = 0;
    virtual int Rewinding_the_tape(bool side) = 0;
};

// Класс эмулирующий работу ленты.
class Tape : public Operation
{
public:

    Tape(string file_name) {

        file.open(file_name, ios::in | ios::out);

        if (!file.is_open()) {
            file.open(file_name, ios::in | ios::out | ios::trunc);
            if (!file.is_open()) {
                cout << "Не удалось открыть или создать файл!" << endl;
            }
        }

        // Чтение конфигурационного файла
        ifstream cfg(config_file);
        if (cfg) {
            cfg >> delay_Write >> delay_Read
                >> delay_Rewinding_tape >> delay_Next_Prev;
        }
        cfg.close();

    }

    ~Tape() {
        file.close();
    }

    int global_pos = 0;

    /*
        Движение ленты на одну позицию вправо.
    */
    bool Next() override {
        Delay(delay_Next_Prev);
        global_pos += 1;
        if (global_pos * 12 >= Max_pos()) { return false; }
        else { return true; }
    }

    /*
        Движение ленты на одну позицию влево.
    */
    bool Prev() override {
        Delay(delay_Next_Prev);
        global_pos -= 1;
        if (global_pos < 0) { global_pos = 0; }
        return true;
    }

    /*
        Чтение числа.
        Возвращает прочитанное число.
        Чтение происходит, если файл открыт.
    */
    int Read() override {
        Delay(delay_Read);
        if (file.is_open()) {
            int number;
            file.seekg(global_pos * size);
            char* buffer = new char[size];
            file.get(buffer, size);
            number = atoi(buffer);
            delete[] buffer;
            return number;
        }
    }

    /*
        Запись числа.
        Принимает на вход число.
        Запись происходит, если файл открыт.
    */
    void Write(int number) override {
        Delay(delay_Write);
        if (file.is_open()) {
            file << std::left << std::setw(12) << std::setfill(' ') << number;
            file.flush();
        }
    }

    /*
        Перемотка ленты.
        Возвращает глобальную позицию в файле.
        При true передвигает ленту в конец.
        При false передвигает ленту в начало.
    */
    int Rewinding_the_tape(bool side) override {
        Delay(delay_Rewinding_tape);
        if (side) {
            file.seekg(0, ios::end);
            global_pos = (int)file.tellg() / size;
            return global_pos;
        }
        else {
            file.seekg(0, ios::beg);
            global_pos = 0;
            return global_pos;
        }
    }

private:

    fstream file;

    /*
        Поиск конца файла.
        Ограничивает движение ленты вправо.
    */
    int Max_pos() {
        int max_pos;
        file.seekg(0, ios::end);
        max_pos = (int)file.tellg();
        file.seekg(0, ios::beg);
        return max_pos;
    }

    /*
        Задержка.
        На вход получает либо 0, либо число прочитанное из конфигурационного файла.
    */
    void Delay(int milliseconds) {
        auto now = chrono::steady_clock::now();
        auto later = now + std::chrono::milliseconds(milliseconds);
        std::this_thread::sleep_until(later);
    }

};

// Класс сортировки
class Sorting
{
public:

    Sorting(Tape* in, Tape* out) {
        input = in;
        output = out;
    }

    ~Sorting() {
        input = nullptr;
        output = nullptr;
        filesystem::remove_all(name_dir);
    }

    /* 
        Публичный метод сортировки входящего файла.
        Результат будет записан в выходящий файл. 
    */
    void Sort() {

        // Cоздание директории и временных файлов в ней.
        filesystem::create_directory(name_dir);
        string al = name_dir + "/ tmp1.txt";
        string ar = name_dir + "/ tmp2.txt";
        Tape AL(al), AR(ar);

        // Количество чисел в файле.
        int kol = input->Rewinding_the_tape(1);

        input->Rewinding_the_tape(0);
        int n = kol / 2;
        Sort_half_a_file(n, &AL);
        Sort_half_a_file(kol - n, &AR);


        int max_pos_AL = AL.Rewinding_the_tape(1);
        AL.Rewinding_the_tape(0);
        int max_pos_AR = AR.Rewinding_the_tape(1);
        AR.Rewinding_the_tape(0);

        // Чтение и сравнения двух чисел, взятых из tmp1 и tmp2.
        while (max_pos_AL > AL.global_pos && max_pos_AR > AR.global_pos) {
            int number_left = AL.Read(), number_right = AR.Read();
            if (number_left >= number_right) {
                output->Write(number_right);
                AR.Next();
            }
            else {
                output->Write(number_left);
                AL.Next();
            }
        }

        /* 
            Если в одном из файлов закончились числа, 
            то цикл while допишет оставшиеся числа из другого файла.
        */
        while (max_pos_AL > AL.global_pos) {
            output->Write(AL.Read());
            AL.Next();
        }

        while (max_pos_AR > AR.global_pos) {
            output->Write(AR.Read());
            AR.Next();
        }
    }

private:

    string name_dir = "tmp_dir";
    Tape* input;
    Tape* output;

    /* 
        Метод сортирующий файл, содеражащий половину входных чисел.
        Числа записываются в массив, который сортируется.
        Отсортированный массив записывается обратно в файл.
    */
    void Sort_half_a_file(int n, Tape* file) {
        int* a = new int[n];
        for (int i = 0; i < n; i++) {
            a[i] = input->Read();
            input->Next();
        }
        Merge_sort_mas(a, n);
        for (int i = 0; i < n; i++) {
            file->Write(a[i]);
        }
        delete[] a;
    }

    /* 
        Реализация сортировки пузырьком
        Сортирует N = 10^5 за 17 секунд 
        Не используется из-за долгой сортировки по времени.
    */
    void Bubble_sort_mas(int* a, int N) {
        int tmp;
        for (int i = N; i > 0; i--) {
            for (int j = 1; j < i; j++) {
                if (a[j - 1] > a[j]) {
                    tmp = a[j];
                    a[j] = a[j - 1];
                    a[j - 1] = tmp;
                }
            }
        }
    }

    /*
        Реализация сортировки слиянием
        Сортирует N = 10^5 за 6 секунд
        Использует N - 1 оперативной памяти
    */
    void Merge_sort_mas(int* a, int N) {
        if (N <= 1) { return; }

        int n = N / 2;
        int* al = a, * ar = a + n;

        Merge_sort_mas(al, n);
        Merge_sort_mas(ar, N - n);

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
};

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    string input_file = argv[1], output_file = argv[2];

    if (!filesystem::exists(input_file)) {
        cout << "Ошибка: входной файл '" << input_file 
             << "' не существует." << endl 
             << "Убедитесь в точности указанного пути до '" << input_file << "'" 
            << endl;
        return 1;
    }

    if (!filesystem::exists(output_file)) {
        cout << "Ошибка: выходного файла '" << output_file
             << "' не существует." << endl 
             << "Убедитесь в точности указанного пути до '" << output_file << "'"
             << endl;
        return 1;
    }
    else if (filesystem::file_size(output_file) != 0) {
        filesystem::remove(output_file);
    }

    Tape tin(input_file), tout(output_file);
    Sorting s(&tin, &tout);
    s.Sort();
    return 0;
}
