#include"Rules.h"

namespace RULE {
    namespace key {
        std::list<Elemet> key_words = {
            Elemet(L"int", "t",DataType::Type::Int),
            Elemet(L"void","t",DataType::Type::None),
            Elemet(L"string","t",DataType::Type::String),
            Elemet(L"function","f"),                         //���������� �������
            Elemet(L"unsigned","e")
        };
    }
    namespace GRB {
        //������� ��������

        std::list<Rule> general_rules = {
            Rule(NS("S"), GRB_ERROR+1,
                {
                    Rule::Chain(3,TS("t"),TS("i"),TS(";")),
                    Rule::Chain(5,TS("t"),TS("i"),TS("="),NS("N"),TS(";"))
                })
        };
    }
}