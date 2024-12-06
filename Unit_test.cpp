#include <iostream>
#include <string>
#include <fstream>

bool compareFiles(const std::string& file1, const std::string& file2) {
    
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Файл '" + file1 + "' или '" + file2 + "' не открылся.\n";
        return false;
    }

    f1.seekg(0, std::ios::end);
    std::streamsize size1 = f1.tellg();
    f2.seekg(0, std::ios::end);
    std::streamsize size2 = f2.tellg();

    // Cравнение длинны обоих файлов.
    if (size1 != size2) {
        return false;
    }

    f1.seekg(0);
    f2.seekg(0);
    
    // Cравнение двух файлов посимвольно.
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

    const std::string input_dir = "tests/data/input/";
    const std::string output_dir = "tests/data/output/";

    for (int i = 1; i <= 7; ++i) {
 
        std::string input_file = input_dir + "input_" + std::to_string(i) + ".txt";
        std::string expected_file = input_dir + "expected_" + std::to_string(i) + ".txt";
        std::string output_file = output_dir + "output_" + std::to_string(i) + ".txt";

        std::string command = "Sort_tape.exe " + input_file + " " + output_file;

        int result = system(command.c_str());

        if (result == -1) {
            std::cerr << "Команда '" + command + "' не выполнена.\n";
            return 1;
        }

        if (compareFiles(expected_file, output_file)) {
            std::cout << "Тест " << i << ": сортировка выполнена корректно.\n";
        }
        else {
            std::cout << "Тест " << i << ": сортировка выполнена некорректно.\n";
        }
    }

    return 0;
}

