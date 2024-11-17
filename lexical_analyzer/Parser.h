#pragma once
#include<string.h>
#include <cstring> // Äëÿ strcmp
#include"Graibach.h"
#include"comon.h"
#include"Rules.h"
#include"Lexem_table.h"
#include"MFST.h"
#include"AST.h"

namespace parser {
	void Parse(LT::Lexem_table table, std::list<Rule>);
}