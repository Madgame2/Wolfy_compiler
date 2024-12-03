#pragma once
#include<map>
#include<list>
#include<string>
#include<fstream>
#include <sstream>
#include <codecvt>
#include <stdexcept>
#include"Rules.h"
#include"AST.h"
#include"Identification_table.h"
#include"Litetral_table.h"

namespace CODE {

	void generate_code(std::wstring name, AST::program_struct tree,ID::ID_table id_table, Lit_table::Literal_table lit_table);

	struct templates {
		std::map<RULE::CODE::comand, RULE::CODE::templates> template_asm;

		templates(std::list<RULE::CODE::templates> prefabs) {
			for (auto& elem : prefabs) {
				template_asm[elem.key] = elem;
			}
		}
	};

}