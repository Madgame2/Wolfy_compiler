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
#include"Log.h"


using namespace std;

void write_bat_file(const std::vector<std::wstring>& files) {
	// Открываем файл для записи
	setlocale(LC_ALL, "rus");

	std::ofstream out_bat("execute.bat");

	if (!out_bat.is_open()) {
		throw Error::get_error(0);
	}

	// Записываем содержимое bat-файла
	out_bat << "@echo off\n\n";

	out_bat << "rem Настраиваем среду через vcvars32.bat\n";
	out_bat << "call \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars32.bat\"\n";
	out_bat << "if %errorlevel% neq 0 (\n";
	out_bat << "    echo Error setting up environment!\n";
	out_bat << "    pause\n";
	out_bat << "    exit /b %errorlevel%\n";
	out_bat << ")\n\n";

	out_bat << "set assembler=ml.exe\n";
	out_bat << "set linker=link.exe\n";
	out_bat << "set flags=/c /coff\n";
	out_bat << "set linker_flags=/subsystem:console\n\n";

	out_bat << "rem Текущая директория\n";
	out_bat << "set CURRENT_DIR=%CD%\n\n";

	out_bat << "rem Компиляция\n";
	out_bat << "echo compiling\n";
	out_bat << "%assembler% %flags% ";

	std::string object_files = "";
	for (const auto& file : files) {
		std::string file_str(file.begin(), file.end());
		out_bat << "\"" << file_str << ".asm\" ";
		object_files += "\"" + file_str + ".obj\" ";
	}
	out_bat << "\n";
	out_bat << "if %errorlevel% neq 0 (\n";
	out_bat << "    pause\n";
	out_bat << "    exit /b %errorlevel%\n";
	out_bat << ")\n\n";

	out_bat << "rem Линковка\n";
	out_bat << "echo linking object files\n";
	out_bat << "%linker% %linker_flags% " << object_files;
	out_bat << "\"WolfyConsoleLib.lib\" \"Wolfy_standart_lib.lib\" /out:main.exe\n";
	out_bat << "if %errorlevel% neq 0 (\n";
	out_bat << "    echo Linking error!\n";
	out_bat << "    pause\n";
	out_bat << "    exit /b %errorlevel%\n";
	out_bat << ")\n\n";

	out_bat << "echo compiling finished\n";
	out_bat << "pause\n";

	out_bat.close();
}



int wmain(int argc, wchar_t* argv[]) {


	LOG::LOG log_file;
	try
	{
		Param::Params params = Param::getParams(argc, argv);
		Param::checking_in_params(params);


		in::IN input_files = in::get_IN(params);
		log_file = LOG::init_LOG(params);

		key_words::Key_words_table key_words;
		key_words::Key_words_table::create_table(key_words);

		map<wstring, LT::Lexem_table> LT_files;				//файл -> таблица лексем
		map<wstring, ID::ID_table> ID_files;				//файл -> таблица индефикаторов
		map<wstring, Lit_table::Literal_table> Lit_files;	//файл -> таблица литералов

		LOG::WriteLog(log_file.stream);
		LOG::WriteParm(log_file.stream, params);


		LOG::WriteLine(log_file.stream, "\nBEGINIGN LEXER");
		lexer::parse(input_files, key_words,LT_files,ID_files,Lit_files,log_file.stream);

		//for (int i = 0; i < LT_files[L"GLOBAL"].size; i++) {
		//	cout << LT_files[L"GLOBAL"].table[i].lexema;
		//}


		LOG::WriteLine(log_file.stream, "\nID tables:");
		for (auto ID_table : ID_files) {
			LOG::WriteLine(log_file.stream, ID_table.first);

			std::string result = ID::to_string(ID_table.second);

			LOG::WriteLine(log_file.stream, result);
		}

		LOG::WriteLine(log_file.stream, "\nLIT tables:");
		for (auto Lit_table : Lit_files) {
			LOG::WriteLine(log_file.stream, Lit_table.first);

			std::string result = Lit_table::to_string(Lit_table.second);

			LOG::WriteLine(log_file.stream, result);
		}

		LOG::WriteLine(log_file.stream, "\nBEGINIGN PARSER");
		map<std::wstring, AST::program_struct> file_structs;
		for (auto& elem : LT_files) {
			LOG::WriteLine(log_file.stream, elem.first);
			if (elem.first == L"MAIN") {


				AST::program_struct tree = parser::Parse(elem.second, ID_files[L"MAIN"], MAIN, log_file.stream);
				file_structs[L"MAIN"] = tree;
			}
			else {
				AST::program_struct tree = parser::Parse(elem.second, ID_files[elem.first], GENERAl, log_file.stream);
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

		LOG::WriteLine(log_file.stream, "CODE GENERATION");
		std::vector<std::wstring> out_file;
		for (auto& elem : file_structs) {

			if (elem.first == L"GLOBAL") {
				continue;
			}
			LOG::WriteLine(log_file.stream, elem.first+L".asm");

			if (elem.second.root != nullptr) {
				CODE::generate_code(elem.first, elem.second, ID_files[elem.first], Lit_files[elem.first], global_functions, log_file.stream);
				out_file.push_back(elem.first);
			}
		}

		for (auto& tree : file_structs) {
			AST::delete_node(tree.second.root);
		}

		write_bat_file(out_file);

		Param::delete_all(params);

		std::cout << "compilation was successful";
	}
	catch (Error::ERROR& err)
	{
		if (err.data.line != -1 && err.data.position != -1) {
			cout << err.error_masage << " line: " << err.data.line << " position: " << err.data.position;
		}
		else {
			cout << err.error_masage;
		}
		LOG::WriteError(log_file.stream, err);
	}
	log_file.close_all();
}