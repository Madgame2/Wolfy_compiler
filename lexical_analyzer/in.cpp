#include "in.h"

#include<iostream>

using namespace in;

int get_file_size(std::ifstream& file) {
	if (!file.is_open()) return -1;

	file.seekg(0, std::ios::end);

	int file_size = file.tellg();

	file.seekg(0, std::ios::beg);

	return file_size;
}

IN in::get_IN(Param::Params param)
{
	IN new_in;
	Param::param in = param.in;

	//Проверяем есть ли такие файлы
	for (int i = 0; i < in.size; i++) {
		std::ifstream file(in.data[i]);

		if (!file.is_open()) throw Error::get_error_in(110, -1, i + 1);
		//new_in.FILES->size++;

		file.close();
	}

	new_in.FILES = new SOURCE_FILE[in.size];
	new_in.file_count = in.size;

	for (int i = 0; i < in.size; i++) {
		std::ifstream file(in.data[i]);

		int file_size = get_file_size(file);

		wchar_t* code = new wchar_t[file_size+1];
		unsigned int index = 0;
		unsigned int line = 0;
		unsigned int characte = 0;

		char buffer = NULL;
		char curent = NULL;
		while (file.get(curent))
		{
			switch (new_in.alphabet[curent])
			{
			case IN::T:
				//Лишние  пробеллы
				if (buffer == ' ' && curent == ' ') {
					continue;
				}
				//Обработка коментариев
				else if (buffer == '/' && curent == '/') {
					//code[index - 1] = ' ';
					index--;

					while (file.get(curent) && curent != '\n');

					//buffer = NULL;
					code[++index] = new_in.alphabet[curent];
					line++;
					characte = 0;
					index++;
				}
				else if (buffer == '/' && curent == '*') {
					code[index - 1] = ' ';
					index--;

					while (file.get(curent) && buffer != '*' && curent != '/') {
						buffer = curent;
					}

					//buffer = NULL;

					line++;
					characte = 0;
				}
				else {

					code[index] = curent;
					index++;
					characte++;

				}
				buffer = curent;

				break;

			case IN::I:

				index++;
				break;

			case IN::F:
				delete[] code;
				throw Error::get_error_in(111, line, characte);
				break;

			default:
				if (buffer == '\n' && curent == '\n') {
					continue;
				}
				else if (buffer == ' ' && curent == '\n') {
					index--;
				}

				code[index] = new_in.alphabet[curent];
				index++;

				if (curent == '\n') {
					line++;
					characte = 0;
				}

				buffer = curent;

				break;
			}
		}
		
		code[index] = '\0';

		std::wstring sorce_code(code);
		std::wcout << sorce_code;

		new_in.FILES[i].file_name = in.data[i];
		new_in.FILES[i].source_code = sorce_code;
		new_in.FILES[i].size = index+1;

		delete[] code; //ПОТОМ УДАЛИТЬ
		file.close();
	}


	return new_in;
}
