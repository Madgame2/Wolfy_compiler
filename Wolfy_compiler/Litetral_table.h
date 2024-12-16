#pragma once
#include"comon.h"
#include"Error.h"
#include<string>
#include<list>
#include <locale>
#include <codecvt>

namespace Lit_table {
	struct Element {
		DataType::Type d_type = DataType::Type::None;
		std::wstring value;
		notations::notation my_notation = notations::notation::None;

		int pos = -1;
		int line = -1;

		Element(){}
		Element(std::wstring value, DataType::Type type) {
			this->value = value;
			this->d_type = type;
		}
	};

	struct Literal_table {
		std::list<Element> table;
		int size = 0;
	};

	void add(Literal_table& table, Element elem);
	Element& find(Literal_table& table, int id);

	std::string to_string(Literal_table table);
}