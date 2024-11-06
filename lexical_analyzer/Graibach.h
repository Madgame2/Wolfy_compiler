#pragma once
#include<map>
#include"Rules.h"
#include"Error.h"

#define GENERAl RULE::GRB::general_rules

typedef short GRBALPHABET;  ///<0 - Нетерминал >0 - терминал

using namespace RULE::GRB;

namespace GRB {
	

	struct Greibach {

		GRBALPHABET Start;					//Стартоввый символ
		GRBALPHABET end;					//Дно стека
		std::map<GRBALPHABET, Rule> rules;



		static  bool isT(GRBALPHABET ch);
		static bool isN(GRBALPHABET ch);

		Rule::Chain getChain(GRBALPHABET N,int rulle_index);
		Rule::Chain getChain(GRBALPHABET N, GRBALPHABET first_T, int offset);
	};

	void get_GRB(Greibach& GRB, GRBALPHABET start, GRBALPHABET stack_end, std::list<Rule> rules);
}