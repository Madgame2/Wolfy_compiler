#pragma once
#include<string>
#include<vector>
#include"AST.h"
#include"Identification_table.h"
#include"Litetral_table.h"

namespace semantic {
	void Parse(AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table);


	namespace scope {

		struct node {
			node* parent = nullptr;
			std::vector<node*> childres;
			std::wstring node_name;

			struct data
			{

			};
		};

		struct scope {
			node* root = nullptr;

			std::stack<node* > last_scope;

			void add_new_scope(std::wstring name);
			void pop_scope();
		};

	}
}