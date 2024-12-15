#pragma once
#include"Error.h"
#include<cwchar>
#include<string>
#include<vector>
#include <cwctype>    // Для std::towlower
#include <algorithm>  // Для std::transform

#define IN_FILE_TYPE L".wolf"

namespace Param {

    struct KEYS {
        struct Key {
            std::wstring name; // Используем std::wstring для удобства
            short id = -1;

            Key(const std::wstring& n, short i) : name(n), id(i) {} // Инициализируем через список инициализации
        };

        // Инициализация массива ключей
        Key keys[2] = { 
            Key(L"-in", 0),
            Key(L"-log:",1)
        }; 
        int lenght = 2;
    };


    struct param
    {
        std::wstring* data = nullptr;
        unsigned int size = 0;

    };

	struct Params {
        param in;
		//std::wstring* param_in =nullptr;
		//wchar_t** 
        param log;

	};

	Params getParams(int argc, wchar_t* argv[]);


    bool checking_in_params(Params& params);


    void delete_all(Params& param);
}