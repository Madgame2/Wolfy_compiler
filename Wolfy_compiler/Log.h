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
	
	void WriteLine(std::ostream* stream, string msg);		// ������ ����� ������ � �������� char
	void WriteLine(std::ostream* stream, wstring msg);	// ������ ����� ������ � �������� wchar_t
	void WriteLog(std::ostream* stream);							// ����� ��������� ���������
	void WriteParm(std::ostream* stream, Param::Params parm);			// ����� � �������� ���������� � ������� ����������
	void WriteIn(std::ostream* stream, in::IN in);					// ����� � �������� ���������� � �����
	void WriteError(std::ostream* stream, Error::ERROR error);		// ����� � �������� ���������� �� ������
}