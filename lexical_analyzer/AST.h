#pragma once
#include<vector>
#include<stack>
#include<queue>
#include"Rules.h"
#include"comon.h"
#include"Error.h"

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

		struct dfs {
			struct entry {
				int offset = 0;
				node* node = nullptr;
			};


			std::stack<entry> stack;
			//std::queue<entry> queue;

			
			node* Step();
		} DFS;
		void Reset();
	};

	void create_ast(program_struct& tree, RULE::GRB::GRBALPHABET start);
	void delete_node(node* sorce,node* node);
	void delete_node(node* node);
	int get_node_id(node* sorce, node* node);

	node** get_node_ref(node* from, node* to);
}