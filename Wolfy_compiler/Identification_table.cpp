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

    std::string to_string(ID_table table)
    {
        std::string result = "";
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        int i = 0;
        for (auto id: table.table) {
            std::string buffer;
            DataType::Type d_type = id.d_type;
            IDType::Type id_type = id.id_type;

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

            std::string ID_type;

            switch (id_type)
            {
            case IDType::Type::Var:
                ID_type = "VAR";
                break;
            case IDType::Type::Func:
                ID_type = "FUNC";
                break;
            case IDType::Type::Param:
                ID_type = "PARAM";
                break;
            case IDType::Type::None:
                ID_type = "NONE";
                break;
            default:
                break;
            }


            std::string name = converter.to_bytes(id.name);
            buffer = "ID: " + std::to_string(i) + "\t\t namme: " + name + "\t\t DataType: " + D_type + "\t\t ID_Type: " + ID_type + '\n';
            result += buffer;

            i++;
        }

        return result;
    }
    
}