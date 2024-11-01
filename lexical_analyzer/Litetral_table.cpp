#include "Litetral_table.h"

namespace Lit_table {

	void add(Literal_table& table, Element elem)
	{
		table.table.push_back(elem);
		table.size++;
	}

	Element& find(Literal_table& table, int id)
	{
		if (id<0 || id>table.size) throw Error::get_error(7);

		int index = 0;
		for (auto& elem : table.table) {
			if (index == id) return elem;
			index++;
		}
	}
}
