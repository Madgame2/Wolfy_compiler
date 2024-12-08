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
            Elemet(L"Console","c"),
            Elemet(L"console","c"),
            Elemet(L"unsigned","e",extra::Type::Unsigned)
        };
    }
    namespace GRB {
        //Правила Грейбаха

        std::list<Rule> general_rules = {
            Rule(NS("S"), GRB_ERROR + 1,
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
                Rule::Chain(4,TS("i"),TS(";"),NS("M")),
                Rule::Chain(5,TS("i"),TS("="),NS("E"),TS(";"),NS("M")),
                Rule::Chain(11,TS("?"),TS("("),TS("t"),TS("i"),TS("="),NS("E"),TS(")"),TS("{"),NS("B"),TS("}"),NS("M")),
                Rule::Chain(8,TS("?"),TS("("),NS("E"),TS(")"),TS("{"),NS("B"),TS("}"),NS("M")),
                Rule::Chain(5,TS("c"),TS("<"),NS("E"),TS(";"),NS("M")),
                Rule::Chain(5,TS("c"),TS(">"),TS("i"),TS(";"),NS("M")),
                Rule::Chain(3,NS("E"),TS(";"),NS("M")),
                Rule::Chain()
            }),
            Rule(NS("E"),GRB_ERROR + 2,{
                    Rule::Chain(1,TS("l")),
                    Rule::Chain(1,TS("i")),
                    Rule::Chain(3,TS("i"),TS("("),TS(")")),
                    Rule::Chain(4,TS("i"),TS("("),NS("A"),TS(")")),
                    Rule::Chain(2,TS("-"),TS("l")),
                    Rule::Chain(2,TS("-"),TS("i")),
                    Rule::Chain(2,TS("+"),TS("l")),
                    Rule::Chain(2,TS("+"),TS("i")),
                    Rule::Chain(3,TS("("),NS("E"),TS(")")),
                    Rule::Chain(5,TS("("),NS("E"),TS(")"),NS("O"),NS("E")),
                    Rule::Chain(6,TS("("),NS("E"),TS(")"),NS("O"),TS("="),NS("E")),
                    //Rule::Chain(4,NS("E"),NS("O"),TS("="),NS("E")),
                    //Rule::Chain(4,NS("E"),TS("="),TS("="),NS("E")),
                    Rule::Chain(3,NS("E"),NS("O"),NS("E")),

            }),
            Rule(NS("A"),GRB_ERROR + 4,{
                Rule::Chain(1,TS("i")),
                Rule::Chain(3,TS("i"),TS("("),TS(")")),
                Rule::Chain(4,TS("i"),TS("("),NS("A"),TS(")")),
                Rule::Chain(1,TS("l")),
                Rule::Chain(3,TS("i"),TS(","),NS("A")),
                Rule::Chain(1,NS("E"))
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
                Rule::Chain(3,TS("i"),TS(";"),NS("B")),
                Rule::Chain(5,TS("i"),TS("="),NS("E"),TS(";"),NS("B")),
                Rule::Chain(11,TS("?"),TS("("),TS("t"),TS("i"),TS("="),NS("E"),TS(")"),TS("{"),NS("B"),TS("}"),NS("B")),
                Rule::Chain(8,TS("?"),TS("("),NS("E"),TS(")"),TS("{"),NS("B"),TS("}"),NS("B")),
                Rule::Chain(3,TS("r"),TS(";"),NS("B")),
                Rule::Chain(4,TS("r"),NS("E"),TS(";"),NS("B")),

            }),
        };
    }


    namespace convetable_dataType {

        namespace save {

            std::list<data_type> save_convertable = {
                data_type(DataType::Type::Int, {}),
                data_type(DataType::Type::Float, {}),
                data_type(DataType::Type::String, {})
            };
        }
    }

    namespace CODE {



        std::list<templates> prefabs = {
            templates(comand::PROGRAM_BEGIN,"ASM_header.asm",{"<start>"}),
            templates(comand::VAR_delclarete,"var.asm",{"<var>","<data>"}),
            templates(comand::MAIN_INIT,"ASM_main.asm",{"<main>"}),
            templates(comand::ASSIGN_VALUE,"assign_a_value.asm",{"<if_block>","<func_code>","<main_code>"}),
            //templates(comand::ASSIGN_EXPRESSION,"ASM_expresin.asm",{"<expresion>"}),
            //templates(comand::Expression_init,"Expression_init.asm",{"<expresion>"}),
            //templates(comand::Assign_to_var,"assign_to_var.asm",{"<func_code>","<main_code>"})
            templates(comand::Expression_init,"Expression_init.asm",{"<if_block>","<func_code>","<main_code>"}),
            templates(comand::ASSIGN_EXPRESSION,"Expression_bin.asm",{"<expresion>"}),
            templates(comand::Expression_plus,"EXPRESION_add.asm",{"<operation>"}),
            templates(comand::Expression_push,"stack_push_expression.asm",{"<expresion>"}),
            templates(comand::Expression_mul,"EXPRESION_mul.asm",{"<operation>"}),

            templates(comand::Func_init,"function_preafab.asm",{"<code>"}),
            templates(comand::Func_proto,"function_proto.asm",{"<function_proto>"}),
            templates(comand::Func_call,"function_call.asm",{"<if_block>","<main_code>"}),
            templates(comand::Func_push_arg,"function_push_param.asm",{"<arg>"}),
            templates(comand::Func_as_a_arg,"function_as_a_arg.asm",{"<arg>"}),
            templates(comand::Func_ret,"function_ret.asm",{"<func_code>"}),
            templates(comand::Func_clear,"clear_local_var.asm",{"<templ_var>","<func_code>"}),

            templates(comand::if_init,"if_prefab.asm",{"<if_block>","<func_code>","<main_code>"})
        };

        std::map<DataType::Type, asm_info> DataType_AsmCode = {
            {DataType::Type::Int, asm_info("DWORD","eax")}
        };
        std::map<std::string, comand> operatin_AsmCode = {
            {"+",comand::Expression_plus},
            {"*",comand::Expression_mul}
        };
    }
}