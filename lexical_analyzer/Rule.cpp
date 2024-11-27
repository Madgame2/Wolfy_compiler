#include"Rules.h"

namespace RULE {
    namespace key {
        std::list<Elemet> key_words = {
            Elemet(L"int", "t",DataType::Type::Int),
            Elemet(L"void","t",DataType::Type::None),
            Elemet(L"string","t",DataType::Type::String),
            Elemet(L"short","t",DataType::Type::Short),
            Elemet(L"float","t",DataType::Type::Float),
            Elemet(L"function","f"),                         //���������� �������
            Elemet(L"Global","g"),
            Elemet(L"if","?"),
            Elemet(L"return","r"),
            Elemet(L"unsigned","e",extra::Type::Unsigned)
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

        std::list<Rule> main_rules = {
            Rule(NS("S"),GRB_ERROR,{
                Rule::Chain(4,TS("g"),TS("{"),TS("}"),NS("M")),
                Rule::Chain(1,NS("M"))
            }),
            Rule(NS("M"),GRB_ERROR,{
                Rule::Chain(4,TS("t"),TS("i"),TS(";"),NS("M")),
                Rule::Chain(6,TS("t"),TS("i"),TS("="),NS("E"),TS(";"),NS("M")),
                Rule::Chain(9,TS("t"),TS("f"),TS("i"),TS("("),TS(")"),TS("{"),NS("B"),TS("}"),NS("M")),
                Rule::Chain(10,TS("t"),TS("f"),TS("i"),TS("("),NS("P"),TS(")"),TS("{"),NS("B"),TS("}"),NS("M")),
                Rule::Chain(9,TS("t"),TS("f"),TS("i"),TS("("),NS("P"),TS(")"),TS("{"),TS("}"),NS("M")),
                Rule::Chain(8,TS("t"),TS("f"),TS("i"),TS("("),TS(")"),TS("{"),TS("}"),NS("M")),
                Rule::Chain(8,TS("?"),TS("("),NS("C"),TS(")"),TS("{"),NS("B"),TS("}"),NS("M")),
                Rule::Chain(3,NS("E"),TS(";"),NS("M")),
                Rule::Chain()
            }),
            Rule(NS("E"),GRB_ERROR + 2,{
                    Rule::Chain(1,TS("l")),
                    Rule::Chain(1,TS("i")),
                    Rule::Chain(3,TS("("),NS("E"),TS(")")),
                    Rule::Chain(5,TS("("),NS("E"),TS(")"),NS("O"),NS("E")),
                    Rule::Chain(3,NS("E"),NS("O"),NS("E")),
                    Rule::Chain(6,TS("("),NS("E"),TS(")"),NS("O"),TS("="),NS("E")),
                    Rule::Chain(4,NS("E"),NS("O"),TS("="),NS("E")),
                    Rule::Chain(4,NS("E"),TS("="),TS("="),NS("E")),

            }),
            Rule(NS("O"),GRB_ERROR + 1,{
                    Rule::Chain(1,TS("+")),
                    Rule::Chain(1,TS("-")),
                    Rule::Chain(1,TS("/")),
                    Rule::Chain(1,TS("*")),
                    Rule::Chain(1,TS("%")),
                    Rule::Chain(1,TS("=")),
                    Rule::Chain(1,TS("<")),
                    Rule::Chain(1,TS(">")),

            }),
            Rule(NS("P"),GRB_ERROR + 3,{
                    Rule::Chain(2,TS("t"),TS("i")),
                    Rule::Chain(4,TS("t"),TS("i"),TS(","),NS("P")),
            }),
            Rule(NS("B"),GRB_ERROR + 3,{
                Rule::Chain(),
                Rule::Chain(4,TS("t"),TS("i"),TS(";"),NS("B")),
                Rule::Chain(6,TS("t"),TS("i"),TS("="),NS("E"),TS(";"),NS("B")),
                Rule::Chain(8,TS("?"),TS("("),NS("E"),TS(")"),TS("{"),NS("B"),TS("}"),NS("B")),
            }),
        };
    }
}