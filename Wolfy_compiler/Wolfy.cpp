#include<iostream>
#include<map>
#include"Error.h"
#include"Params.h"
#include"in.h"
#include"Lexem_table.h"
#include"Rules.h"
#include"comon.h"
#include"Lexer.h"

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