#include"Parser.h"

namespace parser {

	struct info
	{
		int  index = 0;
		int line = 0;
		int error = 0;
	};

	struct AST_info {
		AST::node* curent_node;
		int count_rules;


		AST_info(AST::node* curent, int count_rules) {
			this->count_rules = count_rules;
			this->curent_node = curent;
		}
	};

	AST::node* create_new_leaf(LT::Entry elem) {
		AST::node* new_node = new AST::node;


		strncpy_s(new_node->symbol, elem.lexema, sizeof(new_node->symbol)-1);
		new_node->symbol_type = AST::symbol_type::Terminal;
		
		if (elem.IT_index != -1) {
			new_node->type = AST::node_type::ID;
		}
		else if (elem.Lit_index != -1) {
			new_node->type = AST::node_type::Lit;
		}
		else {
			new_node->type = AST::node_type::general;
		}


		return new_node;
	}
	AST::node* create_new_NT(GRBALPHABET ch) {
		AST::node* new_node = new AST::node;

		new_node->symbol_type = AST::symbol_type::NonTerminal;
		new_node->symbol[0] = (char)-ch;
		new_node->symbol[1] = '\0';

		return new_node;
	}

	void Parse(LT::Lexem_table table, std::list<Rule> rules)
	{
		info data;

		std::stack<info> info_saves;
		std::stack<AST_info> struct_stack;


		GRB::Greibach grb;
		GRB::get_GRB(grb, NS("S"), TS("$"), rules);

		MFST::MFST mfst;
		MFST::create_MFST(mfst, table, grb);

		AST::program_struct tree;
		AST::create_ast(tree, NS("S"));



		while (true)
		{
			if (table.table[data.index].lexema == "|") {
				data.index++;
				data.line++;
			}

			auto buffer_temp = mfst.buffer;
			//ОТладка
			while (!buffer_temp.empty()) {
				std::cout << (grb.isT(buffer_temp.top()) ? (char)buffer_temp.top() : (char)(-(buffer_temp.top())));
				buffer_temp.pop();
			}
			std::cout << '\n';

			
			int chain_size;
			GRBALPHABET GRB_buffer = mfst.buffer.top();
			MFST::Results res = mfst.step(data.error, chain_size);



			switch (res)
			{
			case MFST::Results::OK_RESUULT:

				//к текушей ноде AST добавлем новый лист
				struct_stack.top().curent_node->links.push_back(create_new_leaf(table.table[data.index]));
				struct_stack.top().count_rules--;


				data.index++;

				break;
			case MFST::Results::FRONG_SYMBOL:

				//откатываемся на предыдушее сохранение 

				try {
					mfst.get_last_save();
					mfst.chain_id++;


					data = info_saves.top();
					info_saves.pop();

					AST::delete_node(struct_stack.top().curent_node);
					struct_stack.pop();
				}
				catch(...) {
					throw Error::get_error_in(data.error, data.line, data.index);
				}

				break;
			case MFST::Results::FIND_RULE:
				
				struct_stack.push(AST_info(create_new_NT(GRB_buffer), chain_size));

				//Просто продолжаем
				info_saves.push(data);

				break;
			case MFST::Results::NO_RULE:

				//исключение
				AST::delete_node(tree.root);

				throw Error::get_error_in(data.error, data.line, data.index);

				break;
			case MFST::Results::FATAL_ERRROR:

				AST::delete_node(tree.root);
				
				throw Error::get_error(0);
				//исключение неизвестная ошибка
				break;
			case MFST::Results::LENTA_END_GOOD:

				return;
				break;

			case MFST::Results::FILE_EMPTY:

					return;
				break;
			}


		}
	}

}
