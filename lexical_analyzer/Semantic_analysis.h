#pragma once
#include<string>
#include<vector>
#include"AST.h"
#include"Identification_table.h"
#include"Litetral_table.h"
#include"Control_flow.h"

namespace semantic {
	namespace data {
		struct var {
			std::wstring name;
			DataType::Type d_type = DataType::Type::None;
			IDType::Type id_type = IDType::Type::None;

			bool is_array = false;

			std::list<extra::Type> extras;
		};

		struct Func_sign {
			std::wstring function_name;
			DataType::Type returable_type = DataType::Type::None;
			DataType::Type* ref_returnable_type = &returable_type;

			std::vector<DataType::Type*> params;

			bool operator ==(Func_sign referens) {
				if (this->function_name != referens.function_name) {
					return false;
				}
				if (this->params.size() != referens.params.size()) {
					return false;
				}
				for (int i = 0; i < this->params.size(); i++) {
					DataType::Type type1 = *this->params[i];
					DataType::Type type2 = *referens.params[i];

					if (type2 != type1) {
						return false;
					}
				}
				return true;
			}
			bool operator !=(Func_sign referens) {
				if (this->function_name != referens.function_name) {
					return false;
				}
				if (this->params.size() != referens.params.size()) {
					return false;
				}
				for (int i = 0; i < this->params.size(); i++) {
					if (this->params[i] != referens.params[i]) {
						return false;
					}
				}
				return true;
			}
		};
		struct global_elem {
			bool is_relisated = false;
			Func_sign function;
		};
	}

	namespace scope {

		struct node {
			node* parent = nullptr;
			std::vector<node*> childres;
			std::wstring node_name;

			struct info
			{
				std::list<data::var> vareiables;
				std::list<data::Func_sign> functions;
			} objects;
		};

		struct scope {
			node* root = nullptr;

			std::stack<node* > last_scope;

			void add_new_scope(std::wstring name);
			void add_new_var(ID::Entry var);

			void add_new_functoin(ID::Entry func);
			void add_param_to_last_func(ID::Entry param, data::Func_sign* function);

			void pop_scope();

			bool has_this_var(AST::node* name);
			bool has_this_var(std::wstring name);
			bool has_this_func_sign(data::Func_sign* last_func, std::list<semantic::data::global_elem>& global, AST::node* curent);

			data::var getvar(std::wstring name);
			data::Func_sign get_last_Func();
			node& get_area(data::var);
		};

	}

	void Parse(AST::program_struct tree, std::list<semantic::data::global_elem>& global, ID::ID_table& id_table, Lit_table::Literal_table& lit_table);
	std::list<data::global_elem> Parse_Global(AST::program_struct tree, ID::ID_table& id_table);
}