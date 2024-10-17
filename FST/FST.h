#pragma once
#include<string>
#include<vector>
#include<algorithm>
#include <cstdarg>
#include <unordered_set>

#define ZERO_TRANSITION 0x00

namespace fst {

	struct RELATION
	{
		short nex_node = 0;
		wchar_t symbol = 0x00;

		RELATION(wchar_t symbol=0x00, short next_node = 0);
	};

	struct NODE
	{
		short count_relations = 0;
		RELATION* relations=nullptr;

		NODE();
		NODE(short count_ralations, RELATION rel, ...);
	};

	struct FST {
		short nstates = 0;
		NODE* nodes = nullptr;

		FST(short states_count, NODE node, ...);
	};


	bool execute(std::wstring string, FST fst);
}