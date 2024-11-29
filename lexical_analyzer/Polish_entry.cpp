#include"Polish_entry.h"
#include<iostream>

namespace POL {

	operatoion_prioritet::operations get_priorete(const std::string& lexem) {
		if (lexem == "+")
			return operatoion_prioritet::operations::PLUS;
		else if (lexem == "-")
			return operatoion_prioritet::operations::MINUS;
		else if (lexem == "*")
			return operatoion_prioritet::operations::MULTIPLY;
		else if (lexem == "/")
			return operatoion_prioritet::operations::DIV;
		else if (lexem == "%")
			return operatoion_prioritet::operations::DIV;
		else if (lexem == "==")
			return operatoion_prioritet::operations::EQUAL; // Добавляем операцию сравнения
		else
			throw std::invalid_argument("Unknown operator: " + lexem);
	}

	std::list<LT::Entry> build_polish_entry(std::list<LT::Entry> expression) {
		std::cout << "build polish entry:\n";
		for (auto elem : expression) {
			std::cout << elem.lexema;
		}
		std::cout << '\n';

		std::stack<LT::Entry> buffer;
		std::list<LT::Entry> polish_entry;

		auto it = expression.begin();
		while (it != expression.end()) {
			auto& elem = *it;

			// Обрабатываем оператор "==" как одну лексему
			if (elem.lexema[0] == '=' && std::next(it) != expression.end() && std::next(it)->lexema[0] == '=') {
				// Создаём лексему для "=="
				LT::Entry combined_lexem;
				combined_lexem.lexema[0] = '=';  // первый символ
				combined_lexem.lexema[1] = '\0'; // завершающий нулевой символ для строки

				// Мы добавляем правильную лексему с двумя символами
				combined_lexem.lexema[1] = '=';

				// Заменяем текущую лексему на "=="
				elem = combined_lexem;

				// Пропускаем второй знак "="
				++it;
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
					while (!buffer.empty() && buffer.top().lexema[0] != '(') {
						polish_entry.push_back(buffer.top());
						buffer.pop();
					}
					buffer.pop(); // Удаляем '(' из стека
				}
				else {
					operatoion_prioritet::operations current_priority = get_priorete(elem.lexema);

					while (!buffer.empty() &&
						get_priorete(buffer.top().lexema) >= current_priority) {
						polish_entry.push_back(buffer.top());
						buffer.pop();
					}
					buffer.push(elem);
				}
			}
			++it;
		}

		// Очищаем стек, добавляя оставшиеся операторы
		while (!buffer.empty()) {
			polish_entry.push_back(buffer.top());
			buffer.pop();
		}

		return polish_entry;
	}



	std::list<AST::node*> refactor_list(std::list<LT::Entry> list) {

		std::list<AST::node*> new_list;

		for (auto elem : list) {
			AST::node* new_node = new AST::node();
			strcpy_s(new_node->symbol, sizeof(new_node->symbol), elem.lexema);
			new_node->symbol_type = AST::symbol_type::Terminal;

			if (elem.IT_index != -1) {
				new_node->type = AST::node_type::ID;
				new_node->table_id = elem.IT_index;
			}
			else if(elem.Lit_index != -1) {
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
