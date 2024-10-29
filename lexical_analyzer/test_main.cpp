#include<iostream>
#include<list>
#include<map>
#include<unordered_set>
#include<stack>

#include"Params.h"
#include"in.h"
#include"Lexem_table.h"
#include"Identification_table.h"
#include"Identification_table.h"

using namespace std;

list<wstring> divid_str(wstring source_code) {
	list<wstring> words;
	unsigned int word_beg = -1;


	std::unordered_set<wchar_t> specialChars = {
	L'+', L'-', L'=', L'<', L'>', L'|',
	L'&', L'/', L'*', L'%', L'!', L'^',
	L';', L',', L'(', L')', L'{', L'}',
	L'[', L']', L':', L'@', L'$', L'#',
	L'~', L'?', L'\\', L'.', L'\"', L'\''
	};

	unsigned int index = 0;
	while (source_code[index] != '\0') {
		char buffer = source_code[index];

		//проверяем символ или число
		if (isalpha(buffer) || isdigit(buffer)) {
			if (word_beg == -1) {
				word_beg = index;
			}
		}
		else {

			//нашли новое слово
			if (word_beg != -1) {
				unsigned int lenght = index - word_beg;
				words.push_back(source_code.substr(word_beg, lenght));

				word_beg = -1;
				continue;
			}


			// Проверяем, является ли символ специальным
			else if (specialChars.find(buffer)!= specialChars.end()) {
				words.push_back(source_code.substr(index, 1)); // Добавляем специальный символ
			}
		}


		index++;
	}

	if (word_beg != -1) {
		unsigned int lenght = index - word_beg;
		words.push_back(source_code.substr(word_beg, lenght));

		word_beg = -1;
	}


	return words;
}

bool is_only_digit(wstring str) {
	for (int i = 0; i < str.size(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}


void getContext(ID::Entry& entry,stack<wstring> context, key_words::Key_words_table table) {
	while (!context.empty())
	{
		if (int id = table.find(context.top()) != -1) {


			RULE::key::Elemet elem = table.get_element(id);
			if (elem.type != DataType::Type::None) {
				entry.d_type = elem.type;
			}
			else if (elem.extra != extra::Type::None) {
				entry.extras.push_back(elem.extra);
			}
		}

		context.pop();
	}
}


void parse(in::IN in_files, key_words::Key_words_table& key_words) {


	std::unordered_set<wchar_t> specialChars = {
	L'+', L'-', L'=', L'<', L'>', L'|',
	L'&', L'/', L'*', L'%', L'!', L'^',
	L';', L',', L'(', L')', L'{', L'}',
	L'[', L']', L':', L'@', L'$', L'#',
	L'~', L'?', L'\\', L'.', L'\"', L'\''
	};


	std::map<wstring, LT::Lexem_table> LT_files;
	std::map<wstring, ID::ID_table> ID_files;
	for (int i = 0; i < in_files.file_count; i++) {

		//LT_files[in_files.FILES->file_name] = LT::Lexem_table();				//Создаем новую таблицу лексем для нового файла 
		list<wstring> words = divid_str(in_files.FILES[i].source_code);			//Разбиваем файл на список слов и смволов 
		
		LT::Lexem_table new_table;
		LT::create_Lexem_table(new_table, words.size());
		LT_files[in_files.FILES->file_name] = new_table;

		ID::ID_table new_id_table;
		ID_files[in_files.FILES->file_name] = new_id_table;

		stack<wstring> context_stack;
		stack<wstring> function_context;

		bool is_word_leteral = false;
		wstring file_name = in_files.FILES->file_name;
		unsigned int line = 0;
		for (auto& word : words) {

			if (is_word_leteral) {

				

				if (word.size() == 1 && (word[0] == L'"' || word[0] == L'\'')) {
					//Добавляем ЛИТЕРАЛ строки

					LT::Entry new_entry;
					memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)LEX_LET, sizeof(new_entry.lexema)-1);
					new_entry.lexema[sizeof(new_entry.lexema)-1] = '\0';
					new_entry.source_code_line = line;

					LT::add(LT_files[file_name], new_entry);

					is_word_leteral = false;

					cout << LEX_LET;
				}
			}
			else {
				if (word[0] == L'\"') 
				{ 
					is_word_leteral = true; 
				}
				else
				{
					//Проверка на ключевое слово
					int id;
					if ((id = key_words.find(word))>=0) {
						//Вставить лексему ключегого слова 

						LT::Entry new_entry;

						memcpy_s( new_entry.lexema, sizeof(new_entry.lexema),  key_words.get_element(id).lexem, sizeof(new_entry.lexema)-1);
						new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';

						LT::add(LT_files[file_name], new_entry);

						cout << new_entry.lexema;
					}
					else if(is_only_digit(word)) {
						//Вставки лексемы литерала 
						
						LT::Entry new_entry;

						memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)LEX_LET, sizeof(new_entry.lexema) - 1);
						new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';

						LT::add(LT_files[file_name], new_entry);

						cout << new_entry.lexema;

					}
					else if (word.size() == 1 && (specialChars.find(word[0]) != specialChars.end())) {
						wchar_t buffer = word[0];

						switch (buffer)
						{
						case L'|':
							line++;
							cout << '\n';   // ДЛЯ ОТЛАДКИ 
							continue;

							break;
						case ';':
							{
							LT::Entry new_entry;

							new_entry.lexema[0] = word[0];
							new_entry.lexema[1] = '\0';

							LT::add(LT_files[file_name], new_entry);

							while (!context_stack.empty())
							{
								context_stack.pop();
							}

							cout << new_entry.lexema;
							}
							break;
						default:

							LT::Entry new_entry;

							new_entry.lexema[0] = word[0];
							new_entry.lexema[1] = '\0';

							LT::add(LT_files[file_name], new_entry);

							cout << new_entry.lexema;

							break;
						}
					}
					else {
						//вставляем индефикатор

						LT::Entry new_entry;

						memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)LEX_ID, sizeof(new_entry.lexema) - 1);
						new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';


						//Таблица индефикаторов 
						int id;
						if ((id = ID::isId(ID_files[file_name], word, function_context.empty() ? L"" : function_context.top())) == -1) {

							ID::Entry new_ID;
							new_ID.name = word;
							new_ID.area = function_context.empty() ? L"" : function_context.top();

							ID::add(ID_files[file_name], new_ID);
						}
						else {
							new_entry.IT_index=id;
						}


						LT::add(LT_files[file_name], new_entry);

						cout << new_entry.lexema;
					}
				}
			}

		}

	}
}



int wmain(int argc, wchar_t* argv[]) {

	Param::Params param = Param::getParams(argc, argv);
	Param::checking_in_params(param);

	for (int i = 0; i < param.in.size; i++) {
		wcout << param.in.data[i] << endl;
	}

	in::IN input_files = in::get_IN(param);



	key_words::Key_words_table key_words;
	key_words::Key_words_table::create_table(key_words);

	cout << '\n';


	parse(input_files, key_words);


	Param::delete_all(param);
}