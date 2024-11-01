#pragma once
#include<list>
#include<string>
#include"comon.h"


namespace RULE {
	namespace key {

		struct Elemet {
			std::wstring str;
			char lexem[LEXEMA_SIZE + 1];
			DataType::Type type = DataType::Type::None;
			extra::Type extra = extra::Type::None;

			Elemet(const std::wstring& str, const char* lexem) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

			}
			Elemet(const std::wstring& str, const char* lexem, DataType::Type type) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

				this->type = type;
			}
			Elemet(const std::wstring& str, const char* lexem, extra::Type extra) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

				this->extra = extra;
			}

			Elemet() {
				lexem[0] = '\0';
			}
		};

		extern std::list <Elemet> key_words;
	}
}