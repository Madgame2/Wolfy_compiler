#pragma once
#include"AST.h"
#include"Lexem_table.h"
#include"Identification_table.h"
#include<list>
#include<stack>
#include"comon.h"


namespace POL {
	AST::node* build_tree(std::list<LT::Entry> expression,ID::ID_table& table);
}