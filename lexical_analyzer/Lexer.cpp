#include"Lexer.h"

#include"Parser.h"
#include"Semantic_analysis.h"
#include"Code_generator.h"

#define LIT_KEY 1
#define ID_KEY 2

#define DEBUG

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
	while (source_code[index] != L'\0') {
		wchar_t current_char = source_code[index];

		// ��������� ������ ��� �����
		if (isalpha(current_char) || isdigit(current_char) || current_char == L'_') {
			if (word_beg == -1) {
				word_beg = index;
			}
		}
		else {

			// ����� ����� �����
			if (word_beg != -1) {
				unsigned int length = index - word_beg;
				words.push_back(source_code.substr(word_beg, length));

				word_beg = -1;
				continue;
			}

			// ���������, �������� �� ������ �����������
			else if (specialChars.find(current_char) != specialChars.end()) {

				// ���� ������� ������ '=' � ��������� ���� '=', �� ��� "=="
				if (current_char == L'=' && source_code[index + 1] == L'=') {
					words.push_back(L"==");
					index++; // ���������� ��������� ������, ��� ��� �� ��� ���������
				}
				// ���������� ��� <=, >=
				else if (current_char == L'<' && source_code[index + 1] == L'=') {
					words.push_back(L"<=");
					index++; // ���������� ��������� ������
				}
				else if (current_char == L'>' && source_code[index + 1] == L'=') {
					words.push_back(L">=");
					index++; // ���������� ��������� ������
				}


				else if (current_char == L'<'&&source_code[index+1]==L'<') {
					words.push_back(L"<<");
					index++;
				}
				else if (current_char == L'>' && source_code[index + 1] == L'>') {
					words.push_back(L">>");
					index++;
				}
				else {
					words.push_back(source_code.substr(index, 1)); // ��������� ����������� ������
				}
			}
		}

		index++;
	}

	// ��������� ��������� �����, ���� ��� ����
	if (word_beg != -1) {
		unsigned int length = index - word_beg;
		words.push_back(source_code.substr(word_beg, length));
	}

	//for (auto elem : words) {
	//	wcout << elem;
	//	cout << '\n';
	//}

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
		int id = table.find(context.top());
		if ( id!= -1) {


			RULE::key::Elemet elem = table.get_element(id);
			if (elem.type != DataType::Type::None) {
				entry.d_type = elem.type;
			}
			else if (elem.extra != extra::Type::None) {
				entry.extras.push_back(elem.extra);
			}
			else if (strcmp(elem.lexem,"f")==0) {
				entry.id_type = IDType::Type::Func;
			}
		}

		context.pop();
	}
}
void  get_litContext(Lit_table::Element& lit, stack<wstring> context, key_words::Key_words_table table) {
	if (lit.d_type == DataType::Type::String) return;
	
	while (!context.empty())
	{
		int id = table.find(context.top());
		if (id != -1) {


			RULE::key::Elemet elem = table.get_element(id);
			if (elem.notation != notations::notation::None) {
				lit.my_notation = elem.notation;
				return;
			}

		}

		context.pop();
	}
	lit.my_notation == notations::notation::Dec;
}
void check_redefinition(ID::Entry& entry, stack<wstring> context, key_words::Key_words_table table, int line, int pos) {
	while (!context.empty())
	{
		int id = table.find(context.top());
		if (id != -1) {


			RULE::key::Elemet elem = table.get_element(id);
			if (elem.type != DataType::Type::None) {
				throw Error::get_error_in(115, line, pos);
			}
			else if (elem.extra != extra::Type::None) {
				throw Error::get_error_in(115, line, pos);
			}
			else if (strcmp(elem.lexem, "f") == 0) {
				throw Error::get_error_in(115, line, pos);
			}
		}

		pos--;
		context.pop();
	}
}
void  clear_context(stack<wstring>& context) {
	while (!context.empty())
	{
		context.pop();
	}
}
void create_LexT_element(LT::Lexem_table& table, const char* lexem, int line) {
	LT::Entry new_entry;
	memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)lexem, sizeof(new_entry.lexema) - 1);
	new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';
	new_entry.source_code_line = line;

	LT::add(table, new_entry);
	//table.size++;

#ifdef DEBUG
	cout << new_entry.lexema;
#endif // DEBUG

}
void create_LexT_element(LT::Lexem_table& table, const char* lexem, int line, int id, int key) {
	LT::Entry new_entry;
	memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)lexem, sizeof(new_entry.lexema) - 1);
	new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';
	new_entry.source_code_line = line;

	if (key == LIT_KEY) {
		new_entry.Lit_index = id;
	}
	else if (key == ID_KEY) {
		new_entry.IT_index = id;
	}


	LT::add(table, new_entry);
	//table.size++;

#ifdef DEBUG
	cout << new_entry.lexema;
#endif // DEBUG

}
void create_LexT_element(LT::Lexem_table& table, char lexem, int line) {
	LT::Entry new_entry;

	new_entry.lexema[0] = lexem;
	new_entry.lexema[1] = '\0';

	LT::add(table, new_entry);
	//table.size++;


#ifdef DEBUG
	cout << new_entry.lexema;
#endif // DEBUG

}
void create_LitT_lement(Lit_table::Literal_table& table, wstring value, DataType::Type type) {
	Lit_table::Element elem;
	elem.d_type = type;
	elem.value = value;

	table.table.push_back(elem);
	table.size++;
}
void  lexer::parse(in::IN in_files, key_words::Key_words_table& key_words,
	std::map<wstring, LT::Lexem_table>& LT_files,
	std::map<wstring, ID::ID_table>& ID_files,
	std::map<wstring, Lit_table::Literal_table>& Lit_files) {


	std::unordered_set<wchar_t> specialChars = {
	L'+', L'-', L'=', L'<', L'>', L'|',
	L'&', L'/', L'*', L'%', L'!', L'^',
	L';', L',', L'(', L')', L'{', L'}',
	L'[', L']', L':', L'@', L'$', L'#',
	L'~', L'?', L'\\', L'.', L'\"', L'\''
	};

	for (int i = 0; i < in_files.file_count; i++) {


		list<wstring> words = divid_str(in_files.FILES[i].source_code);			//��������� ���� �� ������ ���� � ������� 

		//for (auto&  elem: words) {
		//	wcout << elem << endl;
		//}

		wstring file_name;
		if (in_files.FILES[i].is_main) {
			file_name = L"MAIN";
		}
		else {
			file_name = in_files.FILES[i].file_name;
		}

		LT::Lexem_table new_table;
		LT::create_Lexem_table(new_table, words.size());
		LT_files[file_name] = new_table;


		ID::ID_table new_id_table;
		ID_files[file_name] = new_id_table;

		Lit_table::Literal_table new_lit_table;
		Lit_files[file_name] = new_lit_table;


		stack<wstring> context_stack;						//���� ��������� ����; (���������� ��� ������ ; )
		stack<wstring> function_context;					//���� ��������� �������

		stack<wchar_t> hocks;								//���� ��� �������� ������

		bool is_word_leteral = false;
		bool new_ID_created = false;
		bool is_joint = false;
		bool is_params = false;
		bool is_notation = false;
		int last_ID_id = -1;
		int last_lit_id = -1;
		wstring lit_buffer;
		unsigned int line = 0;
		unsigned int word_index = -1;
		for (auto& word : words) {
			word_index++;

			if (is_word_leteral) {



				if (word.size() == 1 && (word[0] == L'"' || word[0] == L'\'')) {
					//��������� ������� ������
					last_lit_id++;

					create_LitT_lement(Lit_files[file_name], lit_buffer, DataType::Type::String);
					create_LexT_element(LT_files[file_name], LEX_LET, line, last_lit_id, LIT_KEY);

					lit_buffer.clear();
					is_word_leteral = false;
				}
				else {
					if (lit_buffer.empty()) {
						lit_buffer += word;
					}
					else {
						lit_buffer += L' ' + word;
					}
				}
			}
			else {
				if (word[0] == L'\"')
				{
					is_word_leteral = true;
				}
				else
				{
					//�������� �� �������� �����
					int id;
					if ((id = key_words.find(word)) >= 0) {
						//�������� ������� ��������� ����� 

						if (key_words.get_element(id).notation != notations::notation::None) is_notation = true;

						context_stack.push(word);

						create_LexT_element(LT_files[file_name], key_words.get_element(id).lexem, line);

					}
					else if (word == L"==") {
						// ������������ �������� "=="
						// ��������� ���� ��� ==

						LT::Entry new_entry;


						new_entry.lexema[0] = '=';
						new_entry.lexema[1] = '\0';
						new_entry.is_double_oeration = true;
						new_entry.source_code_line = line;

#ifdef DEBUG
						cout << new_entry.lexema;
#endif // DEBUG

						LT::add(LT_files[file_name], new_entry);

					}
					else if (word == L"<=") {
						// ������������ �������� "<="
						// ��������� ���� ��� <=

						LT::Entry new_entry;


						new_entry.lexema[0] = '<';
						new_entry.lexema[1] = '\0';
						new_entry.is_double_oeration = true;
						new_entry.source_code_line = line;

#ifdef DEBUG
						cout << new_entry.lexema;
#endif // DEBUG

						LT::add(LT_files[file_name], new_entry);
					}
					else if (word == L">=") {
						// ������������ �������� ">="
						// ��������� ���� ��� >=


						LT::Entry new_entry;


						new_entry.lexema[0] = '>';
						new_entry.lexema[1] = '\0';
						new_entry.is_double_oeration = true;
						new_entry.source_code_line = line;

#ifdef DEBUG
						cout << new_entry.lexema;
#endif // DEBUG

						LT::add(LT_files[file_name], new_entry);
					}
					else if (word == L">>") {
						// ������������ �������� ">="
						// ��������� ���� ��� >=


						LT::Entry new_entry;


						new_entry.lexema[0] = '>';
						new_entry.lexema[1] = '\0';
						new_entry.console_operations = true;
						new_entry.source_code_line = line;

#ifdef DEBUG
						cout << new_entry.lexema;
#endif // DEBUG

						LT::add(LT_files[file_name], new_entry);
					}
					else if (word == L"<<") {
						// ������������ �������� ">="
						// ��������� ���� ��� >=


						LT::Entry new_entry;


						new_entry.lexema[0] = '<';
						new_entry.lexema[1] = '\0';
						new_entry.console_operations = true;
						new_entry.source_code_line = line;

#ifdef DEBUG
						cout << new_entry.lexema;
#endif // DEBUG

						LT::add(LT_files[file_name], new_entry);
					}
					else if (word.size() == 1 && (specialChars.find(word[0]) != specialChars.end())) {

						wchar_t buffer = word[0];


						//�������� ������ 
						if (buffer == L'(' || buffer == L'[' || buffer == L'{') {
							hocks.push(buffer);
						}
						else if (buffer == L')' || buffer == L']' || buffer == L'}') {

							switch (buffer)
							{
							case L')':
								if (!hocks.empty() && hocks.top() == L'(') {
									is_params = false;
									hocks.pop();
								}
								else
								{
									throw Error::get_error_in(114, line, word_index);
								}
								break;
							case L']':
								if (!hocks.empty() && hocks.top() == L'[') {
									hocks.pop();
								}
								else
								{
									throw Error::get_error_in(114, line, word_index);
								}
								break;
							case L'}':
								if (!hocks.empty() && hocks.top() == L'{') {
									hocks.pop();
								}
								else
								{
									throw Error::get_error_in(114, line, word_index);
								}
								break;
							}
						}


						//����������� �������
						switch (buffer)
						{
						case L'|':
							line++;
							cout << '\n';   // ��� ������� 
							continue;

							break;
						case L';':
							clear_context(context_stack);
							break;
						case L'.':
						{
							if (is_only_digit(context_stack.top())) {
								LT::Entry latst_entry = LT::getEntry(LT_files[file_name], LT_files[file_name].size - 1);
								int  last_lit_id = latst_entry.Lit_index;
								if (last_lit_id != -1) {
									Lit_table::Element lat_lit = Lit_table::find(Lit_files[file_name], last_lit_id);
									if (lat_lit.d_type != DataType::Type::Float) {

										is_joint = true;
										continue;
									}
									else {
										throw Error::get_error_in(8, line, word_index);
									}
								}
								break;

							}
						}
						case'(': {
							LT::Entry last_entry = LT::getEntry(LT_files[file_name], LT_files[file_name].size - 1);
							int ID_index = last_entry.IT_index;


							if (ID_index != -1 && ID::getEntry(ID_files[file_name], ID_index).id_type == IDType::Type::Func)
								is_params = true;

							if (new_ID_created) {
								ID::getEntry(ID_files[file_name], last_ID_id).id_type = IDType::Type::Func;
							}
							if(!is_notation)
							clear_context(context_stack);
						}
							   break;
						case')': {
							clear_context(context_stack);
						
						}
						case '[':
						{

							if (new_ID_created && (ID::getEntry(ID_files[file_name], last_ID_id).id_type != IDType::Type::Func))
							{
								ID::getEntry(ID_files[file_name], last_ID_id).is_array = true;
							}
						}

						break;

						case '{':
							clear_context(context_stack);
							break;
						case',':
							clear_context(context_stack);
							break;
						}
						create_LexT_element(LT_files[file_name], word[0], line);

						context_stack.push(word);
					}
					else if (is_only_digit(word) || is_notation) {
						//������� ������� �������� 

						if (!is_joint) {
							last_lit_id++;

							create_LitT_lement(Lit_files[file_name], word, DataType::Type::Int);
							create_LexT_element(LT_files[file_name], LEX_LET, line, last_lit_id, LIT_KEY);

							get_litContext(Lit_files[file_name].table.back(), context_stack, key_words);
						}
						else
						{
							Lit_table::find(Lit_files[file_name], last_lit_id).value += context_stack.top() + word;
							Lit_table::find(Lit_files[file_name], last_lit_id).d_type = DataType::Type::Float;

							is_joint = false;
						}
						context_stack.push(word);
						is_notation = false;
					}
					else {
						//��������� �����������

						LT::Entry new_entry;

						memcpy_s(new_entry.lexema, sizeof(new_entry.lexema), (const char*)LEX_ID, sizeof(new_entry.lexema) - 1);
						new_entry.lexema[sizeof(new_entry.lexema) - 1] = '\0';


						//������� ������������� 

						ID::Entry new_ID;
						last_ID_id++;
						new_ID.name = word;
						new_ID.area = function_context.empty() ? L"" : function_context.top();
						if (!is_params) {
							new_ID.id_type = IDType::Type::Var;
						}
						else {
							new_ID.id_type = IDType::Type::Param;
						}

						getContext(new_ID, context_stack, key_words);

						ID::add(ID_files[file_name], new_ID);
						new_ID_created = true;

						new_entry.IT_index = last_ID_id;


						LT::add(LT_files[file_name], new_entry);

						cout << new_entry.lexema;
						continue;
					}

					new_ID_created = false;

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

	cout << '\n';

	key_words::Key_words_table key_words;
	key_words::Key_words_table::create_table(key_words);

	map<wstring, LT::Lexem_table> LT_files;				//���� -> ������� ������
	map<wstring, ID::ID_table> ID_files;				//���� -> ������� �������������
	map<wstring, Lit_table::Literal_table> Lit_files;	//���� -> ������� ���������

	lexer::parse(input_files, key_words, LT_files, ID_files, Lit_files);
	cout << "\n";
	//parser::Parse(LT_files[L"file1.wolf"]);


	for (auto& elem : LT_files) {
		if (elem.first == L"MAIN") {

			AST::program_struct tree = parser::Parse(elem.second, ID_files[L"MAIN"], MAIN);
			semantic::Parse(tree, ID_files[L"MAIN"], Lit_files[L"MAIN"]);
			CODE::generate_code(elem.first,tree, ID_files[L"MAIN"], Lit_files[L"MAIN"]);
		}
		else {
			//parser::Parse(elem.second, GENERAl);
		}
	}	

	Param::delete_all(param);
}
