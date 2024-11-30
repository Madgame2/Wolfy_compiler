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

		std::stack<LT::Entry> buffer;
		std::list<LT::Entry> polish_entry;
		for (auto& elem : expression) {


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

			if (elem.IT_index != -1) {
				new_node->type = AST::node_type::ID;
				new_node->table_id = elem.IT_index;
			}
			else if (elem.Lit_index != -1) {
				new_node->type = AST::node_type::Lit;
				new_node->table_id = elem.IT_index;
			}

			new_list.push_back(new_node);
		}

		return new_list;
	}


	AST::node* build_tree(std::list<LT::Entry> expression)
	{
		std::list<LT::Entry> polish_expr = build_polish_entry(expression);
		std::cout << "end build polish entry\n";
		std::list<AST::node*> new_list = refactor_list(polish_expr);

		std::stack<AST::node*> buffer;

		for (auto elem : new_list) {
			std::cout << elem->symbol;
		}

		for (auto elem : new_list) {
			if (elem->type == AST::node_type::ID || elem->type == AST::node_type::Lit) {
				buffer.push(elem);
			}
			else
			{
				AST::node* operation = elem;
				AST::node* right = buffer.top();
				buffer.pop();
				AST::node* left = buffer.top();
				buffer.pop();
				

				operation->links.push_back(left);
				operation->links.push_back(right);
				
				buffer.push(operation);
			}
		}
		std::cout << '\n';

		return buffer.top();
	}
}
