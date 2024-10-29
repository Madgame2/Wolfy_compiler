#pragma once

#define LEXEMA_SIZE 1
#define LEX_LET "l"
#define LEX_ID "i"



    namespace DataType {
        enum class Type {
            Int,
            Float,
            String,
            Short,
            None
        };
    }

    namespace IDType {
        enum class Type {
            Var,
            Func,
            Param,
            None
        };
    }

    namespace extra {
        enum class Type {
            Unsigned,
            None
        };
    }