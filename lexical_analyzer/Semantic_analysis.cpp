#include"Semantic_analysis.h"
#include<iostream>

void semantic::Parse(AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table)
{
	tree.Reset();

	while (true)
	{
		AST::node* curent = tree.DFS.Step();

		if (curent == nullptr) {

			break;
		}

		std::cout << curent->symbol << std::endl;




	}
}
