#pragma once
#include<iostream>
#include<list>
#include<map>
#include<unordered_set>
#include<stack>

#include"Params.h"
#include"in.h"
#include"Lexem_table.h"
#include"Identification_table.h"
#include"Identification_table.h"
#include"Litetral_table.h"

#define LIT_KEY 1
#define ID_KEY 2

using namespace std;

namespace lexer {
	void parse(in::IN in_files, key_words::Key_words_table& key_words);
}