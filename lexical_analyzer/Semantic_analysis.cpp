#include"Semantic_analysis.h"
#include<iostream>


struct info {
	int line = 0;
	int pos = 0;
};

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

DataType::Type getExpressinType(AST::program_struct& tree, AST::node* curent, semantic::scope::scope area_visibilyty, ID::ID_table id_table, Lit_table::Literal_table lit_table) {

	

	DataType::Type result;
	DataType::Type buffer = DataType::Type::None;
	DataType::Type curent_type = DataType::Type::None;
	while (curent!=nullptr &&curent->is_expression)
	{

		if (strcmp(curent->symbol, "i") == 0 || strcmp(curent->symbol, "l") == 0) {






			if (curent->type == AST::node_type::ID) {
				ID::Entry  elem = ID::getEntry(id_table, curent->table_id);

				if (!area_visibilyty.has_this_var(elem.name)) {
					throw Error::get_error_in(302, curent->line, curent->index);
				}

				curent_type = elem.d_type;

			}
			else if (curent->type == AST::node_type::Lit) {
				Lit_table::Element  elem = Lit_table::find(lit_table, curent->table_id);
				curent_type = elem.d_type;
			}
		}

		if (buffer != DataType::Type::None) {
			if (buffer != curent_type) {
				throw Error::get_error_in(302, curent->line, curent->index);
			}
		}

		buffer = curent_type;

		curent = tree.DFS.Step();
	}

	return buffer;
}

void  get_conntext(ID::Entry& elem,semantic::scope::scope area) {
	while (!area.last_scope.empty())
	{
		elem.area += area.last_scope.top()->node_name;
		area.pop_scope();
	}
}

void semantic::Parse(AST::program_struct tree, ID::ID_table& id_table, Lit_table::Literal_table& lit_table)
{
	tree.Reset();

	Constrol_flow::Control_flow control_flow_analyzer;

	//std::stack<std::wstring> context_stack;
	scope::scope area_visibilyty;
	AST::node* buffer = nullptr;
	data::Func_sign* last_func = nullptr;
	DataType::Type retyrnable_type;
	std::wstring last_func_name;
	
	std::stack<data::Func_sign> buffer_sing;
	std::vector<data::Func_sign> sign_for_checking;
	std::vector<data::Func_sign> inited_func_sign;
	std::vector<info> info_for_sign;

	bool is_params = false;

	bool is_expresion = false;
	while (true)
	{
		
		AST::node* curent = tree.DFS.Step();

		if (curent == nullptr) {

			break;
		}

		std::cout << curent->symbol << std::endl;


		if (curent->is_expression) {

			DataType::Type expression = getExpressinType(tree, curent,area_visibilyty, id_table, lit_table);

			if (!is_params&&buffer->type == AST::node_type::ID) {
				ID::Entry elem = ID::getEntry(id_table,buffer->table_id);

				data::var varr = area_visibilyty.getvar(elem.name);

				if (varr.d_type != expression) {
					throw Error::get_error_in(302, curent->line, curent->index);
				}
			}
			else if (is_params) {
				buffer_sing.top().params.push_back(&expression);
			}
		}

		if (is_params && !curent->is_param) {
			
			sign_for_checking.push_back(buffer_sing.top());
			info new_info;
			new_info.line = curent->line;
			new_info.pos = curent->index;
			info_for_sign.push_back(new_info);

			buffer_sing.pop();
			if(buffer_sing.empty())
			is_params = false;
		}
		
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

					if (control_flow_analyzer.curent->id== BRANCH_BEGIN_DEFOULT_ID) {

						control_flow_analyzer.analyze() ? NULL : throw Error::get_error_in(303, curent->line, curent->index);
					}
					else {
						control_flow_analyzer.merge_last();
					}
				}

				area_visibilyty.last_scope.pop();
			}
			else if (last_func!=nullptr&&strcmp(curent->symbol, ")") == 0) {
				if (area_visibilyty.has_this_func_sign(last_func)) throw Error::get_error_in(301, curent->line, curent->index);
				retyrnable_type = last_func->returable_type;

				inited_func_sign.push_back(*last_func);

				last_func = nullptr;
			}
			else if (strcmp(curent->symbol, "?") == 0) {
				area_visibilyty.add_new_scope(L"IF");

				control_flow_analyzer.create_new_branch();
			}
			else if (strcmp(curent->symbol, "r") == 0) {
				control_flow_analyzer.set_returnable();
			}
			else {

				IDType::Type id_type = get_terminal_context(curent, id_table);

				if (id_type == IDType::Type::Func) {
					int table_id = curent->table_id;
					ID::Entry& elem = ID::getEntry(id_table, table_id);

					//добавляем обьявление функции к этой облости видемости
					if (buffer && strcmp(buffer->symbol, "f") == 0) {

						area_visibilyty.add_new_functoin(elem);
						last_func = &area_visibilyty.last_scope.top()->objects.functions.back();
						area_visibilyty.add_new_scope(elem.name);

						get_conntext(elem, area_visibilyty);

						control_flow_analyzer.beign();
					}
					else {
						data::Func_sign new_sign;

						if (is_params) {
							buffer_sing.top().params.push_back(&new_sign.returable_type);
						}

						new_sign.function_name = elem.name;
						buffer_sing.push(new_sign);
						is_params = true;
					}
				}
				else if (id_type == IDType::Type::Var && buffer && strcmp(buffer->symbol, "t") == 0) {

					//к этой области видимости добовляем новуб переменную
					int table_id = curent->table_id;
					ID::Entry& var = ID::getEntry(id_table, table_id);

					if (area_visibilyty.has_this_var(var.name)) throw Error::get_error_in(300, curent->line, curent->index);

					area_visibilyty.add_new_var(var);
					get_conntext(var, area_visibilyty);
				}
				else if (id_type == IDType::Type::Var && buffer && strcmp(buffer->symbol, "t") != 0&&!curent->is_param) {

					//к этой области видимости добовляем новуб переменную
					int table_id = curent->table_id;
					ID::Entry& var = ID::getEntry(id_table, table_id);

					if (!area_visibilyty.has_this_var(var.name)) {
						throw Error::get_error_in(302, curent->line, curent->index);
					}
					var.d_type = area_visibilyty.getvar(var.name).d_type;
					get_conntext(var, area_visibilyty);
				}
				else if (id_type == IDType::Type::Param) {
					int table_id = curent->table_id;
					ID::Entry& param = ID::getEntry(id_table, table_id);

					area_visibilyty.add_param_to_last_func(param, last_func);
					get_conntext(param, area_visibilyty);
				}
				//else if (id_type == IDType::Type::Param) {
				//	if (strcmp(buffer->symbol, "f") == 0) {

				//	}
				//	else {

				//	}
				//}
				else if (strcmp(curent->symbol, "=") == 0 && !curent->is_double_operation) {
					continue;
				}
				else if ((strcmp(curent->symbol, "l") == 0) || (strcmp(curent->symbol, "i") == 0)) {
					if(curent->is_param){
						DataType::Type type;
						int id = curent->table_id;
						if (curent->type == AST::node_type::ID) {

							try {
								type = area_visibilyty.getvar(ID::getEntry(id_table, id).name).d_type;
								get_conntext(ID::getEntry(id_table, curent->table_id), area_visibilyty);
							}
							catch(...){
								throw Error::get_error_in(302, curent->line, curent->index);
							}

							//type = ID::getEntry(id_table, id).d_type;
						}
						else if(curent->type == AST::node_type::Lit)
						{
							type = Lit_table::find(lit_table, id).d_type;

						}


						buffer_sing.top().params.push_back(&type);
					}
					else if ((strcmp(buffer->symbol, "i") == 0)) {

						std::wstring name = ID::getEntry(id_table, buffer->table_id).name;
						data::var varyable = area_visibilyty.getvar(name);

						int id = curent->table_id;
						DataType::Type type;
						if (curent->type == AST::node_type::ID) {
							std::wstring name = ID::getEntry(id_table, curent->table_id).name;
							type = area_visibilyty.getvar(name).d_type;
						}
						else {
							type = Lit_table::find(lit_table, curent->table_id).d_type;
						}


						if (varyable.d_type != type) {
							throw Error::get_error_in(304, curent->line, curent->index);
						}
					}
					else if (strcmp(buffer->symbol, "r") == 0) {
						
						int id = curent->table_id;
						DataType::Type type;
						if (curent->type == AST::node_type::ID) {
							std::wstring name = ID::getEntry(id_table, curent->table_id).name;
							type = area_visibilyty.getvar(name).d_type;
						}
						else {
							type = Lit_table::find(lit_table, curent->table_id).d_type;
						}


						if (retyrnable_type != type) {
							throw Error::get_error_in(305, curent->line, curent->index);
						}
					}

				}
				else if (strcmp(curent->symbol, "<") == 0 && strcmp(buffer->symbol, "c") == 0) {
				if (!curent->console_operation) {
					throw Error::get_error_in(601, curent->line, curent->index);
					}
				}
				else if(strcmp(curent->symbol, "+") == 0 ||
						strcmp(curent->symbol, "-") == 0 ||
						strcmp(curent->symbol, "*") == 0 ||
						strcmp(curent->symbol, "/") == 0 ||
						strcmp(curent->symbol, "%") == 0) {
					buffer->is_expression = curent->is_expression;
					is_expresion = true;
				}

				else if (strcmp(buffer->symbol, "r") == 0 && retyrnable_type != DataType::Type::None) {
					throw Error::get_error_in(305, curent->line, curent->index);
				}

			}
			break;
		}
		}

		buffer = curent;
	}


	int i = 0;
	for (auto& elem : sign_for_checking) {

		//elem.ref_returnable_type = DataType::Type::String;

		bool find = false;
		for (auto& referens : inited_func_sign) {
			if (elem == referens) {
				*elem.ref_returnable_type = referens.returable_type;
				find = true;
				break;
			}
		}

		if (!find) {
			throw Error::get_error_in(306, info_for_sign[i].line, info_for_sign[i].pos);
		}
		i++;
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
	function->params.push_back(&param.d_type);
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
					return true;
				}
				else {
					return false;
				}
			}
		}
		index++;
	}

	return false;
}

semantic::data::var semantic::scope::scope::getvar(std::wstring name)
{
	std::stack<node*> stack = last_scope;

	while (!stack.empty()) {

		std::list<data::var> list = stack.top()->objects.vareiables;

		for (auto& elem : list) {
			if (elem.name == name) {

				return elem;
			}
		}
		stack.pop();
	}
	throw NULL;
}

