#include"Semantic_analysis.h"
#include<iostream>



semantic::data::Func_sign* defoult_functions_sign(int& size) {
	semantic::data::Func_sign* sign = new semantic::data::Func_sign[3];


	sign[0] = semantic::data::Func_sign(L"is_equal", DataType::Type::Int, { DataType::Type::String, DataType::Type::String });
	sign[1] = semantic::data::Func_sign(L"save_is_equal", DataType::Type::Int, { DataType::Type::String, DataType::Type::String, DataType::Type::Int});
	sign[2] = semantic::data::Func_sign(L"factorial", DataType::Type::Int, { DataType::Type::Int});

	size = 3;
	return sign;
}


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


void  get_conntext(ID::Entry& elem, semantic::scope::scope area) {
	try
	{
		elem.area = area.get_area(area.getvar(elem.name)).node_name;

	}
	catch (...)
	{
		elem.area = area.last_scope.top()->node_name;
	}

}

bool is_only_dec(std::wstring value) {
	for (auto i : value) {
		if (!isdigit(i)) {
			return false;
		}
	}
	return true;
}

bool is_only_bin(std::wstring value) {
	for (auto i: value) {
		if (i != L'1' && i != L'0') {
			return false;
		}
	}
	return true;
}

bool is_only_oct(std::wstring value) {
	for (auto i : value) {
		if (isdigit(i)) {
			int num = static_cast<int>(i) - L'0';

			if (!(num >= 0 && num <= 7)) {
				return false;
			}
		}

	}
	return true;
}

bool is_only_Hex(std::wstring& value) {
	std::wstring buffer;

	int index = 0;
	for (auto i : value) {
		if (isdigit(i)) {
			int num = static_cast<int>(i) - L'0';

			if (!(num >= 0 && num <= 7)) {
				return false;
			}
			buffer += i;
		}
		else {
			if (index == 0) buffer = L'0';

			if (!((i >= 'A' && i <= 'F') || (i >= 'a' && i <= 'f'))) {
				return false;
			}
			buffer += i;
		}
		index++;
	}
	value = buffer;

	return true;
}

void check_lit_struct(Lit_table::Element& lit,AST::node*curent) {

	switch (lit.my_notation)
	{
	case notations::notation::Dec:
		
		if (!is_only_dec(lit.value)) {
			throw Error::get_error_in(605, curent->line, curent->index);
		}

		break;
	case notations::notation::Bin:

		if (!is_only_bin(lit.value)) {
			throw Error::get_error_in(605, curent->line, curent->index);
		}

		break;
	case notations::notation::oct:

		if (!is_only_oct(lit.value)) {
			throw Error::get_error_in(605, curent->line, curent->index);
		}

		break;
	case notations::notation::Hex:

		if (!is_only_Hex(lit.value)) {
			throw Error::get_error_in(605, curent->line, curent->index);
		}

		break;
	default:
		break;
	}
}

bool is_convertable_types(DataType::Type& type1, DataType::Type& type2) {
	if (type1 == type2) {
		return true;
	}

	std::list<DataType::Type> convertable = RULE::convetable_dataType::save::save_convertable[type2];
	for (auto& elem : convertable)
	{
		if (elem == type1) {
			type2 = type1;
			return true;
		}
	}
	return false;
}


DataType::Type getExpressinType(AST::program_struct& tree, AST::node* curent, semantic::scope::scope area_visibilyty, ID::ID_table& id_table, Lit_table::Literal_table lit_table) {

	

	DataType::Type result;
	DataType::Type buffer = DataType::Type::None;
	DataType::Type curent_type = DataType::Type::None;
	while (curent!=nullptr &&curent->is_expression)
	{

		if (strcmp(curent->symbol, "i") == 0 || strcmp(curent->symbol, "l") == 0) {

			if (curent->type == AST::node_type::ID) {
				ID::Entry&  elem = ID::getEntry(id_table, curent->table_id);

				try
				{
					elem.d_type = *area_visibilyty.getvar(elem.name).d_type;
					get_conntext(elem, area_visibilyty);
					
				}
				catch (...)
				{
					throw Error::get_error_in(302, curent->line, curent->index);

				}

				curent_type = elem.d_type;

			}
			else if (curent->type == AST::node_type::Lit) {
				Lit_table::Element&  elem = Lit_table::find(lit_table, curent->table_id);
				curent_type = elem.d_type;

				check_lit_struct(elem,curent);

				if (elem.d_type == DataType::Type::String) throw Error::get_error_in(605, curent->line, curent->index);
				
			}
		}

		if (buffer != DataType::Type::None) {

			/*!is_convertable_types(buffer, curent_type)*/

			if (buffer!=curent_type) {
				throw Error::get_error_in(302, curent->line, curent->index);
			}
		}

		if (curent->type == AST::node_type::notation) {
			
			curent = tree.DFS.Step();
			continue;
		}

		buffer = curent_type;

		curent = tree.DFS.Step();
	}

	return buffer;
}


void semantic::Parse(AST::program_struct tree, std::list<semantic::data::global_elem>& global, ID::ID_table& id_table, Lit_table::Literal_table& lit_table)
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
	std::list<data::Func_sign> inited_func_sign;
	std::vector<info> info_for_sign;

	int size = 0;
	data::Func_sign* standart_functions = defoult_functions_sign(size);

	for (int i = 0; i < size; i++) {
		inited_func_sign.push_back(standart_functions[i]);
	}

	for (auto& elem : global) {
		inited_func_sign.push_back(elem.function);
	}

	bool is_params = false;

	bool is_expresion = false;
	while (true)
	{
		
		AST::node* curent = tree.DFS.Step();


		if (curent == nullptr) {

			break;
		}


		if (curent->is_expression) {

			DataType::Type expression = getExpressinType(tree, curent,area_visibilyty, id_table, lit_table);

			if (!is_params&&buffer->type == AST::node_type::ID) {
				ID::Entry elem = ID::getEntry(id_table,buffer->table_id);

				data::var varr = area_visibilyty.getvar(elem.name);

				if (!is_convertable_types(*varr.d_type, expression)) {
					throw Error::get_error_in(302, curent->line, curent->index);
				}

				curent->expresion_type = expression;
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
				if (area_visibilyty.has_this_func_sign(last_func, global,curent)) throw Error::get_error_in(301, curent->line, curent->index);
				retyrnable_type = last_func->returable_type;

				inited_func_sign.push_back(*last_func);

				last_func = nullptr;
			}
			else if (strcmp(curent->symbol, "?") == 0) {

				if (area_visibilyty.last_scope.top()->node_name != L"MAIN")
					control_flow_analyzer.create_new_branch();

				area_visibilyty.add_new_scope(L"IF");
			}
			else if (strcmp(curent->symbol, "w") == 0) {
				if (area_visibilyty.last_scope.top()->node_name != L"MAIN")
					control_flow_analyzer.create_new_branch();

				area_visibilyty.add_new_scope(L"WHILE");
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

						new_sign.func_unick_id = &elem.func_unick_id;
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

					try {
						area_visibilyty.getvar(var.name);
					}
					catch(...) {
						throw Error::get_error_in(302, curent->line, curent->index);
					}

					var.d_type = *area_visibilyty.getvar(var.name).d_type;
					get_conntext(var, area_visibilyty);
				}
				else if (id_type == IDType::Type::Param) {

					int table_id = curent->table_id;
					ID::Entry& param = ID::getEntry(id_table, table_id);

					area_visibilyty.add_param_to_last_func(param, last_func);
					area_visibilyty.add_new_var(param);
					get_conntext(param, area_visibilyty);

				}
				else if (strcmp(curent->symbol, "=") == 0 && !curent->is_double_operation) {
					continue;
				}
				else if ((strcmp(curent->symbol, "l") == 0) || (strcmp(curent->symbol, "i") == 0)) {
					if(curent->is_param){
						DataType::Type type;
						int id = curent->table_id;
						if (curent->type == AST::node_type::ID) {

							try {
								type = *area_visibilyty.getvar(ID::getEntry(id_table, id).name).d_type;
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

							check_lit_struct(Lit_table::find(lit_table, id),curent);

						}


						buffer_sing.top().params.push_back(&type);
					}
					else if ((strcmp(buffer->symbol, "i") == 0)) {

						std::wstring name = ID::getEntry(id_table, buffer->table_id).name;
						data::var& varyable = area_visibilyty.getvar(name);

						int id = curent->table_id;
						DataType::Type* type;
						if (curent->type == AST::node_type::ID) {
							std::wstring name = ID::getEntry(id_table, curent->table_id).name;
							type = area_visibilyty.getvar(name).d_type;
						}
						else {
							type = &Lit_table::find(lit_table, curent->table_id).d_type;
						}


						if (!is_convertable_types(*varyable.d_type, *type)) {
							throw Error::get_error_in(304, curent->line, curent->index);
						}
					}
					else if (strcmp(buffer->symbol, "r") == 0) {
						
						int id = curent->table_id;
						DataType::Type type;
						if (curent->type == AST::node_type::ID) {
							std::wstring name = ID::getEntry(id_table, curent->table_id).name;
							type = *area_visibilyty.getvar(name).d_type;
						}
						else {
							type = Lit_table::find(lit_table, curent->table_id).d_type;
						}


						if (!is_convertable_types(retyrnable_type, type)) {
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
				if (referens.func_unick_id) {
					*elem.func_unick_id = *referens.func_unick_id;
				}
				elem.returable_type = referens.returable_type;
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


bool is_func_declere(AST::node* curent, AST::node* buffer) {
	if (!buffer) return false;

	if (curent->type == AST::node_type::ID && strcmp(buffer->symbol, "f") == 0) {
		return true;
	}
	return false;
}

bool is_Var_declere(AST::node* curent, AST::node* buffer) {

	if (!buffer) return false;

	if (curent->type == AST::node_type::ID && strcmp(buffer->symbol, "t") == 0) {
		return true;
	}
	return false;
}

bool has_this_func_sign(std::list<semantic::data::global_elem> list, semantic::data::Func_sign curent) {

	int index = 0;
	for (auto elem : list) {
		if (index == list.size() - 1) {
			break;
		}

		if (elem.function == curent) {
			return true;
		}

		index++;
	}
	return false;
}

std::list<semantic::data::global_elem> semantic::Parse_Global(AST::program_struct tree, ID::ID_table& id_table)
{
	std::list<semantic::data::global_elem> result;

	AST::node* curent = nullptr;
	AST::node* buffer = nullptr;
	bool is_new_func = false;
	tree.Reset();

	int unick_id = 0;
	do {
		curent = tree.DFS.Step();

		if (curent&&is_func_declere(curent, buffer)) {
			ID::Entry& id = ID::getEntry(id_table, curent->table_id);

			data::Func_sign new_sign;
			new_sign.function_name = id.name;
			new_sign.returable_type = id.d_type;

			data::global_elem new_elem;
			new_elem.function = new_sign;
			id.func_unick_id = unick_id;
			new_elem.function.func_unick_id = &id.func_unick_id;

			result.push_back(new_elem);

			is_new_func = true;

			unick_id++;
		}
		else if (curent&&is_Var_declere(curent, buffer)) {
			ID::Entry& id = ID::getEntry(id_table, curent->table_id);

			result.back().function.params.push_back(&id.d_type);
		}
		else if (curent &&strcmp(curent->symbol, ")") == 0) {
			if (is_new_func) {

				if (has_this_func_sign(result, result.back().function)) {
					throw Error::get_error_in(301, curent->line, curent->index);
				}

				is_new_func = false;
			}
		}

		buffer = curent;

	} while (curent != nullptr);


	return result;
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

void semantic::scope::scope::add_new_var(ID::Entry& var)
{
	data::var new_var;

	new_var.id_type = var.id_type;
	new_var.d_type = &var.d_type;
	new_var.name = var.name;

	new_var.is_array = var.is_array;

	last_scope.top()->objects.vareiables.push_back(new_var);

}

void semantic::scope::scope::add_new_functoin(ID::Entry& func)
{
	data::Func_sign new_function;

	new_function.func_unick_id = &func.func_unick_id;
	*new_function.func_unick_id = data::func_id++;
	new_function.func_unick_id = &func.func_unick_id;
	new_function.function_name = func.name;
	new_function.returable_type = func.d_type;

	last_scope.top()->objects.functions.push_back(new_function);
}

void semantic::scope::scope::add_param_to_last_func(ID::Entry& param, data::Func_sign* function)
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

bool equal_sign(semantic::data::Func_sign elem1, semantic::data::Func_sign elem2) {
	if (elem1 == elem2) {
		return true;
	}
	return false;
}

bool semantic::scope::scope::has_this_func_sign(data::Func_sign* last_func, std::list<semantic::data::global_elem>& global, AST::node* curent)
{
	for (auto& elem : global) {
		if (equal_sign(elem.function, *last_func)) {


			if (elem.is_relisated) {
				throw Error::get_error_in(307, curent->line, curent->index);
			}
			else {
				elem.is_relisated = true;
			}
		}
	}

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

semantic::data::var& semantic::scope::scope::getvar(std::wstring name)
{
	std::stack<node*> stack = last_scope;

	while (!stack.empty()) {

		std::list<data::var>& list = stack.top()->objects.vareiables;

		for (auto& elem : list) {
			if (elem.name == name) {

				return elem;
			}
		}
		stack.pop();
	}
	throw NULL;
}

semantic::scope::node& semantic::scope::scope::get_area(data::var i)
{
	std::stack<node* > last = last_scope;

	while (!last.empty()) {

		for (auto& elem : last.top()->objects.vareiables) {
			if (elem.name == i.name) {
				return *last.top();
			}
		}
		last.pop();
	}
	throw NULL;
}

