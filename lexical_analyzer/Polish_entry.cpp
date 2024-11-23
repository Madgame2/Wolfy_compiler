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
			break;
		}
	}

	std::list<LT::Entry> build_polish_entry(std::list<LT::Entry> expression) {

		std::cout << "build polish entry:\n";
		for (auto elem : expression) {
			std::cout << elem.lexema;
		}
		std::cout << '\n';

		std::stack<LT::Entry> buffer;
		std::list<LT::Entry> polish_entry;
		for (auto elem : expression) {
			

			if (elem.IT_index != -1 || elem.Lit_index != -1) {
				polish_entry.push_back(elem);
			}
			else {

				char lexem = elem.lexema[0];
				if (lexem == '(') {

					buffer.push(elem);
				}
				else if(lexem ==')') {
					while (buffer.top().lexema[0] != '(')
					{
						polish_entry.push_back(buffer.top());
						buffer.pop();
					}
					buffer.pop();
				}
				else {
					operatoion_prioritet::operations prioritet  = get_priorete(lexem);

					 !buffer.empty() && get_priorete(buffer.top().lexema[0]) > prioritet ? polish_entry.push_back(elem) : buffer.push(elem);

				}
				
			}
		}

		while (!buffer.empty()) {
			polish_entry.push_back(buffer.top());
			buffer.pop();
		}

		return polish_entry;
	}

	AST::node* build_tree(std::list<LT::Entry> expression)
	{
		std::list<LT::Entry> polish_expr = build_polish_entry(expression);
		std::cout << "end build polish entry\n";

		for (auto elem : polish_expr) {
			std::cout << elem.lexema;
		}
		std::cout << '\n';

		return nullptr;
	}
}
