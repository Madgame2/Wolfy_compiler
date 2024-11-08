#pragma once
#include<stack>
#include"Lexem_table.h"
#include"Graibach.h"

typedef short GRBALPHABET;  ///<0 - Нетерминал >0 - терминал

namespace MFST {

	enum class Results {
		OK_RESUULT,
		FRONG_SYMBOL,
		NO_RULE,
		FATAL_ERRROR
	};

	struct MFST {
		GRBALPHABET* lenta = nullptr;
		std::stack<GRBALPHABET> buffer;
		unsigned int lenta_position = 0;
		unsigned int lenta_size = 0;
		unsigned int chain_id = 0;

		GRB::Greibach grb;

		Results step();

		void make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer);
		void make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer, unsigned int n_chain);
	};

	void create_MFST(MFST& msft, LT::Lexem_table table, GRB::Greibach grb);


	namespace save {
		struct saves {
			unsigned int lenta_position=0;
			unsigned int n_rule_cnain=0;
			std::stack<GRBALPHABET> buffer;
		};

		std::stack<saves> saves_stack;
	}
}