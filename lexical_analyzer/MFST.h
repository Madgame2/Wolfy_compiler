#pragma once
#include<stack>
#include"Lexem_table.h"
#include"Graibach.h"

typedef short GRBALPHABET;  ///<0 - Íåòåðìèíàë >0 - òåðìèíàë

namespace MFST {

	enum class Results {
		OK_RESUULT,
		FRONG_SYMBOL,
		FIND_RULE,
		NO_RULE,
		FATAL_ERRROR,
		LENTA_END_GOOD,
		FILE_EMPTY
	};

	struct MFST {
		GRBALPHABET* lenta = nullptr;
		std::stack<GRBALPHABET> buffer;
		unsigned int lenta_position = 0;
		unsigned int lenta_size = 0;
		unsigned int chain_id = 0;

		GRB::Greibach grb;

		Results step(int& error_code, int& chain_size);

		void make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer);
		void make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer, unsigned int n_chain);

		void get_last_save();
	};

	void create_MFST(MFST& msft, LT::Lexem_table table, GRB::Greibach grb);


	namespace save {
		struct saves {
			unsigned int lenta_position=0;
			unsigned int n_rule_cnain=0;
			std::stack<GRBALPHABET> buffer;
		};

		extern std::stack<saves> saves_stack;
	}
}