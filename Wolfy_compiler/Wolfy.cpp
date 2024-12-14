#include<iostream>
#include<map>
#include<fstream>
#include"Error.h"
#include"Params.h"
#include"in.h"
#include"Lexem_table.h"
#include"Rules.h"
#include"comon.h"
#include"Lexer.h"
#include"Parser.h"
#include"Semantic_analysis.h"
#include"Code_generator.h"


using namespace std;

void write_bat_file(const std::vector<std::wstring>& files) {
	// Открываем файл для записи

	setlocale(LC_ALL, "rus");

	std::ofstream out_bat("execute.bat");

	if (!out_bat.is_open()) {
		throw Error::get_error(0);
	}

	
	out_bat << "@echo off\n";
	out_bat << "set assembler=ml.exe\n";
	out_bat << "set linker=link.exe\n";
	out_bat << "set flags=/c /coff\n";
	out_bat << "set linker_flags=/subsystem:console\n\n";

	out_bat << "set VCToolsInstallDir=C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\Tools\\MSVC\\14.31.31103\\\n";
	out_bat << "set WindowsSdkDir=C:\\Program Files (x86)\\Windows Kits\\10\\\n";
	out_bat << "set UCRTVersion=10.0.19041.0\n\n";
	
	out_bat << "rem Установите пути для 32-битных библиотек\n";
	out_bat << "set LIB=%VCToolsInstallDir%lib\\x86\n";
	out_bat << "set SDK_LIB=%WindowsSdkDir%Lib\\%UCRTVersion%\\um\\x86\n";
	out_bat << "set UCRT_LIB=%WindowsSdkDir%Lib\\%UCRTVersion%\\ucrt\\x86\n\n";

	out_bat << "rem Печатаем значения переменных\n";
	out_bat << "echo VCToolsInstallDir=%VCToolsInstallDir%\n";
	out_bat << "echo WindowsSdkDir=%WindowsSdkDir%\n";
	out_bat << "echo UCRTVersion=%UCRTVersion%\n\n";

	out_bat << "rem Установите путь к текущей директории для библиотек\n";
	out_bat << "set CURRENT_DIR=%CD%\n\n";

	out_bat << "rem Компиляция\n";
	out_bat << "echo compiling\n";
	out_bat << "%assembler% %flags% ";

	std::string object_files = "";
	for (auto file : files) {
		std::string file_str(file.begin(), file.end());
		out_bat << file_str << ".asm ";

		//size_t dot_pos = file_str.find_last_of('.');
		//std::string base_name = (dot_pos == std::string::npos) ? file_str : file_str.substr(0, dot_pos);
		object_files += file_str + ".obj ";  // Добавляем .obj файл в список
	}
	out_bat << "\n";
	out_bat << "if %errorlevel% neq 0 (\n";
	out_bat << "pause\n";
	out_bat << "exit /b %errorlevel%\n";
	out_bat << ")\n\n";

	out_bat << "rem Линковка\n";
	out_bat << "echo linking object files\n";
	out_bat << "%linker% %linker_flags% /LIBPATH:%LIB% /LIBPATH:%SDK_LIB% /LIBPATH:%UCRT_LIB% /LIBPATH:%CURRENT_DIR% ";
	out_bat << object_files;
	out_bat << " WolfyConsoleLib.lib Wolfy_standart_lib.lib";
	out_bat << " /out:main.exe\n";
	out_bat << "if %errorlevel% neq 0 (\n";
	out_bat << "echo Linking error!\n";
	out_bat << "pause\n";
	out_bat << "exit /b %errorlevel%\n";
	out_bat << ")\n\n";

	out_bat << "echo compiling finished\n";
	out_bat << "pause";
	
	out_bat.close();
}



int wmain(int argc, wchar_t* argv[]) {

	try
	{
		Param::Params params = Param::getParams(argc, argv);
		Param::checking_in_params(params);


		in::IN input_files = in::get_IN(params);

		key_words::Key_words_table key_words;
		key_words::Key_words_table::create_table(key_words);

		map<wstring, LT::Lexem_table> LT_files;				//файл -> таблица лексем
		map<wstring, ID::ID_table> ID_files;				//файл -> таблица индефикаторов
		map<wstring, Lit_table::Literal_table> Lit_files;	//файл -> таблица литералов

		lexer::parse(input_files, key_words,LT_files,ID_files,Lit_files);

		map<std::wstring, AST::program_struct> file_structs;
		for (auto& elem : LT_files) {
			if (elem.first == L"MAIN") {


				AST::program_struct tree = parser::Parse(elem.second, ID_files[L"MAIN"], MAIN);
				file_structs[L"MAIN"] = tree;
			}
			else {
				AST::program_struct tree = parser::Parse(elem.second, ID_files[elem.first], GENERAl);
				file_structs[elem.first] = tree;
			}
		}

		std::list<semantic::data::global_elem> global_functions = semantic::Parse_Global(file_structs[L"GLOBAL"], ID_files[L"GLOBAL"]);

		for (auto& elem : file_structs) {
			if (elem.first == L"GLOBAL") {
				continue;
			}
			semantic::Parse(elem.second, global_functions, ID_files[elem.first], Lit_files[elem.first]);
		}

		for (auto& elem : global_functions) {
			if (!elem.is_relisated) {
				throw Error::get_error(308);
			}
		}

		std::vector<std::wstring> out_file;
		for (auto& elem : file_structs) {
			if (elem.first == L"GLOBAL") {
				continue;
			}

			if (elem.second.root != nullptr) {
				CODE::generate_code(elem.first, elem.second, ID_files[elem.first], Lit_files[elem.first], global_functions);
				out_file.push_back(elem.first);
			}
		}

		write_bat_file(out_file);

		Param::delete_all(params);

	}
	catch (Error::ERROR err)
	{
		if (err.data.line != -1 && err.data.position != -1) {
			cout << err.error_masage << " line: " << err.data.line << " position: " << err.data.position;
		}
		else {
			cout << err.error_masage;
		}
	}

}