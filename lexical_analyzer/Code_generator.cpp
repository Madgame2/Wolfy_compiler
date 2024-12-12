#include"Code_generator.h"
#include<iostream>

#define HEX 'f'
#define BIN 'b'
#define OCT 'o'

namespace CODE {

	int func_id = -1;
	bool func_params = false;

	std::string wstring_to_string(const std::wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstr);
	}
	std::string readFile(const std::string& filename) {

		std::ifstream file(filename);


		if (!file.is_open()) {
			std::cerr << "Не удалось открыть файл!" << std::endl;
			return "";
		}

		std::string content;
		std::string line;

		while (std::getline(file, line)) {
			content += line + "\n";
		}


		file.close();
		return content;
	}

	void delite_tag(std::string& source_code, std::string tag, size_t pos) {
		source_code.erase(pos, tag.size());
	}
	void write_by_template(std::string& source_code, RULE::CODE::templates prefab, bool delete_tag) {
		std::string tag;
		int  offstet = 0;


		bool find = false;
		for (int i = 0; i < prefab.tags.size(); i++) {
			if (source_code.find(prefab.tags[i]) != std::string::npos) {
				tag = prefab.tags[i];
				offstet = i;
				find = true;
				break;
			}
		}

		if (!find) return;

		size_t pos = source_code.find(tag);

		if (delete_tag)
			delite_tag(source_code, tag, pos);

		std::string code = readFile("x86_templates\\" + prefab.path_to_tamplate);

		source_code.insert(pos, code);

		//std::cout << source_code<<std::endl;
	}
	void write_var_to_asm(std::string& source_code, AST::node* curent, ID::ID_table table) {
		int table_id = curent->table_id;
		ID::Entry varr = ID::getEntry(table, table_id);


		size_t type_pos = source_code.find("<data_type>");

		if (type_pos != std::string::npos) {
			delite_tag(source_code, "<data_type>", type_pos);
			source_code.insert(type_pos, RULE::CODE::DataType_AsmCode[varr.d_type].asm_data_type);
		}

		size_t var_name_pos = source_code.find("<var>");
		if (var_name_pos != std::string::npos) {
			delite_tag(source_code, "<var>", var_name_pos);
			std::wstring area = func_params ? varr.area + std::to_wstring(func_id) : varr.area;
			source_code.insert(var_name_pos, wstring_to_string(area  + varr.name));
		}

		size_t value_pos = source_code.find("<value>");
		if (curent->is_expression && value_pos != std::string::npos) {
			delite_tag(source_code, "<value>", value_pos);
			source_code.insert(value_pos, "0");
		}
	}
	void varr_defoult_value(std::string& source_code) {
		if (source_code.find("<value>") != std::string::npos) {
			size_t poss = source_code.find("<value>");

			delite_tag(source_code, "<value>", poss);
			source_code.insert(poss, "0");
		}
	}
	void insert_value(std::string& source_code, AST::node* curent, bool negative, ID::ID_table id_table, Lit_table::Literal_table table) {

		if (curent->type == AST::node_type::Lit) {

			int table_id = curent->table_id;
			Lit_table::Element lit = Lit_table::find(table, table_id);

			size_t pos = source_code.find("<value>");

			if (lit.d_type == DataType::Type::String) {
				delite_tag(source_code, "<value>", pos);

				source_code.insert(pos, "\"" + wstring_to_string(lit.value) + "\", 0");

			}
			else {

				char notation = ' ';

				switch (lit.my_notation)
				{
				case notations::notation::Bin:
					notation = BIN;
					break;
				case notations::notation::oct:
					notation = OCT;

					break;
				case notations::notation::Hex:
					notation = HEX;

					break;
				}


				if (pos != std::string::npos) {
					delite_tag(source_code, "<value>", pos);
					if (negative) {
						source_code.insert(pos, '-' + wstring_to_string(lit.value) + notation);
					}
					else
					{
						source_code.insert(pos, wstring_to_string(lit.value) + notation);
					}
				}
			}
		}
		else if (curent->type == AST::node_type::ID) {
			int table_id = curent->table_id;
			ID::Entry id = ID::getEntry(id_table, table_id);

			size_t pos = source_code.find("<value>");

			if (pos != std::string::npos) {
				delite_tag(source_code, "<value>", pos);

				std::wstring area = func_params ? id.area + std::to_wstring(func_id) : id.area;
				if (negative) {
					//негатива добавит 

					source_code.insert(pos, wstring_to_string(area + id.name));
				}
				else {
					source_code.insert(pos, wstring_to_string(area + id.name));

				}
			}
		}
	}
	void insert_expression(std::string& sourece_code, AST::node* curent, ID::ID_table id_table, Lit_table::Literal_table lit_table) {
		int table_id = curent->table_id;
		DataType::Type data_type;
		if (curent->type == AST::node_type::ID) {
			ID::Entry var = ID::getEntry(id_table, table_id);
			data_type = var.d_type;
		}
		else if (curent->type == AST::node_type::Lit) {
			Lit_table::Element lit = Lit_table::find(lit_table, table_id);
			data_type = lit.d_type;
		}


		size_t pos = sourece_code.find("<register>");

		if (pos != std::string::npos) {
			delite_tag(sourece_code, "<register>", pos);
			sourece_code.insert(pos, RULE::CODE::DataType_AsmCode[data_type].asm_register);
		}
	}
	void insert_operationn(std::string& sourece_code, AST::node* curent, templates prefabs) {

		if (RULE::CODE::operatin_AsmCode.find(curent->symbol) != RULE::CODE::operatin_AsmCode.end()) {
			RULE::CODE::comand com = RULE::CODE::operatin_AsmCode[curent->symbol];

			write_by_template(sourece_code, prefabs.template_asm[com], true);
		}
	}

	void insert_function_names(std::string& sourece_code, std::string func_name) {
		size_t pos = sourece_code.find("<func_name>");


		while (pos != std::string::npos) {
			delite_tag(sourece_code, "<func_name>", pos);
			sourece_code.insert(pos, func_name);

			pos = sourece_code.find("<func_name>");
		}
	}

	void insert_params_to_proto(std::string& sourece_code, AST::node* curent, ID::ID_table id_table, int argv_count) {
		int id = curent->table_id;
		ID::Entry param = ID::getEntry(id_table, id);

		size_t pos = sourece_code.find("<data_type_proto>");

		if (pos != std::string::npos) {

			if (argv_count == 0) {
				sourece_code.insert(pos, ": " + RULE::CODE::DataType_AsmCode[param.d_type].asm_data_type);
			}
			else {
				sourece_code.insert(pos, ", :" + RULE::CODE::DataType_AsmCode[param.d_type].asm_data_type);
			}
		}

	}

	void insert_cleaner_local_vars(std::string& source_code, templates prefabs, std::vector<std::wstring> names) {
		for (auto& elem : names) {
			write_by_template(source_code, prefabs.template_asm[RULE::CODE::comand::Func_clear], false);

			size_t pos = source_code.find("<var>");

			if (pos != std::string::npos) {
				delite_tag(source_code, "<var>", pos);
				source_code.insert(pos, wstring_to_string(elem));
			}
		}
	}

	void insert_clear_all_local_vars(std::string& source_code, templates prefabs, std::stack<std::vector<std::wstring>> local_vars) {
		while (!local_vars.empty()) {

			insert_cleaner_local_vars(source_code, prefabs, local_vars.top());

			local_vars.pop();
		}
	}

	void insert_stack_size(std::string& source_code, int size) {
		size_t pos = source_code.find("<size>");

		if (pos != std::string::npos) {
			delite_tag(source_code, "<size>", pos);
			source_code.insert(pos, std::to_string(size));
		}
	}
	void insert_if_blocks_id(std::string& asm_code, int id) {
		size_t pos = asm_code.find("<num>");

		if (pos != std::string::npos) {
			delite_tag(asm_code, "<num>", pos);
			asm_code.insert(pos, std::to_string(id));
		}

		pos = asm_code.find("<num>");

		if (pos != std::string::npos) {
			delite_tag(asm_code, "<num>", pos);
			asm_code.insert(pos, std::to_string(id));
		}
	}

	void inseert_while_blocks_id(std::string& asm_code, int id) {
		size_t pos;

		for (int i = 0; i < 4; i++) {
			pos = asm_code.find("<num>");

			delite_tag(asm_code, "<num>", pos);
			asm_code.insert(pos, std::to_string(id));
		}
	}

	void insert_stack_offset(std::string& asm_code, int& offset) {
		size_t pos = asm_code.find("<num>");

		if (pos != std::string::npos) {
			delite_tag(asm_code, "<num>", pos);
			asm_code.insert(pos, std::to_string(offset));
			offset += 4;
		}
	}

	void clear_all_tags(std::string& asm_code) {
		size_t tag_begin;
		size_t tag_end;

		while ((tag_begin = asm_code.find("<"))!=std::string::npos ) {
			tag_end = asm_code.find(">");	
			asm_code.erase(tag_begin, tag_end - tag_begin + 1);
		}
	}

	void insert_register(std::string& asm_code, AST::node* curent) {

		size_t poss = asm_code.find("<register>");

		if (poss != std::string::npos) {
			delite_tag(asm_code, "<register>", poss);
			

			std::string reg = RULE::CODE::DataType_AsmCode[curent->expresion_type].asm_register;

			asm_code.insert(poss, reg);
		}
	}



	void generate_code(std::wstring name, AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table, std::list<semantic::data::global_elem> global_functions)
	{
		func_id = -1;

		std::ofstream out_file(name + L".asm");

		if (!out_file.is_open()) {
			out_file.close();
			throw Error::get_error(112);
		}


		tree.Reset();

		templates prefabs(RULE::CODE::prefabs);
		std::string asm_code = "<start>";

		AST::node* curent = tree.DFS.Step();
		AST::node* buffer = nullptr;
		if (strcmp(curent->symbol, "S") == 0) {
			write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::PROGRAM_BEGIN], true);

			if (name == L"MAIN") {
				write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::MAIN_INIT], true);

			}
		}

		for (auto& elem : global_functions) {
			write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Global_init], false);

			std::string  name = wstring_to_string(elem.function.function_name + std::to_wstring(*elem.function.func_unick_id));
			insert_function_names(asm_code, name);
		}

		bool varr_init = false;
		bool befor_minus = false;
		bool is_functon_params = false;
		bool console_func = false;
		bool is_if_expreson = false;
		bool is_while = false;
		int argv_count = 0;
		int LITERAL_count = 0;
		int funct_arg_size = 0;
		int if_block_id = 0;
		int while_id = 0;
		int regisnter_id = 0;
		int stack_offset = 8;
		int params_count = 0;
		std::stack<std::vector<std::wstring>> local_vars;
		while (true)
		{

			curent = tree.DFS.Step();


			if (curent == nullptr) {

				break;
			}

			if (strcmp(curent->symbol, ";") == 0) {
				varr_defoult_value(asm_code);
				if (varr_init) {
					varr_init = false;

					buffer = nullptr;
					continue;
				}
				console_func = false;
			}
			else if (strcmp(curent->symbol, ",") == 0) {
				if (varr_init) {
					varr_defoult_value(asm_code);

					buffer = nullptr;
					continue;
				}
			}
			else if (strcmp(curent->symbol, ")") == 0) {
				if (varr_init) {
					varr_defoult_value(asm_code);

					buffer = nullptr;
				}
				if (is_while) {


					size_t pos = asm_code.find("<expresion>");

					if (pos != std::string::npos) {
						delite_tag(asm_code, "<expresoin>", pos);
					}
					is_while = false;
				}
				if (is_functon_params) {

					size_t pos = asm_code.find("<data_type_proto>");

					if (pos != std::string::npos) {
						delite_tag(asm_code, "<data_type_proto>", pos);
					}

					stack_offset = 4;

					pos = asm_code.find("<params>");

					if (pos != std::string::npos) {
						delite_tag(asm_code, "<params>", pos);
					}

					varr_init = false;
					is_functon_params = false;
				}
				if (is_if_expreson) {
					is_if_expreson = false;
				}

			}
			else if (strcmp(curent->symbol, "}") == 0) {
				size_t poss = asm_code.find("<block>");

				if (poss != std::string::npos) {
					delite_tag(asm_code, "<block>", poss);
				}
				else {
					poss = asm_code.find("<func_code>");

					if (poss != std::string::npos) {
						delite_tag(asm_code, "<func_code>", poss);
						func_params = false;

					}
				}
			}
			else if (strcmp(curent->symbol, "s") == 0) {
				continue;
			}

			if (!curent->is_expression && !varr_init && !is_if_expreson && !is_while) {
				size_t pos = asm_code.find("<expresion>");

				if (pos != std::string::npos) {
					delite_tag(asm_code, "<expresoin>", pos);
				}
			}
			if (!curent->is_param && is_functon_params&&!console_func) {
				//is_functon_params = false;

				size_t pos = asm_code.find("<arg>");

				if (pos != std::string::npos) {
					delite_tag(asm_code, "<arg>", pos);
				}
			}


			AST::symbol_type node_type = curent->symbol_type;
			AST::node_type type = curent->type;

			switch (node_type)
			{
			case AST::symbol_type::Terminal:
			{
				switch (type)
				{
				case AST::node_type::ID:

				{
					int table_id = curent->table_id;
					ID::Entry id = ID::getEntry(id_table, table_id);
					IDType::Type id_type = id.id_type;


					switch (id_type)
					{
					case IDType::Type::Var:

					{
						if (console_func) {
							DataType::Type type = id.d_type;

							size_t pos = asm_code.find("<func_name>");

							if (pos != std::string::npos) {

								delite_tag(asm_code, "<func_name>", pos);
								switch (type)
								{
								case DataType::Type::Int:
									asm_code.insert(pos, "print_int");

									break;
								case DataType::Type::Float:
									break;
								case DataType::Type::String:
									asm_code.insert(pos, "print_string");
									break;
								case DataType::Type::Short:
									break;
								case DataType::Type::None:
									break;
								default:
									break;
								}
							}
							console_func = false;

							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_push_arg], true);
							insert_value(asm_code, curent, befor_minus, id_table, lit_table);

							pos = asm_code.find("<arg>");
							delite_tag(asm_code, "<arg>", pos);
						}


						if (buffer != nullptr && strcmp(buffer->symbol, "t") == 0) {
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::VAR_delclarete], false);
							write_var_to_asm(asm_code, curent, id_table);

							buffer = curent;
							varr_init = true;

							if (!local_vars.empty()) {
								local_vars.top().push_back(id.area + id.name);
							}

							if (id.d_type == DataType::Type::String) {
								varr_defoult_value(asm_code);
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_init], false);

								write_var_to_asm(asm_code, curent, id_table);
								continue;
							}



							//МЫСЛИ: тут если вырожение добавить в секцию мейна вырожение 
							if (curent->is_expression) {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_init], false);

								write_var_to_asm(asm_code, curent, id_table);
								buffer = curent;
							}
						}
						else {
							if (!curent->is_expression && !curent->is_param) {
								insert_value(asm_code, curent, befor_minus, id_table, lit_table);
							}
							else {
								if (curent->is_param) {
									write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_push_arg], true);
									insert_value(asm_code, curent, befor_minus, id_table, lit_table);

								}
								else if (is_while) {
									write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_push], true);
									insert_value(asm_code, curent, befor_minus, id_table, lit_table);
								}
								else if (curent->is_expression) {
									write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_push], true);
									insert_value(asm_code, curent, befor_minus, id_table, lit_table);

									buffer = curent;
								}
								else {
									buffer = curent;
								}
							}
						}
					}


					break;
					case IDType::Type::Func: {
						ID::Entry func = ID::getEntry(id_table, curent->table_id);


						if (buffer&&strcmp(buffer->symbol, "f") == 0) {

							size_t pos = asm_code.find("<func_code>");
							if (pos != std::string::npos) {
								delite_tag(asm_code, "<func_code>", pos);
							}
							funct_arg_size = 0;

							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_init], false);
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_proto], false);

							insert_function_names(asm_code, wstring_to_string(func.name)+std::to_string(func.func_unick_id));

							local_vars.push(std::vector<std::wstring>());
							is_functon_params = true;
							func_id = func.func_unick_id;

							func_params = true;
						}
						else {
							if (!curent->is_param) {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_call], false);
								insert_function_names(asm_code, wstring_to_string(func.name) + std::to_string(func.func_unick_id));
								is_functon_params = true;
								params_count = curent->argv_count;
							}
							else {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_as_a_arg], true);
								insert_function_names(asm_code, wstring_to_string(func.name) + std::to_string(func.func_unick_id));
							}
						}
					}
										   break;
					case IDType::Type::Param:
					{
						is_functon_params = true;

						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::VAR_delclarete], false);
						write_var_to_asm(asm_code, curent, id_table);

						varr_init = true;

						//insert_params_to_proto(asm_code, curent, id_table, argv_count);
						argv_count++;

						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_take_arg], false);
						write_var_to_asm(asm_code, curent, id_table);
						insert_stack_offset(asm_code, stack_offset);

						switch (id.d_type)
						{
						case DataType::Type::Int:
							funct_arg_size += 4;
							break;
						case DataType::Type::Short:
							funct_arg_size += 2;
						case DataType::Type::Float:
							funct_arg_size += 4;
							break;
						case DataType::Type::String:
							funct_arg_size += 4;
							break;
						default:
							break;
						}

						if (!local_vars.empty()) {
							local_vars.top().push_back(id.area + id.name);
						}
					}
					break;
					case IDType::Type::None:

						buffer = curent;

						break;
					}

				}

				break;
				case AST::node_type::Lit:
				{
					Lit_table::Element lit = Lit_table::find(lit_table, curent->table_id);
					DataType::Type data_type = lit.d_type;


					switch (data_type)
					{
					case DataType::Type::String:
					{
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Const_declare], false);
						size_t name_pos = asm_code.find("<var>");
						delite_tag(asm_code, "<var>", name_pos);


						std::string l("L");
						l += (char)LITERAL_count + '0';

						asm_code.insert(name_pos, l);


						size_t type_pos = asm_code.find("<data_type>");
						delite_tag(asm_code, "<data_type>", type_pos);
						asm_code.insert(type_pos, "db");

						size_t value_pos = asm_code.find("<value>");
						delite_tag(asm_code, "<value>", value_pos);
						asm_code.insert(value_pos, "\"" + wstring_to_string(lit.value) + "\"," + " 0");
					}
					break;
					default:
						break;
					}

					if (console_func) {
						size_t pos = asm_code.find("<func_name>");

						if (pos != std::string::npos) {

							delite_tag(asm_code, "<func_name>", pos);
							switch (lit.d_type)
							{
							case DataType::Type::Int:
								asm_code.insert(pos, "print_int");

								break;
							case DataType::Type::Float:
								break;
							case DataType::Type::String:
								asm_code.insert(pos, "print_string");
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_push_arg], true);

								break;
							case DataType::Type::Short:
								break;
							case DataType::Type::None:
								break;
							default:
								break;
							}
						}
						console_func = false;
					}


					if (!curent->is_expression && !curent->is_param) {
						if (lit.d_type == DataType::Type::String) {
							size_t pos = asm_code.find("<value>");

							std::string l("L");
							l += (char)LITERAL_count + '0';

							if (pos != std::string::npos) {
								delite_tag(asm_code, "<value>", pos);
								asm_code.insert(pos, "offset " + l);
							}

							pos = asm_code.find("<arg>");
							if (pos != std::string::npos)
								delite_tag(asm_code, "<arg>", pos);
						}
						else {

							insert_value(asm_code, curent, befor_minus, id_table, lit_table);
						}
					}
					else if (curent->is_param) {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_push_arg], true);

						if (lit.d_type == DataType::Type::String) {
							size_t pos = asm_code.find("<value>");

							std::string l("L");
							l += (char)LITERAL_count + '0';

							if (pos != std::string::npos) {
								delite_tag(asm_code, "<value>", pos);
								asm_code.insert(pos, "offset " + l);
							}

							pos = asm_code.find("<arg>");
							if (pos != std::string::npos)
								delite_tag(asm_code, "<arg>", pos);
						}
						else {
							insert_value(asm_code, curent, befor_minus, id_table, lit_table);
						}
						params_count--;
					}
					else {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_push], true);
						insert_value(asm_code, curent, befor_minus, id_table, lit_table);
					}

				}
				break;
				case AST::node_type::notation:
					break;
				case AST::node_type::general:
				{
					if (strcmp(curent->symbol, "=") == 0 && !curent->is_double_operation)
					{
						if (buffer->type == AST::node_type::ID) {
							if (ID::getEntry(id_table, buffer->table_id).d_type == DataType::Type::String) {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_push], true);
								continue;
							}
						}

						if (!varr_init) {
							if (!buffer->is_expression) {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_VALUE], false);
							}
							else {
								write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Expression_init], false);
							}
							write_var_to_asm(asm_code, buffer, id_table);
						}
					}
					else if (strcmp(curent->symbol, "=") == 0 && curent->is_double_operation)
					{
						size_t pos = asm_code.find("<flag>");
						if (pos != std::string::npos) {
							delite_tag(asm_code, "<flag>", pos);
							asm_code.insert(pos, "jne");
						}
					}
					else if (strcmp(curent->symbol, "-") == 0) {
						if (curent->links.size() > 1) {
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_EXPRESSION], true);
							insert_register(asm_code, curent);
							insert_operationn(asm_code, curent, prefabs);
						}
						else {
							befor_minus = true;
							continue;
						}
					}
					else if (strcmp(curent->symbol, "r") == 0) {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_ret], false);
						insert_clear_all_local_vars(asm_code, prefabs, local_vars);

						insert_stack_size(asm_code, funct_arg_size);

						size_t pos = asm_code.find("<templ_var>");
						if (pos != std::string::npos) {
							delite_tag(asm_code, "<templ_var>", pos);
						}
					}
					else if (strcmp(curent->symbol, "c") == 0) {
						//write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_call], false);
						console_func = true;
					}
					else if (strcmp(curent->symbol, "<") == 0) {
						if (console_func) {
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_call], false);
							//console_func = true;
						}
						else if (curent->is_double_operation) {
							size_t pos = asm_code.find("<flag>");
							if (pos != std::string::npos) {
								delite_tag(asm_code, "<flag>", pos);
								asm_code.insert(pos, "jge");
							}
						}
						else {
							size_t pos = asm_code.find("<flag>");
							if (pos != std::string::npos) {
								delite_tag(asm_code, "<flag>", pos);
								asm_code.insert(pos, "jg");
							}

						}
					}
					else if (strcmp(curent->symbol, "}") == 0) {

						if (!local_vars.empty()) local_vars.pop();

						size_t pos = asm_code.find("<block>");

						if (pos != std::string::npos) {
							delite_tag(asm_code, "<block>", pos);
						}

					}
					else if (strcmp(curent->symbol, "n") == 0) {
						size_t pos = asm_code.find("<func_name>");

						if (pos != std::string::npos) {
							asm_code.insert(pos, "print_newline");
							//write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Func_push_arg], true);
						}
					}
					else if (strcmp(curent->symbol, "?") == 0)
					{
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::if_init], false);
						insert_if_blocks_id(asm_code, if_block_id);
						local_vars.push(std::vector<std::wstring>());

						is_if_expreson = true;
						if_block_id++;
					}

					else if (strcmp(curent->symbol, "w") == 0) {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::while_init], false);
						inseert_while_blocks_id(asm_code, while_id);
						local_vars.push(std::vector<std::wstring>());

						is_while = true;
					}
					else if (curent->is_expression) {


						//ТУТ вырожение для while
						if (is_while) {


							//write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::pop_value], true);
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::while_expresion], true);
							insert_operationn(asm_code, curent, prefabs);

						}
						else if (!curent->is_double_operation) {
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_EXPRESSION], true);
							insert_register(asm_code, curent);
							insert_operationn(asm_code, curent, prefabs);
						}
					}
					else {
						buffer = curent;
					}

					std::cout << "------------------------------------------------------------------------------" << std::endl;
					std::cout << asm_code << std::endl;
					befor_minus = false;
				}

				break;
				}
			case AST::symbol_type::NonTerminal:

				continue;
				break;
			}

			}
		}
		clear_all_tags(asm_code);

		out_file << asm_code;

		out_file.close();
	}
}