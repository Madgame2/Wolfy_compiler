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
	std::string to_string(Literal_table table)
	{
        std::string result = "";
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        int i = 0;
        for (auto id : table.table) {
            std::string buffer;
            DataType::Type d_type = id.d_type;

            std::string D_type;
            switch (d_type)
            {
            case DataType::Type::Int:
                D_type = "Int";
                break;
            case DataType::Type::Float:
                D_type = "Float";
                break;
            case DataType::Type::String:
                D_type = "String";
                break;
            case DataType::Type::Short:
                D_type = "Short";
                break;
            case DataType::Type::None:
                D_type = "NONE";
                break;
            default:
                break;
            }




            std::string name = converter.to_bytes(id.value);
            buffer = "ID: " + std::to_string(i) + "\t\t namme: " + name + "\t\t DataType: " + D_type + '\n';
            result += buffer;

            i++;
        }

        return result;
	}
}
