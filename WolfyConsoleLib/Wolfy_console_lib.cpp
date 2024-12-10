#include <iostream>



    // Функция для вывода строки
extern "C"  void _stdcall print_string(const char* str) {
        std::cout << str;
    }

    // Функция для вывода целого числа
extern "C" void _stdcall print_int(int num) {
        std::cout << num;
    }

    // Функция для вывода новой строки
extern "C" void _stdcall print_newline() {
        std::cout << std::endl;
    }

