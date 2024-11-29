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

	Constrol_flow::Control_flow control_flow_analyzer;

	//std::stack<std::wstring> context_stack;
	scope::scope area_visibilyty;
	AST::node* buffer = nullptr;
	data::Func_sign* last_func = nullptr;
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


			break;
		}
		case AST::symbol_type::Terminal: {

			if (strcmp(curent->symbol, "g") == 0) {
				area_visibilyty.add_new_scope(L"GLOBAL");
			}
			else if (strcmp(curent->symbol, "}") == 0) {

				if (control_flow_analyzer.is_active) {

					if (control_flow_analyzer.curent == control_flow_analyzer.root) {

						control_flow_analyzer.analyze();
					}
					else {
						control_flow_analyzer.merge_last();
					}
				}

				last_func = nullptr;
				area_visibilyty.last_scope.pop();
			}
			else if (last_func!=nullptr&&strcmp(curent->symbol, ")") == 0) {
				if (area_visibilyty.has_this_func_sign(last_func)) throw Error::get_error_in(301, curent->line, curent->index);
				last_func = nullptr;
			}
			else if (strcmp(curent->symbol, "?") == 0) {
				area_visibilyty.add_new_scope(L"IF");

				control_flow_analyzer.create_new_branch();
			}
			else {

				IDType::Type id_type = get_terminal_context(curent, id_table);

				if (id_type == IDType::Type::Func &&buffer&& strcmp(buffer->symbol, "f") == 0) {
					//добавляем обьявление функции к этой облости видемости

					int table_id = curent->table_id;
					ID::Entry elem = ID::getEntry(id_table, table_id);


					area_visibilyty.add_new_functoin(elem);
					last_func = &area_visibilyty.last_scope.top()->objects.functions.back();
					area_visibilyty.add_new_scope(elem.name);



					control_flow_analyzer.beign();
				}
				else if (id_type == IDType::Type::Var && buffer && strcmp(buffer->symbol, "t") == 0) {

					//к этой области видимости добовляем новуб переменную
					int table_id = curent->table_id;
					ID::Entry var = ID::getEntry(id_table, table_id);

					if (area_visibilyty.has_this_var(var.name)) throw Error::get_error_in(300, curent->line, curent->index);

					area_visibilyty.add_new_var(var);
				}
				else if (id_type == IDType::Type::Var && buffer && strcmp(buffer->symbol, "t") != 0) {

					//к этой области видимости добовляем новуб переменную
					int table_id = curent->table_id;
					ID::Entry var = ID::getEntry(id_table, table_id);

					if (!area_visibilyty.has_this_var(var.name)) {
						throw Error::get_error_in(302, curent->line, curent->index);
					}
				}
				else if (id_type == IDType::Type::Param) {
					int table_id = curent->table_id;
					ID::Entry param = ID::getEntry(id_table, table_id);

					area_visibilyty.add_param_to_last_func(param, last_func);
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
		new_node->parent = last_scope.top();
		last_scope.top()->childres.push_back(new_node);
	}
	last_scope.push(new_node);

}

void semantic::scope::scope::add_new_var(ID::Entry var)
{
	data::var new_var;

	new_var.id_type = var.id_type;
	new_var.d_type = var.d_type;
	new_var.name = var.name;

	new_var.is_array = var.is_array;

	last_scope.top()->objects.vareiables.push_back(new_var);

}

void semantic::scope::scope::add_new_functoin(ID::Entry func)
{
	data::Func_sign new_function;

	new_function.function_name = func.name;
	new_function.returable_type = func.d_type;

	last_scope.top()->objects.functions.push_back(new_function);
}

void semantic::scope::scope::add_param_to_last_func(ID::Entry param, data::Func_sign* function)
{
	function->params.push_back(param.d_type);
}

void semantic::scope::scope::pop_scope()
{
	last_scope.pop();
}

bool semantic::scope::scope::has_this_var(std::wstring name)
{
	std::list<data::var> list = last_scope.top()->objects.vareiables;

	for (auto& elem : list) {
		if (elem.name == name) {
			return true;
		}
	}

	return false;
}

bool semantic::scope::scope::has_this_func_sign(data::Func_sign* last_func)
{
	std::list<data::Func_sign> list = last_scope.top()->parent->objects.functions;

	int index = 0;
	for (auto& func : list) {
		if (func.function_name == last_func->function_name) {

			if (func.params.size() == last_func->params.size()) {

				if (index != list.size()-1) {
					if (func.params.empty()) return true;

					int param_size = func.params.size();
					for (int i = 0; i < param_size; i++) {
						if (func.params[i] != last_func->params[i]) {
							return false;
						}
					}
				}
				else {
					return false;
				}
			}
		}
		index++;
	}

	return true;
}
