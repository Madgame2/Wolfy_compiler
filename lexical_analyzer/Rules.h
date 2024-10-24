#pragma once
#include<list>
//#include"Lexem_table.h"
#include<string>

#define LEXEMA_SIZE 1

namespace RULE {
	namespace key {

		struct Elemet {
			std::wstring str;
			char lexem[LEXEMA_SIZE + 1];  // Массив лексем с местом для завершающего нуля

			// Конструктор с инициализацией
			Elemet(const std::wstring& str, const char* lexem) {
				this->str = str;

				// Безопасное копирование строки в массив lexem
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);

				// Добавляем завершающий ноль вручную
				this->lexem[LEXEMA_SIZE] = '\0';
			}

			// Конструктор по умолчанию
			Elemet() {
				lexem[0] = '\0';
			}
		};

		extern std::list <Elemet> key_words;
	}
}