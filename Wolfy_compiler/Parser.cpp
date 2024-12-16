#include"Parser.h"

namespace parser {

	struct AST_node_info {
		AST::node* curent_node;
		int count_rules;

		bool is_expression = false;
		AST_node_info(AST::node* curent, int count_rules) {
			this->count_rules = count_rules;
			this->curent_node = curent;
		}
		AST_node_info(AST::node* curent, int count_rules, bool is_expresion) {
			this->count_rules = count_rules;
			this->curent_node = curent;
			this->is_expression = is_expresion;
		}
	};
	struct expression_data {
		int begin = -1;
		int end = -1;

		AST::node* prerent_node = nullptr;
	};
	struct info
	{
		int  index = 0;
		int line = 0;
		int error = 0;
		AST::node* last_node = nullptr;

		std::stack<AST_node_info> NT_node_stuct;
		std::map<AST::node*, expression_data>  expressions;
	};

	AST::node* create_new_leaf(LT::Entry elem) {
		AST::node* new_node = new AST::node;

		new_node->line = elem.source_code_line;

		strncpy_s(new_node->symbol, elem.lexema, sizeof(new_node->symbol)-1);
		new_node->symbol_type = AST::symbol_type::Terminal;
		new_node->is_double_operation = elem.is_double_oeration;
		new_node->console_operation = elem.console_operations;

		if (elem.IT_index != -1) {
			new_node->type = AST::node_type::ID;
			new_node->table_id = elem.IT_index;
		}
		else if (elem.Lit_index != -1) {
			new_node->type = AST::node_type::Lit;
			new_node->table_id = elem.Lit_index;

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

	AST::program_struct Parse(LT::Lexem_table table, ID::ID_table id_table, std::list<Rule> rules, std::ofstream* log)
	{
		info data;

		std::stack<info> info_saves;
		std::stack<AST_node_info> NT_node_struct;						//Стек  вложенности нетерминальных символов 


		GRB::Greibach grb;
		GRB::get_GRB(grb, NS("S"), TS("$"), rules);

		MFST::MFST mfst;
		MFST::create_MFST(mfst, table, grb);

		AST::program_struct tree;
		
		bool is_active = true;
		ID::Entry* last_func_id = nullptr;
		while (is_active)
		{
			if (table.table[data.index].lexema == "|") {
				data.index++;
				data.line++;
			}

			auto buffer_temp_mfst = mfst.buffer;


			//ОТладка
			std::string stack_str;
			while (!buffer_temp_mfst.empty()) {
				stack_str +=(grb.isT(buffer_temp_mfst.top()) ? (char)buffer_temp_mfst.top() : (char)(-(buffer_temp_mfst.top())));
				buffer_temp_mfst.pop();
			}
			//std::cout << '\n';

			auto buffer_temp = NT_node_struct;
			std::string nT_stack;
			bool was_E = false;
			while (!buffer_temp.empty())
			{
				if (buffer_temp.top().curent_node->symbol[0] == 'E'|| buffer_temp.top().curent_node->symbol[0] == 'O') {
					if (!was_E) {
						was_E = true;
					}
					else {
						buffer_temp.pop();
						continue;
					}
				}
				nT_stack += std::string(buffer_temp.top().curent_node->symbol);
				nT_stack += ' ' + std::to_string(buffer_temp.top().count_rules);
				nT_stack += " is_expression: ";
				nT_stack += buffer_temp.top().is_expression ? "True" : "False";
				nT_stack += '\n';
				buffer_temp.pop();
			}


			int chain_size;
			int error;
			GRBALPHABET GRB_buffer = mfst.buffer.top();
			std::string selected_rule;
			MFST::Results res = mfst.step(data.error, chain_size,selected_rule);

			*log << "\n----------------------------------------------------------------------\n";
			*log << selected_rule<< "\t\t" << stack_str << '\n';
			*log << nT_stack<<'\n';

			int error_count = 0;
			switch (res)
			{
			case MFST::Results::OK_RESUULT:
				
			{
				//if (table.table[data.index].IT_index != -1) {
				//	if (ID::getEntry(id_table, table.table[data.index].IT_index).id_type == IDType::Type::Func) {
				//		last_func_id = &ID::getEntry(id_table, table.table[data.index].IT_index);
				//	}
				//}

				while (NT_node_struct.top().count_rules <= 0) {
					bool buffer = NT_node_struct.top().is_expression;
					AST::node* buffer_node = NT_node_struct.top().curent_node;
					NT_node_struct.pop();


					if (buffer == true && NT_node_struct.top().is_expression == false) {
						//тут строим польскую запись
						data.expressions[buffer_node].end = data.index-1;
						

						std::cout << "build poland" << std::endl;
						std::cout << data.expressions[buffer_node].begin << " - " << data.expressions[buffer_node].end << std::endl;
					}
				}

				if (!NT_node_struct.top().is_expression) {
					AST::node* new_node = create_new_leaf(table.table[data.index]);

					new_node->index = data.index;

					NT_node_struct.top().curent_node->links.push_back(new_node);
				}

				NT_node_struct.top().count_rules--;
				if (NT_node_struct.top().count_rules <= 0) {
					bool buffer = NT_node_struct.top().is_expression;
					AST::node* buffer_node = NT_node_struct.top().curent_node;
					NT_node_struct.pop();


					if (buffer == true && NT_node_struct.top().is_expression == false) {
						//тут строим польскую запись
						data.expressions[buffer_node].end = data.index;


						std::cout << "build poland" << std::endl;
						std::cout << data.expressions[buffer_node].begin << " - " << data.expressions[buffer_node].end << std::endl;
					}
				}

				data.index++;
			}
				break;
			case MFST::Results::FRONG_SYMBOL:

				//откатываемся на предыдушее сохранение 

				error_count++;

				if (chain_size<=0)  throw Error::get_error_in(data.error, data.line, data.index);

				try {


					mfst.get_last_save();
					mfst.chain_id++;

					
					AST::node* buffer = NT_node_struct.top().curent_node;

					data = info_saves.top();
					NT_node_struct = data.NT_node_stuct;
					info_saves.pop();

					//data.expressions.erase(buffer);
					AST::delete_node(NT_node_struct.top().curent_node,data.last_node);
					if(data.expressions.find(data.last_node)!=data.expressions.end())
					{
						data.expressions.erase(data.last_node);
					}

				}
				catch(...) {
					throw Error::get_error_in(data.error, data.line, data.index);
				}

				break;
			case MFST::Results::FIND_RULE:
			{

				data.NT_node_stuct = NT_node_struct;

				if (!NT_node_struct.empty()) {
					NT_node_struct.top().count_rules--;
				}

				if (chain_size >= 0) {
					AST::node* new_node = create_new_NT(GRB_buffer);


					if (!NT_node_struct.empty()) {

						data.last_node = new_node;

						NT_node_struct.top().curent_node->links.push_back(new_node);
						NT_node_struct.push(AST_node_info(new_node, chain_size, NT_node_struct.top().is_expression));
					}
					else {
						tree.root = new_node;
						NT_node_struct.push(AST_node_info(new_node, chain_size));
					}

					if (GRB_buffer == NS("E")) {
						if (NT_node_struct.top().is_expression == false) {
							AST_node_info temp = NT_node_struct.top();
							NT_node_struct.pop();

							data.expressions[new_node].prerent_node = NT_node_struct.top().curent_node;
							NT_node_struct.push(temp);

							data.expressions[new_node].begin = data.index;
						}

						NT_node_struct.top().is_expression = true;
					}
				}

				//Просто продолжаем
				info_saves.push(data);

			}	
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

				is_active = false;
				break;

			case MFST::Results::FILE_EMPTY:

				return tree;
				break;
			}


		}


		*log << "\nBEGINIG POLISH ENTRY\n";
		for (auto& elem : data.expressions) {

			*log << "expresion:  ";
			std::list<LT::Entry> temp;
			for (int i = elem.second.begin; i <= elem.second.end; i++) {
				*log << table.table[i].lexema;
				temp.push_back(table.table[i]);
			}
			

			AST::node** ref = AST::get_node_ref(elem.second.prerent_node, elem.first);
			AST::delete_node(elem.first);
			*log << " poland: ";
			*ref = POL::build_tree(temp, id_table,log);

			*log << '\n';
		}


		return tree;
	}

}
