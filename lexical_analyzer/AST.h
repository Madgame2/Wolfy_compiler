#pragma once
#include<vector>
#include"Rules.h"
#include"comon.h"

namespace AST {

	enum class node_type {
		ID,
		Lit,
		general
	};

	enum class symbol_type {
		Terminal,
		NonTerminal
	};

	struct node {
		char symbol[LEXEMA_SIZE + 1] ={'\0'};

		std::vector<node*> links;

		node_type type = node_type::general;
		symbol_type symbol_type = symbol_type::Terminal;

		int table_id = -1;
	};

	struct  program_struct
	{
		node* root = nullptr;
	};

	void create_ast(program_struct& tree, RULE::GRB::GRBALPHABET start);
	void delete_node(node* node);
}