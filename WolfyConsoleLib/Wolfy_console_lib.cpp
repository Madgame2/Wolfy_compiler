#include <iostream>



    // ������� ��� ������ ������
extern "C"  void _stdcall print_string(const char* str) {
        std::cout << str;
    }

    // ������� ��� ������ ������ �����
extern "C" void _stdcall print_int(int num) {
        std::cout << num;
    }

    // ������� ��� ������ ����� ������
extern "C" void _stdcall print_newline() {
        std::cout << std::endl;
    }

