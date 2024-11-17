#include"Parser.h"

namespace parser {

	struct info
	{
		int  index = 0;
		int line = 0;
		int error = 0;
	}  data;

	std::stack<info> info_saves;


	inline void push_save() {
		info_saves.push(data);
	}
	inline void get_save() {
		data = info_saves.top();
		info_saves.pop();
	}

	void Parse(LT::Lexem_table table, std::list<Rule> rules)
	{
		GRB::Greibach grb;
		GRB::get_GRB(grb, NS("S"), TS("$"), rules);

		MFST::MFST mfst;
		MFST::create_MFST(mfst, table, grb);


		while (true)
		{
			if (table.table[data.index].lexema == "|") {
				data.index++;
				data.line++;
			}

			auto buffer_temp = mfst.buffer;
			//ОТладка
			while (!buffer_temp.empty()) {
				std::cout << (grb.isT(buffer_temp.top()) ? (char)buffer_temp.top() : (char)(-(buffer_temp.top())));
				buffer_temp.pop();
			}
			std::cout << '\n';

			MFST::Results res = mfst.step(data.error);



			switch (res)
			{
			case MFST::Results::OK_RESUULT:

				data.index++;

				break;
			case MFST::Results::FRONG_SYMBOL:

				//откатываемся на предыдушее сохранение 

				try {
					mfst.get_last_save();
					mfst.chain_id++;
					get_save();
				}
				catch(...) {
					throw(data.error, data.line, data.index);
				}

				break;
			case MFST::Results::FIND_RULE:

				//Просто продолжаем
				push_save();

				break;
			case MFST::Results::NO_RULE:

				//исключение

				throw Error::get_error_in(data.error, data.line, data.index);

				break;
			case MFST::Results::FATAL_ERRROR:

				throw Error::get_error(0);
				//исключение неизвестная ошибка
				break;
			case MFST::Results::LENTA_END_GOOD:

				return;
				break;

			case MFST::Results::FILE_EMPTY:

					return;
				break;
			}


		}
	}

}
