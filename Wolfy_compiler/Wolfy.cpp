#include<iostream>
#include<map>
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

		for (auto& elem : file_structs) {
			if (elem.first == L"GLOBAL") {
				continue;
			}

			CODE::generate_code(elem.first, elem.second, ID_files[elem.first], Lit_files[elem.first], global_functions);
		}

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