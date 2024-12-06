#pragma once
#include<list>
#include<string>
#include<cstdarg>
#include<vector>
#include<map>
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

		typedef short GRBALPHABET;  ///<0 - Íåòåðìèíàë >0 - òåðìèíàë
		struct Rule {
			
			struct Chain;

			GRBALPHABET start_symbol;
			int size = 0;
			int error_id;
			std::list<Chain> chains;
		
		
			struct Chain {
				std::list<GRBALPHABET> elements;
				int size = 0;
				bool empty = false;
				Chain(unsigned int size, GRBALPHABET first, ...) {
					va_list args;
					va_start(args, first);

					elements.push_back(first);

					for (int i = 1; i < size; i++) {
						elements.push_back(va_arg(args, GRBALPHABET));
					}

					va_end(args);
				}
				Chain() { empty = true; }
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
		extern std::list<Rule> main_rules;
	}


	namespace convetable_dataType {
		struct data_type
		{
			DataType::Type dataType;
			std::list<DataType::Type> converatable_types;

			data_type(DataType::Type type, std::list<DataType::Type> types) {
				dataType = type;
				converatable_types = types;
			}
		};
		namespace save {
			extern  std::list<data_type> save_convertable;
		}

	}

	namespace CODE {

		enum class comand {
			PROGRAM_BEGIN,
			VAR_delclarete,
			MAIN_INIT,
			ASSIGN_VALUE,
			ASSIGN_EXPRESSION,
			Expression_init,
			Assign_to_var,


			Expression_plus,
			Expression_mul,
			Expression_push,

			Func_init,
			Func_proto,
			Func_call,
			Func_push_arg,
			Func_as_a_arg,
			Func_ret,
			Func_clear
		};

		struct templates {
			comand key;
			std::string  path_to_tamplate;
			std::vector<std::string> tags;
			templates() {

			}

			templates(comand key, std::string path) {

				this->key = key;
				path_to_tamplate = path;
			}
			templates(comand key, std::string path, std::vector<std::string> tags) {

				this->key = key;
				path_to_tamplate = path;
				this->tags = tags;
			}
		};

		struct asm_info {
			std::string asm_data_type;
			std::string asm_register;


			asm_info(){}

			asm_info(std::string data_type, std::string reg) {
				this->asm_data_type = data_type;
				this->asm_register = reg;
			}
		};

		extern std::list<templates> prefabs;
		extern std::map<DataType::Type, asm_info> DataType_AsmCode;
		extern std::map<std::string, comand> operatin_AsmCode;
	}
}