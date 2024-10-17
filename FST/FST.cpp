#include"FST.h"


fst::FST::FST(short states_count, NODE node, ...)
{
	this->nstates = states_count;
	this->nodes = new NODE[nstates];

	nodes[0] = node;

	va_list args;
	va_start(args, node);

	for (int i = 1; i < states_count; i++) {
		this->nodes[i] = va_arg(args, NODE);
	}

	va_end(args);
}

fst::NODE::NODE()
{
}

fst::NODE::NODE(short count_ralations, RELATION rel, ...)
{
	this->count_relations = count_ralations;
	this->relations = new RELATION[count_ralations];

	relations[0] = rel;

	va_list args;
	va_start(args, rel);

	for (int i = 1; i < count_ralations; ++i) {
		relations[i] = va_arg(args, RELATION); // Извлекаем следующий аргумент типа RELATION
	}

	va_end(args);
}

fst::RELATION::RELATION(wchar_t symbol, short next_node)
{
	this->nex_node = next_node;
	this->symbol = symbol;
}


short* build_ailable_statests(unsigned short& size, short* arr, fst::FST fst) {

	// используем unordered_set для быстрого поиска доступных состояний
	std::unordered_set<short> available_states;

	// добавляем доступные NODE из исходного массива
	for (int i = 0; i < size; i++) {
		available_states.insert(arr[i]);
	}

	// Пробегаемся по каждой доступной NODE
	std::vector<short> states_queue(available_states.begin(), available_states.end());

	for (int i = 0; i < states_queue.size(); i++) {
		fst::NODE current = fst.nodes[states_queue[i]];

		// Проверяем все связи из текущего узла
		for (int j = 0; j < current.count_relations; j++) {
			fst::RELATION rel = current.relations[j];

			if (rel.symbol == ZERO_TRANSITION) { // Если это "нулевой переход"

				// Если узел ещё не добавлен в список доступных состояний
				if (available_states.find(rel.nex_node) == available_states.end()) {
					available_states.insert(rel.nex_node);
					states_queue.push_back(rel.nex_node); // Добавляем в очередь
				}
			}
		}
	}

	// Обновляем размер и пересоздаем массив arr
	size = available_states.size();
	if (arr != nullptr) {
		delete[] arr;
	}
	short* new_arr = new short[size];

	// Копируем элементы в массив
	int index = 0;
	for (short state : available_states) {

		new_arr[index] = state;
		index++;
	}
	return new_arr;
}


bool fst::execute(std::wstring string, FST fst)
{
	short* curent = new short[fst.nstates];
	short* next = new short[fst.nstates];

	for (int i = 0; i < fst.nstates; i++) {
		i == 0 ? curent[i] = 0 : curent[i] = -1;
		next[i] = -1;
	}

	//Определяем начальное состояние 
	short* available_states = new short[1];
	available_states[0] = 0;
	unsigned short available_statest_count = 1;

	for (int i = 0; i < string.size();i++) {

		//available_states = build_ailable_statests(available_statest_count, available_states, fst);
	
		std::vector<short> new_available_states;

		for (int j = 0; j < available_statest_count; j++) {
			NODE curent_node = fst.nodes[available_states[j]];

			for (int p = 0; p < curent_node.count_relations; p++) {
				if (curent_node.relations[p].symbol == string[i]) {
					new_available_states.push_back(curent_node.relations[p].nex_node);
					curent[curent_node.relations[p].nex_node] = i + 1;
				}
			}
		}

		short* temp = curent;
		curent = next;
		next = temp;


		delete[]available_states;
		available_statest_count = new_available_states.size();
		available_states = new short[available_statest_count];
		for (int j = 0; j < available_statest_count; j++) {
			available_states[j] = new_available_states[j];
		}
		
		if (new_available_states.size() == 0&&i!= string.size()-1) return false;


	}

	delete[] curent;
	curent = next;
	if (curent[fst.nstates - 1] == string.size()) {

		delete[] curent;
		return true;
	}

	//delete[] curent;
	return false;
}
