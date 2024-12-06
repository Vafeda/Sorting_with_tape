#include <iostream>
#include <filesystem>

#include "tape.h"
#include "sort.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    if (argc > 3) {
        std::cerr << "Введено больше трех аргументов";
        return 1;
    }
    else if (argc < 3) {
        std::cerr << "Введено меньше трех аргументов";
        return 1;
    }


    std::string input_file = argv[1], output_file = argv[2];

    if (!std::filesystem::exists(input_file)) {
        std::cerr << "Входной файл '" << input_file
        << "' не существует.\n"
        << "Убедитесь в точности указанного пути до '" << input_file << "'\n";
        return 1;
    }

    if (!std::filesystem::exists(output_file)) {
        std::cerr << "Выходного файла '" << output_file
        << "' не существует." << std::endl
        << "Убедитесь в точности указанного пути до '" << output_file << "'"
        << std::endl;
        return 1;
    }
    else if (std::filesystem::file_size(output_file) != 0) {
        std::filesystem::remove(output_file);
    }

    Tape tin(input_file), tout(output_file);
    Sorting s(&tin, &tout);
    s.sort();
    return 0;
}