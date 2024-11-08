#pragma once
#include<list>
#include<string>
#include<cstdarg>
#include"comon.h"


#define GRB_ERROR 600
#define NS(n) Rule::N(n)
#define TS(n) Rule::T(n)

namespace RULE {
	namespace key {

		struct Elemet {
			std::wstring str;
			char lexem[LEXEMA_SIZE + 1];
			DataType::Type type = DataType::Type::None;
			extra::Type extra = extra::Type::None;

			Elemet(const std::wstring& str, const char* lexem) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

			}
			Elemet(const std::wstring& str, const char* lexem, DataType::Type type) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

				this->type = type;
			}
			Elemet(const std::wstring& str, const char* lexem, extra::Type extra) {

				this->str = str;
				memcpy_s(this->lexem, sizeof(this->lexem), lexem, LEXEMA_SIZE);
				this->lexem[LEXEMA_SIZE] = '\0';

				this->extra = extra;
			}
			Elemet() {
				lexem[0] = '\0';
			}
		};

		extern std::list <Elemet> key_words;
	}

	namespace GRB {

		typedef short GRBALPHABET;  ///<0 - Нетерминал >0 - терминал
		struct Rule {
			
			struct Chain;

			GRBALPHABET start_symbol;
			int size = 0;
			int error_id;
			std::list<Chain> chains;
		
		
			struct Chain {
				std::list<GRBALPHABET> elements;
				int size = 0;

				Chain(unsigned int size, GRBALPHABET first, ...) {
					va_list args;
					va_start(args, first);

					elements.push_back(first);

					for (int i = 1; i < size; i++) {
						elements.push_back(va_arg(args, GRBALPHABET));
					}

					va_end(args);
				}
			};


			Rule() {
				start_symbol = 0;
				error_id = GRB_ERROR;
			}
			Rule(GRBALPHABET start_symbol,int error_id, std::list<Chain> chains) {
				this->error_id = error_id;
				this->start_symbol = start_symbol;
				this->chains = chains;
				this->size = chains.size();
			}

			static GRBALPHABET N(const char lexem[LEXEMA_SIZE]) {
				GRBALPHABET sum = 0;
				for (int i = 0; i < LEXEMA_SIZE || lexem[i] != '\0'; i++) {
					sum += lexem[i];
				}
				return -sum;
			}
			static GRBALPHABET T(const char lexem[LEXEMA_SIZE]) {
				GRBALPHABET sum = 0;
				for (int i = 0; i < LEXEMA_SIZE || lexem[i] != '\0'; i++) {
					sum += lexem[i];
				}
				return sum;
			}
		};


		extern std::list<Rule> general_rules;
	}
}