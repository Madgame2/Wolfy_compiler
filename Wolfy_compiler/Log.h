#pragma once
#include"Error.h"
#include <string>
#include <fstream>
#include"in.h"
#include"Params.h"
#include <locale>
#include <codecvt>

using namespace std;

namespace LOG {
	struct LOG
	{
		std::wstring file;
		std::ofstream* stream;

		void close_all();
	};

	LOG init_LOG(Param::Params param);
	
	void WriteLine(std::ostream* stream, string msg);		// Вывода одной строки в протокол char
	void WriteLine(std::ostream* stream, wstring msg);	// Вывода одной строки в протокол wchar_t
	void WriteLog(std::ostream* stream);							// Вывод заголовка протокола
	void WriteParm(std::ostream* stream, Param::Params parm);			// Вывод в протокол информации о входных параметрах
	void WriteIn(std::ostream* stream, in::IN in);					// Вывод в протокол информации о файле
	void WriteError(std::ostream* stream, Error::ERROR error);		// Вывод в протокол информации об ошибке
}