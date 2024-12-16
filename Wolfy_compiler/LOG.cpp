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

	// ------- Вывода одной строки в протокол wchar_t -------
	void WriteLine(std::ostream* stream, wstring msg)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string utf8Msg = converter.to_bytes(msg);

		*stream << utf8Msg;
		
		*stream << endl;
	}

	// ------- Вывод заголовка протокола -------
	void WriteLog(std::ostream* stream)
	{
		char buffer[48];
		time_t rawtime;
		time(&rawtime);							// получить текущую дату, выраженную в секундах
		tm* timeinfo = localtime(&rawtime);		// текущая дата, представленная в нормальной форме

		strftime(buffer, 48, "Дата: %d.%m.%Y %A %H:%M:%S ", timeinfo);

		*stream << "----- Протокол ----- " << buffer << endl << endl;
	}

	// ------- Вывод в протокол информации о входных параметрах -------
	void WriteParm(std::ostream* stream, Param::Params parm)
	{
		string log;
		string in="";

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		log = converter.to_bytes(*parm.log.data);

		for (int i = 0; i < parm.in.size; i++) {
			in += converter.to_bytes(parm.in.data[i])+" ";
		}

		*stream << "----- Параметры -----" << endl <<
			"-log: " << log << endl <<
			//"-out: " << ptrOut << endl <<
			"-in: " << in << endl << endl;

	}

	//// ------- Вывод в протокол информации о файле -------
	//void WriteIn(std::ostream* stream, In::IN in)
	//{
	//	*stream << "----- Исходные данные -----" << endl <<
	//		"Количество символов\t: " << in.size << endl <<
	//		"Проигнорировано\t\t: " << in.ignor << endl <<
	//		"Количество строк\t: " << in.lines << endl << endl;

	//	//delete[] in.text;			//Нужно разобраться когда чистить память
	//}

	// ------- Вывод в протокол информации об ошибке -------
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
