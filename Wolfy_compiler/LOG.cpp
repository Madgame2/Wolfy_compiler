#include"Log.h"
#pragma warning(disable:4996)
using namespace std;

namespace LOG {

	LOG init_LOG(Param::Params& param)
	{
		LOG new_log;

		new_log.file = param.log.data ? *param.log.data+L".log" : L"project_log.log";
		new_log.stream = new std::ofstream(new_log.file);

		if (!param.log.data) {
			param.log.data = new wstring[1];
			param.log.data[0] = new_log.file;
		}
		if (!new_log.stream->is_open()) {
			throw Error::get_error(112);
		}

		return new_log;
	}
	void LOG::close_all()
	{

		if (stream != nullptr) {
			stream->close();
			
			delete stream;
		}
	}

	void WriteLine(std::ostream* stream, const char* c, ...)
	{
		const char** ptrC = &c;
		while (*ptrC != "")
		{
			*stream << *ptrC;
			ptrC++;
		}
		*stream << endl;
	}

	void WriteLine(std::ostream* stream, string msg)
	{
		*stream << msg;
		*stream << endl;
	}

	// ------- ������ ����� ������ � �������� wchar_t -------
	void WriteLine(std::ostream* stream, wstring msg)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string utf8Msg = converter.to_bytes(msg);

		*stream << utf8Msg;
		
		*stream << endl;
	}

	// ------- ����� ��������� ��������� -------
	void WriteLog(std::ostream* stream)
	{
		char buffer[48];
		time_t rawtime;
		time(&rawtime);							// �������� ������� ����, ���������� � ��������
		tm* timeinfo = localtime(&rawtime);		// ������� ����, �������������� � ���������� �����

		strftime(buffer, 48, "����: %d.%m.%Y %A %H:%M:%S ", timeinfo);

		*stream << "----- �������� ----- " << buffer << endl << endl;
	}

	// ------- ����� � �������� ���������� � ������� ���������� -------
	void WriteParm(std::ostream* stream, Param::Params parm)
	{
		string log;
		string in="";

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		log = converter.to_bytes(*parm.log.data);

		for (int i = 0; i < parm.in.size; i++) {
			in += converter.to_bytes(parm.in.data[i])+" ";
		}

		*stream << "----- ��������� -----" << endl <<
			"-log: " << log << endl <<
			//"-out: " << ptrOut << endl <<
			"-in: " << in << endl << endl;

	}

	//// ------- ����� � �������� ���������� � ����� -------
	//void WriteIn(std::ostream* stream, In::IN in)
	//{
	//	*stream << "----- �������� ������ -----" << endl <<
	//		"���������� ��������\t: " << in.size << endl <<
	//		"���������������\t\t: " << in.ignor << endl <<
	//		"���������� �����\t: " << in.lines << endl << endl;

	//	//delete[] in.text;			//����� ����������� ����� ������� ������
	//}

	// ------- ����� � �������� ���������� �� ������ -------
	void WriteError(std::ostream* stream, Error::ERROR error)
	{
		if (error.data.line == -1)
			*stream << "ERROR " << error.id << ": " << error.error_masage << endl;
		else if (error.id >= 110 && error.id <= 119)
		{
			*stream << "Lexer error" << endl;
			*stream << "ERROR " << error.id << ": " << error.error_masage << ", line " << error.data.line << ", position " << error.data.position << endl;
		}
		else if (error.id >= 600 && error.id <= 610)
		{
			*stream << "parser error" << endl;
			*stream << "ERROR " << error.id << ": " << error.error_masage << ", line " << error.data.line << ", position " << error.data.position << endl;
		}
		else if (error.id >= 300 && error.id <= 310)
		{
			*stream << "Semathic error" << endl;
			*stream << "ERROR " << error.id << ": " << error.error_masage << ", line " << error.data.line << ", position " << error.data.position << endl;

		}
		else {
			*stream << "ERROR " << error.id << ": " << error.error_masage << ", line " << error.data.line << ", position " << error.data.position << endl;
		}
	}
}
