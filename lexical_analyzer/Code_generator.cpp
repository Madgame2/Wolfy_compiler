#include"Code_generator.h"
#include<iostream>

namespace CODE {

	
	std::string wstring_to_string(const std::wstring& wstr) {
		// Используем стандартный конвертер
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


		for (int i = 0; i < prefab.tags.size(); i++) {
			if (source_code.find(prefab.tags[i]) != std::string::npos) {
				tag = prefab.tags[i];
				offstet = i;
				break;
			}
		}


		size_t pos = source_code.find(tag);

		if(delete_tag)
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
			source_code.insert(var_name_pos, wstring_to_string(varr.name));
		}

		size_t value_pos = source_code.find("<value>");
		if (curent->is_expression && value_pos != std::string::npos) {
			delite_tag(source_code, "<value>", value_pos);
			source_code.insert(value_pos,"0");
		}
	}
	void varr_defoult_value(std::string& source_code) {
		if (source_code.find("<value>") != std::string::npos) {
			size_t poss = source_code.find("<value>");

			delite_tag(source_code, "<value>", poss);
			source_code.insert(poss,"0");
		}
	}
	void insert_value(std::string& source_code, AST::node* curent, bool negative , ID::ID_table id_table,Lit_table::Literal_table table) {

		if (curent->type == AST::node_type::Lit) {

			int table_id = curent->table_id;
			Lit_table::Element lit = Lit_table::find(table, table_id);

			size_t pos = source_code.find("<value>");

			if (pos != std::string::npos) {
				delite_tag(source_code, "<value>", pos);
				if (negative) {
					source_code.insert(pos, '-' + wstring_to_string(lit.value));
				}
				else
				{
					source_code.insert(pos, wstring_to_string(lit.value));
				}
			}
		}
		else if (curent->type == AST::node_type::ID) {
			int table_id = curent->table_id;
			ID::Entry id = ID::getEntry(id_table, table_id);

			size_t pos = source_code.find("<value>");

			if (pos != std::string::npos) {
				delite_tag(source_code, "<value>", pos);
				if (negative) {
					source_code.insert(pos, '-' + wstring_to_string(id.name));
				}
				else {
					source_code.insert(pos, wstring_to_string(id.name));

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

	void generate_code(std::wstring name, AST::program_struct tree, ID::ID_table id_table, Lit_table::Literal_table lit_table)
	{

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

		bool varr_init = false;
		bool befor_minus = false;
		while (true)
		{

			curent = tree.DFS.Step();
			

			if (curent == nullptr) {

				break;
			}

			if (strcmp(curent->symbol, ";") == 0) {
				if (varr_init) {
					varr_init = false;
					varr_defoult_value(asm_code);

					buffer = nullptr;
					continue;
				}
			}

			if (curent->type == AST::node_type::ID) {
				int table_id = curent->table_id;
				ID::Entry id = ID::getEntry(id_table, table_id);

				switch (id.id_type)
				{
				case IDType::Type::Var: {

					if (buffer!=nullptr&&strcmp(buffer->symbol, "t") == 0) {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::VAR_delclarete], false);

						write_var_to_asm(asm_code, curent, id_table);

						varr_init = true;


						//МЫСЛИ: тут если вырожение добавить в секцию мейна вырожение 


						if (curent->is_expression) {
							write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_EXPRESSION], false);

							insert_expression(asm_code, curent, id_table, lit_table);

						}
					}
					else {
						if (!curent->is_expression) {
							insert_value(asm_code, curent, befor_minus, id_table, lit_table);
						}
						else {
							//write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_EXPRESSION], false);

							//insert_expression(asm_code, curent, id_table, lit_table);
						}
					}
				}
				case IDType::Type::Param: {

				}
				case IDType::Type::Func: {

				}
				case IDType::Type::None: {
					buffer = curent;
				}
				}

			}
			else if (curent->type == AST::node_type::Lit) {
				insert_value(asm_code, curent,befor_minus,id_table, lit_table);
			}
			else if(strcmp(curent->symbol,"=")==0&& !curent->is_double_operation)
			{

				if (!varr_init) {
					if (!buffer->is_expression) {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_VALUE], false);
					}
					else {
						write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::Assign_to_var], false);
						insert_expression(asm_code, buffer, id_table,lit_table);
					}
					write_var_to_asm(asm_code, buffer, id_table);
				}
			}
			else if (strcmp(curent->symbol, "-") == 0) {
				befor_minus = true;
				continue;
			}
			else if (curent->is_expression) {
				write_by_template(asm_code, prefabs.template_asm[RULE::CODE::comand::ASSIGN_EXPRESSION], true);
			}
			else {
				buffer = curent;
			}


			std::cout << asm_code << std::endl;
			befor_minus = false;
		}

		out_file.close();
	}
}