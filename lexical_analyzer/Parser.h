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

namespace parser {
	AST::program_struct Parse(LT::Lexem_table table, std::list<Rule>);
}