#include"Polish_entry.h"

namespace POL {

	operatoion_prioritet::operations get_priorete(char lexem) {
		switch (lexem)
		{
		case'+':
			return operatoion_prioritet::operations::PLUS;
			break;
		case'-':
			return operatoion_prioritet::operations::MINUS;
			break;
		case'*':
			return operatoion_prioritet::operations::MULTIPLY;
			break;
		case'/':
			return operatoion_prioritet::operations::DIV;
			break;
		case'%':
			return operatoion_prioritet::operations::DIV;

		case'=':
			return operatoion_prioritet::operations::EQUAL;
			break;
		}
	}

	std::list<LT::Entry> build_polish_entry(std::list<LT::Entry> expression, ID::ID_table& id_table) {

		bool is_func = false;
		bool first_param = false;
		std::stack<LT::Entry> buffer;
		std::list<LT::Entry> polish_entry;
		std::stack<ID::Entry*> last_func;
		for (auto& elem : expression) {


			if (strcmp( elem.lexema , ",")==0) {
				if (is_func) {
					last_func.top()->arg_count++;
				}
				 
				continue;
			}

			if (elem.IT_index != -1 || elem.Lit_index != -1) {
				if (elem.IT_index != -1) {
					ID::Entry& id = ID::getEntry(id_table, elem.IT_index);
					if (id.id_type == IDType::Type::Func) {
						is_func = true;
						last_func.push(&id);

						buffer.push(elem);
					}
					else {

						polish_entry.push_back(elem);
					}
				}
				else {
					polish_entry.push_back(elem);
				}

				if (!buffer.empty() && !last_func.empty() && buffer.top().lexema[0] == '(') {
					if (!first_param) {
						last_func.top()->arg_count = 1;
						first_param = true;
					}
				}
			}
			else {

				char lexem = elem.lexema[0];
				if (lexem == '(') {

					buffer.push(elem);
				}
				else if (lexem == ')') {
					while (buffer.top().lexema[0] != '(')
					{
						polish_entry.push_back(buffer.top());
						buffer.pop();
					}
					buffer.pop();
					if (is_func) {
						last_func.pop();
					}
					if (last_func.empty()) {
						is_func = false;
					}

				}
				else if (lexem == 's') {
					polish_entry.push_back(elem);
				}
				else {
					operatoion_prioritet::operations curent_prioritet = get_priorete(lexem);

					if (!buffer.empty()) {
						operatoion_prioritet::operations buffer_priorety = get_priorete(buffer.top().lexema[0]);

						if (buffer.top().lexema[0] == '(') {
							buffer.push(elem);
						}
						else if (buffer_priorety < curent_prioritet) {
							polish_entry.push_back(elem);
						}
						else if (buffer_priorety == curent_prioritet) {
							polish_entry.push_back(buffer.top());
							buffer.pop();
							buffer.push(elem);
						}
						else {
							polish_entry.push_back(buffer.top());
							buffer.pop();
							buffer.push(elem);
						}
					}
					else {
						buffer.push(elem);
					}

					//!buffer.empty() && get_priorete(buffer.top().lexema[0]) > prioritet ? polish_entry.push_back(elem) : buffer.push(elem);

				}

			}
		}

		while (!buffer.empty()) {
			polish_entry.push_back(buffer.top());
			buffer.pop();
		}

		return polish_entry;
	}

	std::list<AST::node*> refactor_list(std::list<LT::Entry> list) {

		std::list<AST::node*> new_list;

		for (auto& elem : list) {
			AST::node* new_node = new AST::node();
			strcpy_s(new_node->symbol, sizeof(new_node->symbol), elem.lexema);
			new_node->symbol_type = AST::symbol_type::Terminal;
			new_node->is_double_operation = elem.is_double_oeration;

			if (elem.IT_index != -1) {
				new_node->type = AST::node_type::ID;
				new_node->table_id = elem.IT_index;
			}
			else if (elem.Lit_index != -1) {
				new_node->type = AST::node_type::Lit;
				new_node->table_id = elem.Lit_index;
			}

			if (strcmp(elem.lexema, "s") == 0) {
				new_node->type = AST::node_type::notation;
			}

			new_list.push_back(new_node);
		}

		return new_list;
	}


	AST::node* build_tree(std::list<LT::Entry> expression, ID::ID_table& table, std::ofstream* log) {
		// Преобразуем выражение в обратную польскую запись
		std::list<LT::Entry> polish_expr = build_polish_entry(expression,table);


		for (auto elem : polish_expr) {
			*log << elem.lexema;
		}
		*log << std::endl;

		// Преобразуем в список узлов
		std::list<AST::node*> node_list = refactor_list(polish_expr);
		std::stack<AST::node*> stack;
		std::stack<AST::node*> buffer;

		// Переносим элементы списка в стек
		for (auto it = node_list.rbegin(); it != node_list.rend(); ++it) {
			stack.push(*it);
		}

		while (stack.size() !=0) {
			AST::node* current = stack.top();
			stack.pop();

			AST::node_type type = current->type;
			IDType::Type id_type = IDType::Type::None;

			if (type == AST::node_type::ID) {
				id_type = ID::getEntry(table, current->table_id).id_type;
			}

			if (type == AST::node_type::ID && id_type != IDType::Type::Func || type == AST::node_type::Lit) {
				buffer.push(current);
			}
			else {
				if (type == AST::node_type::notation) {
					AST::node* new_node = stack.empty() ? nullptr : stack.top();
					if (new_node) stack.pop();

					new_node->is_expression = true;
					current->links.push_back(new_node);

					buffer.push(current);
				}
				else if (id_type != IDType::Type::Func) {
					AST::node* right = buffer.empty() ? nullptr : buffer.top();
					if (right) buffer.pop();
					AST::node* left = buffer.empty() ? nullptr : buffer.top();
					if (left) buffer.pop();

					if (node_list.size() > 2) {
						current->is_expression = true;
						if (right) right->is_expression = true;
						if (left) left->is_expression = true;
					}

					if (left) current->links.push_back(left);
					if (right) current->links.push_back(right);



					buffer.push(current);
				}
				else {
					AST::node* func = current;
					//IDType::Type id_type = IDType::Type::None;
					//if (buffer.top()->type == AST::node_type::ID) {
					//	id_type = ID::getEntry(table, buffer.top()->table_id).id_type;
					//}

					ID::Entry& id = ID::getEntry(table, func->table_id);

					int argv_coutn_save = id.arg_count;
					while (id.arg_count!=0) {
						func->links.push_back(buffer.top());
						buffer.top()->is_param = true;
						buffer.pop();
						id.arg_count--;
					}
					id.arg_count = argv_coutn_save;

					std::reverse(func->links.begin(), func->links.end());

					buffer.push(func);
				}
			}
		}

		*log << '\n';
		return buffer.top();
	}
}
