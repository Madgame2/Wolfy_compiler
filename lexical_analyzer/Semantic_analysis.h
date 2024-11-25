#pragma once
#include"AST.h"
#include"Identification_table.h"
#include"Litetral_table.h"

namespace semantic {
	void Parse(AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table);
}