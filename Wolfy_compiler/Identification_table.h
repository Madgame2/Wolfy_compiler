#pragma once
#include <string>
#include <list>
#include <locale>
#include <codecvt>
#include"comon.h"
#include"Error.h"

namespace ID {

    struct Entry {
        std::wstring name;
        std::wstring area;
        DataType::Type d_type = DataType::Type::None;
        IDType::Type id_type = IDType::Type::None;

        bool is_array = false;
        int arg_count = 0;
        int func_unick_id = 0;

        std::list<extra::Type> extras;

        std::list<DataType::Type> params;
    };

    struct ID_table
    {
        std::list<Entry> table;
        int size = 0;
    };

    void add(ID_table& table ,Entry entry);
    Entry& getEntry(ID_table& table, int  id);
    int isId(ID_table& table, std::wstring name, std::wstring function_name);
    std::string to_string(ID_table table);

}