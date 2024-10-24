#pragma once
#include<list>
//#include"Lexem_table.h"
#include<string>

#define LEXEMA_SIZE 1

namespace RULE {
	namespace key {

		struct Elemet {
			std::wstring str;
			char lexem[LEXEMA_SIZE + 1];  // ������ ������ � ������ ��� ������������ ����

			// ����������� � ��������������
			Elemet(const std::wstring& str, const char* lexem) {
				this->str = str;

				// ���������� ����������� ������ � ������ lexem
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);

				// ��������� ����������� ���� �������
				this->lexem[LEXEMA_SIZE] = '\0';
			}

			// ����������� �� ���������
			Elemet() {
				lexem[0] = '\0';
			}
		};

		extern std::list <Elemet> key_words;
	}
}