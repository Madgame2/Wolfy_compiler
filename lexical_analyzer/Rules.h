#pragma once
#include<list>
//#include"Lexem_table.h"
#include<string>

#include"Lexem.h"

//#define LEXEMA_SIZE 1

namespace RULE {
	namespace key {

		struct Elemet {
			std::wstring str;
			char lexem[LEXEMA_SIZE + 1];

			// Конструктор с инициализацией
			Elemet(const std::wstring& str, const char* lexem) {
				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
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