#include"FST.h"



fst::fst::fst(short states_count, NODE node, ...)
{
	this->nstates = states_count;
	this->nodes = new NODE[nstates];

	NODE* p = &node;
	for (int i = 0; i < nstates; i++) {
		nodes[i] = *p;
		p++;
	}
}

fst::NODE::NODE()
{
}

fst::NODE::NODE(short count_ralations, RELATION rel, ...)
{
	this->count_relations = count_ralations;
	this->relations = new RELATION[count_ralations];

	RELATION* p = &rel;

	for (int i = 0; i < count_ralations; i++) {
		relations[i] = *p;
		p++;
	}
}

fst::RELATION::RELATION(short next_node, wchar_t symbol)
{
	this->nex_node = next_node;
	this->symbol = symbol;
}


void build_ailable_statests(unsigned short& size, short* arr, fst::fst fst ) {

	//добавлем доступные NODE
	std::vector<short> available_states;
	for (int i = 0; i < size; i++) {
		available_states.push_back(arr[i]);
	}

	//ѕробегаемс€ по каждой доступной NODE
	for (int i = 0; i < available_states.size(); i++) {
		fst::NODE curent = fst.nodes[available_states[i]];

		for (int j = 0; j < curent.count_relations; j++) {
			fst::RELATION rel = available_states[j];

			if (rel.symbol == ZERO_TRANSITION) {

				if (std::find(available_states.begin(), available_states.end(), rel.nex_node) == available_states.end()) {
					available_states.push_back(rel.nex_node);
				}

			}
		}
	}

	size = available_states.size();
	delete[] arr;
	arr = new short[size];

	for (int i = 0; i < size; i++) {
		arr[i] = available_states[i];
	}
}

bool fst::execute(std::wstring string, fst fst)
{
	short* curent = new short[fst.nstates];
	short* next = new short[fst.nstates];


	//ќпредел€ем начальное состо€ние 
	short* available_states = new short;
	*available_states = 0;
	unsigned short available_statest_count = 1;

	for (int i = 0; i < string.size();) {

		build_ailable_statests(available_statest_count, available_states, fst);
		
	}

	delete[] curent;
	delete[] next;
	return false;
}
