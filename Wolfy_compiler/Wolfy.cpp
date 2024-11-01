#include<iostream>
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

		lexer::parse(input_files, key_words);
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