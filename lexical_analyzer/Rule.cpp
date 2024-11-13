#include"Rules.h"

namespace RULE {
    namespace key {
        std::list<Elemet> key_words = {
            Elemet(L"int", "t",DataType::Type::Int),
            Elemet(L"void","t",DataType::Type::None),
            Elemet(L"string","t",DataType::Type::String),
            Elemet(L"short","t",DataType::Type::Short),
            Elemet(L"float","t",DataType::Type::Float),
            Elemet(L"function","f"),                         //Обьявление функции
            Elemet(L"Global","g"),
            Elemet(L"if","?"),
            Elemet(L"return","r"),
            Elemet(L"unsigned","e",extra::Type::Unsigned)
        };
    }
    namespace GRB {
        //Правила Грейбаха

        std::list<Rule> general_rules = {
            Rule(NS("S"), GRB_ERROR+1,
                {
                    Rule::Chain(3,TS("t"),TS("i"),TS(";")),
                    Rule::Chain(5,TS("t"),TS("i"),TS("="),NS("N"),TS(";"))
                })
        };
    }
}