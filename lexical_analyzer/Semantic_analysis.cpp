#include"Semantic_analysis.h"
#include<iostream>


IDType::Type get_terminal_context(AST::node* curent, ID::ID_table table) {
	switch (curent->type)
	{
	case AST::node_type::ID: {

		int table_id = curent->table_id;
		ID::Entry entry = ID::getEntry(table, table_id);

		return entry.id_type;

		break;
	}
	case AST::node_type::Lit: {

		return IDType::Type::None;
		break;
	}
	}
}

void semantic::Parse(AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table)
{
	tree.Reset();

	//std::stack<std::wstring> context_stack;
	scope::scope area_visibilyty;
	AST::node* buffer = nullptr;
	while (true)
	{
		
		AST::node* curent = tree.DFS.Step();

		if (curent == nullptr) {

			break;
		}

		std::cout << curent->symbol << std::endl;

		
		switch (curent->symbol_type)
		{
		case AST::symbol_type::NonTerminal: {

			if (strcmp(curent->symbol, "S") == 0) {

				area_visibilyty.add_new_scope(L"START");
			}
			else if (strcmp(curent->symbol, "M") == 0&& area_visibilyty.last_scope.top()->node_name!=L"MAIN") {
				area_visibilyty.add_new_scope(L"MAIN");
			}


			if ( true != true) {

			}

			break;
		}
		case AST::symbol_type::Terminal: {

			if (strcmp(curent->symbol, "g") == 0) {
				area_visibilyty.add_new_scope(L"GLOBAL");
			}
			else if (strcmp(curent->symbol, "}") == 0) {
				area_visibilyty.last_scope.pop();
			}
			else if (strcmp(curent->symbol, "?") == 0) {
				area_visibilyty.add_new_scope(L"IF");
			}
			else {

				IDType::Type id_type = get_terminal_context(curent, id_table);

				if (id_type == IDType::Type::Func &&buffer&& strcmp(buffer->symbol, "f") == 0) {
					//добавляем обьявление функции к этой облости видемости
					//создаем новую область видемости

					int table_id = curent->table_id;
					ID::Entry elem = ID::getEntry(id_table, table_id);

					area_visibilyty.add_new_scope(elem.name);
				}
			}
			break;
		}
		default:
			break;
		}

		buffer = curent;
	}
}

void semantic::scope::scope::add_new_scope(std::wstring name)
{
	semantic::scope::node* new_node = new semantic::scope::node();

	new_node->node_name = name;

	if (last_scope.empty()) {
		root = new_node;
	}
	else {
		last_scope.top()->childres.push_back(new_node);
	}
	last_scope.push(new_node);

}

void semantic::scope::scope::pop_scope()
{
	last_scope.pop();
}
