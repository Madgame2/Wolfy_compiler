#include"Rules.h"

namespace RULE {
    namespace key {
        std::list<Elemet> key_words = {
            Elemet(L"int", "t",DataType::Type::Int),
            Elemet(L"void","t",DataType::Type::None),
            Elemet(L"string","t",DataType::Type::String),
            Elemet(L"function","f"),                         //Обьявление функции
            Elemet(L"unsigned","e")
        };
    }
    namespace GRB {
        //Правила Грейбаха

        std::list<Rule> general_rules = {
            Rule(NS("N"), GRB_ERROR+1,
                {
                    Rule::Chain(3,TS("t"),TS("i"),TS(";")),
                    Rule::Chain(3,TS("t"),TS("i"),TS("="),NS("N"),TS(";"))
                })
        };
    }
}