#include <iostream>
#include <string>
#include <cstdlib> 
#include <fstream>
using namespace std;

// Функция сравнивающая output.txt и expected.txt
bool compareFiles(const string& file1, const string& file2) {
    
    ifstream f1(file1, ios::binary);
    ifstream f2(file2, ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        cerr << "Ошибка: Файл не открылся." << endl;
        return false;
    }

    f1.seekg(0, ios::end);
    f2.seekg(0, ios::end);
    streamsize size1 = f1.tellg();
    streamsize size2 = f2.tellg();

    if (size1 != size2) {
        return false;
    }

    f1.seekg(0);
    f2.seekg(0);

    char c1, c2;
    while (f1.get(c1) && f2.get(c2)) {
        if (c1 != c2) {
            return false;
        }
    }

    return true;
}


int main() {
    
    setlocale(LC_ALL, "ru");

    const string input_dir = "unit_test/input/";
    const string output_dir = "unit_test/output/";

    for (int i = 1; i <= 7; ++i) {
 
        string input_file = input_dir + to_string(i) + "_input.txt";
        string expected_output_file = output_dir + to_string(i) + "_expected.txt";
        string output_file = output_dir + to_string(i) + "_output.txt";

        string command = "Sort_Tape.exe " + input_file + " " + output_file;

        int result = system(command.c_str());

        if (result == -1) {
            cerr << "Ошибка: Команда не выполнена." << endl;
            return 1;
        }

        if (compareFiles(expected_output_file, output_file)) {
            cout << "Тест пройден для input " << i << ": сортировка выполнена корректно." << endl;
        }
        else {
            cout << "Тест не пройден для input " << i << ": сортировка выполнена некорректно." << endl;
        }
    }

    return 0;
}

