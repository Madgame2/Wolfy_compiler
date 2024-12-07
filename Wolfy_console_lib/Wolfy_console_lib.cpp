#include"Wolfy_console_lib.h"


extern "C"
{
	void _stdcall print_string(const char* str)
	{
		std::cout << str;
	}
	void _stdcall print_int(int vall) {
		std::cout << vall;
	}
	void _stdcall endl() {
		std::cout << std::endl;
	}
}