#include"Polish_entry.h"
#include<iostream>

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

	std::list<LT::Entry> build_polish_entry(std::list<LT::Entry> expression) {

		std::cout << "build polish entry:\n";
		for (auto& elem : expression) {
			std::cout << elem.lexema;
		}
		std::cout << '\n';

		bool is_func = false;
		std::stack<LT::Entry> buffer;
		std::list<LT::Entry> polish_entry;
		for (auto& elem : expression) {


			if (strcmp( elem.lexema , ",")==0) {
				continue;
			}

			if (elem.IT_index != -1 || elem.Lit_index != -1) {
				polish_entry.push_back(elem);
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
				}
				else {
					operatoion_prioritet::operations curent_prioritet = get_priorete(lexem);

					if (!buffer.empty()) {
						operatoion_prioritet::operations buffer_priorety = get_priorete(buffer.top().lexema[0]);

						if (buffer.top().lexema[0] == '(') {
							buffer.push(elem);
						}
						else if (buffer_priorety > curent_prioritet) {
							polish_entry.push_back(elem);
						}
						else if (buffer_priorety == curent_prioritet) {
							polish_entry.push_back(buffer.top());
							buffer.pop();
							buffer.push(elem);
						}
						else {
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

			new_list.push_back(new_node);
		}

		return new_list;
	}


	//AST::node* build_tree(std::list<LT::Entry> expression, ID::ID_table table)
	//{
	//	std::list<LT::Entry> polish_expr = build_polish_entry(expression);
	//	std::cout << "end build polish entry\n";
	//	std::list<AST::node*> new_list = refactor_list(polish_expr);

	//	std::stack<AST::node*> buffer;

	//	for (auto elem : new_list) {
	//		std::cout << elem->symbol;
	//	}

	//	for (auto elem : new_list) {
	//		AST::node_type type = elem->type;
	//		IDType::Type id_type = IDType::Type::None;
	//		if (type == AST::node_type::ID) {
	//			id_type = ID::getEntry(table, elem->table_id).id_type;
	//		}


	//		if (elem->type == AST::node_type::ID && id_type!=IDType::Type::Func || elem->type == AST::node_type::Lit) {
	//			buffer.push(elem);
	//		}
	//		else
	//		{

	//			if (id_type != IDType::Type::Func) {
	//				AST::node* right = nullptr;
	//				AST::node* left = nullptr;

	//				AST::node* operation = elem;
	//				if (!buffer.empty()) {
	//					right = buffer.top();
	//					buffer.pop();
	//				}

	//				if (!buffer.empty()) {
	//					left = buffer.top();
	//					buffer.pop();
	//				}

	//				if (new_list.size() > 2) {
	//					operation->is_expression = true;
	//					right != nullptr ? right->is_expression = true : NULL;
	//					left != nullptr ? left->is_expression = true : NULL;
	//				}
	//				if (left != nullptr) operation->links.push_back(left);
	//				if (right != nullptr) operation->links.push_back(right);

	//				buffer.push(operation);
	//			}
	//			else {
	//				AST::node* root = elem;
	//				while (!buffer.empty())
	//				{
	//					root->links.push_back(buffer.top());
	//				}

	//				buffer.push(root);
	//			}
	//		}
	//	}
	//	std::cout << '\n';

	//	return buffer.top();
	//}


	AST::node* build_tree(std::list<LT::Entry> expression, ID::ID_table table) {
		// Преобразуем выражение в обратную польскую запись
		std::list<LT::Entry> polish_expr = build_polish_entry(expression);
		std::cout << "End build polish entry\n";

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
				if (id_type != IDType::Type::Func) {
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
					IDType::Type id_type = IDType::Type::None;
					if (stack.top()->type == AST::node_type::ID) {
						id_type = ID::getEntry(table, stack.top()->table_id).id_type;
					}


					while (!stack.empty() && id_type != IDType::Type::Func) {
						func->links.push_back(stack.top());
						stack.top()->is_param = true;
						stack.pop();
					}

					std::reverse(func->links.begin(), func->links.end());

					buffer.push(func);
				}
			}
		}

		std::cout << '\n';
		return buffer.top();
	}
}
