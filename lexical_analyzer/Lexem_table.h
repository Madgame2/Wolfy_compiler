#pragma once
#include<iostream>
#include<string>

#include"Error.h"
#define LEXEMA_SIZE 1

namespace LT {
	struct Lexem_table {

		unsigned int size = 0;
		unsigned int max_size = 0;
		Entry* table = nullptr;
	};

	struct Entry {
		char lexema[LEXEMA_SIZE];
		unsigned int source_code_line = 0;
		unsigned int IT_index = -1;
	};


	void create_Lexem_table(Lexem_table& table, unsigned int size);
	void Delete(Lexem_table& table);

	void add(Lexem_table& table, Entry ent);
	Entry getEntry(Lexem_table& table, int id);
}

namespace key_words {
	struct Key_words_table {
	private:
		std::wstring* table = nullptr;
		unsigned int size = 0;

		int hash(std::wstring str);
		int hash(std::wstring str, int ofset);
		void add(std::wstring new_key_word);
	public:
		bool find(std::wstring key_word);
	};
}