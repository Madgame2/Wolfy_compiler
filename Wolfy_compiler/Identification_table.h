#pragma once
#include <string>
#include <list>
#include"comon.h"
#include"Error.h"

namespace ID {

    struct Entry {
        std::wstring name;
        std::wstring area;
        DataType::Type d_type = DataType::Type::None;
        IDType::Type id_type = IDType::Type::None;

        bool is_array = false;

        std::list<extra::Type> extras;
    };

    struct ID_table
    {
        std::list<Entry> table;
        int size = 0;
    };

    void add(ID_table& table ,Entry entry);
    Entry& getEntry(ID_table& table, int  id);
    int isId(ID_table& table, std::wstring name, std::wstring function_name);

}