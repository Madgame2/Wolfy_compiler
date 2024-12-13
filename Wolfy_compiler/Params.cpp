#include"Params.h"

using namespace Param;

void to_lowercase(wchar_t* str) {
	for (size_t i = 0; str[i] != L'\0'; ++i) {
		str[i] = std::towlower(str[i]);
	}
}
void to_lowercase(std::wstring& str) {
	std::transform(str.begin(), str.end(), str.begin(), std::towlower);
}

void Read_new_key(wchar_t* argv, int& mode) {
	mode = -1;//Сбрасываем режим
	wchar_t* end_of_key = wcschr(argv, L':');


	if (end_of_key == nullptr) {
		Error::get_error(103);
	}


	size_t lenght = end_of_key - argv; //длина ключа 

	wchar_t* key = new wchar_t[lenght];
	wmemcpy(key, argv, lenght);


	//Пробегаемся по списку ключей 
	KEYS keys;
	for (int j = 0; j < keys.lenght; j++) {
		if (keys.keys[j].name.compare(key)) {
			mode = keys.keys[j].id;
			break;
		}
	}

	delete[] key;
	if (mode == -1) {
		Error::get_error(105);
	}
}

Params Param::getParams(int argc, wchar_t* argv[])
{

	if (argc <= 1) throw Error::get_error(100);

	short count_in_param = 0;
	for (int i = 0; i < argc; i++) {
		if (wcsstr(argv[i], L"-in:")) {
			count_in_param++;
		}
	}

	if (count_in_param > 1) throw Error::get_error(101);
	if (count_in_param < 1) throw Error::get_error(100);

	Params params;
	int mode = -1;
	std::vector<wchar_t*> in;
	for (int i = 0; i < argc; i++) {

		//Найден новый ключ
		if (argv[i][0] == '-') {
			Read_new_key(argv[i], mode);
		}

		switch (mode)
		{
		case 0:

			//Проверяем а не находятся ли данные в том же слове что и кллюч(в будушем можно разделить на функцию)

			//ОШИБКА ТУТ ИСПРАВИТЬ КОГДА ЕСТЬ ПРОБЕЛ ОН НЕ НАХОДИТ ФАЙЛ

			if (argv[i][0] == '-' && argv[i][wcslen(argv[i] - 1)] != L':') {
				wchar_t* end_of_key = wcschr(argv[i], L':');
				wchar_t* begin = argv[i];

				size_t param_size = wcslen(argv[i]);
				if (end_of_key != nullptr && param_size != end_of_key - begin + 1) {
					wchar_t* argv = end_of_key + 1; //new wchar_t[size+1];

					in.push_back(argv);

				}
				else {
					//throw Error::get_error(103);
				}
				continue;
			}

			in.push_back(argv[i]);

			break;
		}

	}

	int count_main = 0;
	for (int i = 0; i < in.size(); i++) {
		std::wstring temp = std::wstring(in[i]);
		to_lowercase(temp);
		if (temp == L"main.wolf") {
			count_main++;
		}

	}

	if (count_main < 1) throw Error::get_error(104);
	else if (count_main > 1) throw Error::get_error(103);

	std::wstring* in_str = new std::wstring[in.size()];
	for (size_t i = 0; i < in.size(); i++) {
		if (in[i] != nullptr) {
			in_str[i] = std::wstring(in[i]);
		}
	}

	params.in.data = in_str;
	params.in.size = in.size();

	return params;
}


bool Param::checking_in_params(Params& params)
{

	param& in = params.in;

	if (in.size == 0) throw Error::get_error(102);

	for (int i = 0; i < in.size; i++) {
		std::wstring file_name = in.data[i];
		// Проверка, что имя файла длиннее, чем расширение
		if (file_name.length() < std::wstring(IN_FILE_TYPE).length() ||
			file_name.compare(file_name.length() - std::wstring(IN_FILE_TYPE).length(), std::wstring(IN_FILE_TYPE).length(), IN_FILE_TYPE) != 0)
		{
			// Добавляем расширение, если его нет в конце строки
			file_name.append(IN_FILE_TYPE);
			in.data[i] = file_name;
		}
	}


	return true;
}

void Param::delete_all(Params& param)
{
	delete[] param.in.data;
}



/* НА РАЗМЫШЛЕНИЕ
* 
* 1) попробовать разбить get_params на более маленькие функции
* 2) обработаку ключа производить не при обнаружении нового ключа а в switch
*    т.к возможно в будушем будут добавлятся новые ключи 
*/
