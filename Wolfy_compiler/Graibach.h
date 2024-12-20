#pragma once
#include<map>
#include"Rules.h"
#include"Error.h"

#define GENERAl RULE::GRB::general_rules
#define MAIN RULE::GRB::main_rules

typedef short GRBALPHABET;  ///<0 - ���������� >0 - ��������

using namespace RULE::GRB;

namespace GRB {
	

	struct Greibach {

		GRBALPHABET Start = 0;					//���������� ������
		GRBALPHABET end = 0;					//��� �����
		std::map<GRBALPHABET, Rule> rules;



		static  bool isT(GRBALPHABET ch);
		static bool isN(GRBALPHABET ch);

		Rule getRule(GRBALPHABET N);
		Rule::Chain getChain(GRBALPHABET N,int rulle_index);
		Rule::Chain getChain(GRBALPHABET N, GRBALPHABET first_T, int offset);
		Rule::Chain getChain_firstN(GRBALPHABET N, int offset);
		Rule::Chain getChain_empty(GRBALPHABET N);
	};

	void get_GRB(Greibach& GRB, GRBALPHABET start, GRBALPHABET stack_end, std::list<Rule> rules);
}