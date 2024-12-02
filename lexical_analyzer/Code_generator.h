#pragma once
#include<map>
#include<list>
#include<string>
#include"Rules.h"
#include"AST.h"
#include"Identification_table.h"
#include"Litetral_table.h"

namespace CODE {

	void generate_code(AST::program_struct tree,ID::ID_table id_table, Lit_table::Literal_table lit_table);

	struct templates {
		std::map<std::string, std::string> template_asm;

		templates(std::list<RULE::CODE::templates> prefabs) {
			for (auto& elem : prefabs) {
				template_asm[elem.key] = elem.path_to_tamplate;
			}
		}
	};

}