#include"Lexem_table.h"

using namespace LT;
void LT::create_Lexem_table(Lexem_table& table,unsigned int size)
{
	if (table.max_size != 0) {
		std::cout << "lexem table canot be reinitialized";
		return;
	}

	table.max_size = size;
	//table.key_words = key_table;
	table.table = new Entry[size];
}

void LT::Delete(Lexem_table& table)
{
	delete[] table.table;
}

void LT::add(Lexem_table& table, Entry ent)
{
	if (table.size >= table.max_size) throw Error::get_error(2);

	table.table[table.size] = ent;
	table.size++;
}

LT::Entry LT::getEntry(Lexem_table& table, int id)
{
	if (id < 0 || id >= table.size) throw Error::get_error(3);


	return table.table[id];
}

int key_words::Key_words_table::hash(std::wstring str)
{
	unsigned int hash_code = 0;
	for (int i = 0; i<str.size(); i++) {
		hash_code += (unsigned int)str[i];
	}

	hash_code = hash_code % this->size;

	//if (!this->table[hash_code].empty()) {
	//	return hash(str, 1);
	//}

	return hash_code;
}

int key_words::Key_words_table::hash(std::wstring str, int ofset)
{
	unsigned int code = 0;
	for (int i = 0; i < str.size(); i++) {
		code += (unsigned int)str[i];
	}

	int hash_code = ((code % this->size) + ofset) % this->size;

	//if (!this->table[hash_code].empty()) {
		//for (int i = ofset, int step = 0; step < this->size; step++, i++) {
			hash_code = ((code % this->size) + ofset) % this->size;

			if (this->table[hash_code].str.empty()) {
				return hash_code;
			}
		//}
	//}

	return hash_code;
}

void key_words::Key_words_table::add(std::wstring new_key_word,char lexema[LEXEMA_SIZE+1])
{
	int  hash_code = hash(new_key_word);

	if (!table[hash_code].str.empty()) {
		for (int i = 1; i < size; i++) {
			hash_code = hash(new_key_word, i);

			if (table[hash_code].str.empty()) {
				break;
			}
		}
	}

	RULE::key::Elemet new_element;
	memcpy_s(new_element.lexem,sizeof(new_element.lexem), lexema, LEXEMA_SIZE);
	new_element.lexem[LEXEMA_SIZE] = '\0';
	new_element.str = new_key_word;

	this->table[hash_code] = new_element;
}

int key_words::Key_words_table::find(std::wstring key_word)
{
	int hash_code = hash(key_word);

	if (table[hash_code].str.empty()) {
		return -1;
	}

	if (table[hash_code].str == key_word) {
		return hash_code;
	}
	
	for (int i = 1; i < size; i++) {

		hash_code = hash(key_word, i);
		if (table[hash_code].str == key_word) {
			return hash_code;
		}
		else if (table[hash_code].str.empty()) {
			break;
		}
	}

	return -1;
}

RULE::key::Elemet key_words::Key_words_table::get_element(int id)
{
	return this->table[id];
}

void key_words::Key_words_table::Delete()
{
	delete[] table;
}

void key_words::Key_words_table::create_table(Key_words_table& table, int size, RULE::key::Elemet first, ...)
{
	table.size = size;
	table.table = new RULE::key::Elemet[size];
	table.table[0] = first;

	va_list args;
	va_start(args, first);

	for (int i = 1; i < size; i++) {

		table.table[i] = va_arg(args, RULE::key::Elemet);
	}

	va_end(args);


}

void key_words::Key_words_table::create_table(Key_words_table& table)
{
	//table.size = RULE::key::key_words.size();
	//table.table = new RULE::key::Elemet[table.size];

	//for (auto elem : RULE::key::key_words){
	//}

}
