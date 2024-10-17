#include"Lexem_table.h"

using namespace LT;
void LT::create_Lexem_table(Lexem_table& table, unsigned int size)
{
	if (table.max_size != 0) {
		std::cout << "lexem table canot be reinitialized";
		return;
	}

	table.max_size = size;
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

Entry LT::getEntry(Lexem_table& table, int id)
{
	if (id < 0 || id >= table.size) throw Error::get_error(3);


	return table.table[id];
}

int key_words::Key_words_table::hash(std::wstring str)
{
	unsigned int hash_code = 0;
	for (int i = 0; str.size(); i++) {
		hash_code += (unsigned int)str[i];
	}

	hash_code = hash_code % this->size;

	if (!this->table[hash_code].empty()) {
		return hash(str, 1);
	}

	return hash_code;
}

int key_words::Key_words_table::hash(std::wstring str, int ofset)
{
	unsigned int code = 0;
	for (int i = 0; i < str.size(); i++) {
		code += (unsigned int)str[i];
	}

	int hash_code = ((code % this->size) + ofset) % this->size;

	if (!this->table[hash_code].empty()) {
		for (int i = ofset, int step = 0; step < this->size; step++, i++) {
			hash_code = ((code % this->size) + i) % this->size;

			if (this->table[hash_code].empty()) {
				return hash_code;
			}
		}
	}

	return hash_code;
}
