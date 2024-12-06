#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include "tape.h"

Tape::Tape(std::string file_name) : global_pos(0) {

    file.open(file_name, std::ios::in | std::ios::out);

    if (!file.is_open()) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть и создать файл '" + file_name + "'\n";
        }
    }

    // Чтение конфигурационного файла
    std::ifstream cfg(config_file);
    if (cfg) {
        cfg >> delay_write >> delay_read
            >> delay_rewinding_tape >> delay_next_prev;
    }
    cfg.close();
}

Tape::~Tape() {
    file.close();
}

//  Метод сдвига ленты вправо на одну позицию
bool Tape::next() {
    setDelay(delay_next_prev);
    global_pos += 1;
    if (global_pos * size >= lenTape()) {
        return false; 
    }
    else { 
        return true; 
    }
}

//  Метод сдвига ленты влево на одну позицию
void Tape::prev() {
    setDelay(delay_next_prev);
    global_pos -= 1;
    if (global_pos < 0) { 
        global_pos = 0; 
    }
}

//  Метод чтения элемента с ленты
int Tape::read() {
    setDelay(delay_read);
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

//  Метод записи элемента на ленту
void Tape::write(int number) {
    setDelay(delay_write);
    if (file.is_open()) {
        file << std::left << std::setw(size) << std::setfill(' ') << number;
        file.flush();
    }
}

/*
    Метод перемотки ленты в крайние положения.
    0 - крайнее левое положение.
    1 - крайнее правое положение.
*/
int Tape::rewindingTheTape(bool pos) {
    setDelay(delay_rewinding_tape);
    if (pos) {
        file.seekg(0, std::ios::end);
        global_pos = (int)file.tellg() / size;
        return global_pos;
    }
    else {
        file.seekg(0, std::ios::beg);
        global_pos = 0;
        return global_pos;
    }
}

/*
    Метод подсчета длинны ленты.
    Используется в методе "next" для предотвращения выхода за пределы ленты.
*/
int Tape::lenTape() {
    int max_pos;
    file.seekg(0, std::ios::end);
    max_pos = (int)file.tellg();
    file.seekg(0, std::ios::beg);
    return max_pos;
}

/*
    Метод задержки.
    По умолчанию для каждого публичного метода "Tape" задержка = 0.
    Значение может быть изменено с помощью конфигурационного файла.
*/
void Tape::setDelay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}