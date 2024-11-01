#include"Identification_table.h"


namespace ID {

    void add(ID_table& table ,Entry entry)
    {
        table.table.push_back(entry);
        table.size++;
    }

    Entry& getEntry(ID_table& table, int id)
    {
        if (id < 0 || id >= table.size) throw Error::get_error(7);

        int i = 0;
        for (auto& elem : table.table) {
            
            if (i == id) {
                return elem;
            }

            i++;
        }
    }

    int isId(ID_table& table, std::wstring name, std::wstring function_name)
    {

        int id = 0;
        for (auto& elem : table.table) {
            if (elem.name == name && elem.area == function_name) {
                return id;
            }
            id++;
        }

        return -1;
    }
    
}