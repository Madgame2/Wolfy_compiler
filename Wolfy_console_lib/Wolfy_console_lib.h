#pragma once
#include<iostream>


extern "C" 
{
	void _stdcall print_string(const char* str);
	void _stdcall print_int(int vall);
	void _stdcall endl();
}