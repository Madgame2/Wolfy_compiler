#pragma once
#include<iostream>
#include<string>
#include<cstdarg>
#include"Rules.h"
#include"Error.h"




namespace key_words {

	struct Key_words_table {
	private:
		RULE::key::Elemet* table = nullptr;
		unsigned int size = 0;

		int hash(std::wstring str);
		int hash(std::wstring str, int ofset);
		void add(RULE::key::Elemet elem);
	public:
		int find(std::wstring key_word);
		RULE::key::Elemet get_element(int id);
		void Delete();

		static void create_table(Key_words_table& table, int size, RULE::key::Elemet first, ...);
		static void create_table(Key_words_table& table);
	};
}

namespace LT {

	struct Entry {
		char lexema[LEXEMA_SIZE+1];
		unsigned int source_code_line = 0;
		int IT_index = -1;
		int Lit_index = -1;

		bool is_double_oeration = false;
		bool console_operations = false;

		Entry() {
			lexema[0] = '\0';
		}
	};

	struct Lexem_table {

		unsigned int size = 0;
		unsigned int max_size = 0;
		Entry* table = nullptr;
		//key_words::Key_words_table key_words;

		//Lexem_table(){}
	};


	void create_Lexem_table(Lexem_table& table, unsigned int size);
	void Delete(Lexem_table& table);

	void add(Lexem_table& table, Entry ent);
	Entry& getEntry(Lexem_table& table, int id);
}

