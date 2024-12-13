#pragma once
#include<string.h>
#include <cstring> // Äëÿ strcmp
#include"Graibach.h"
#include<map>
#include"comon.h"
#include"Rules.h"
#include"Lexem_table.h"
#include"MFST.h"
#include"AST.h"
#include"Polish_entry.h"
#include"Identification_table.h"

namespace parser {
	AST::program_struct Parse(LT::Lexem_table table,ID::ID_table id_table, std::list<Rule> rules);
}