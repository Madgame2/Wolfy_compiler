#include"MFST.h"


void push_to_stack(std::stack<GRBALPHABET>& stack, RULE::GRB::Rule::Chain chain) {

	std::stack<GRBALPHABET> buffer;
	for (auto& elem : chain.elements) {
		buffer.push(elem);
	}

	while (!buffer.empty()) {
		stack.push(buffer.top());
		buffer.pop();
	}
}


namespace MFST {

	namespace save {
		std::stack<save::saves> saves_stack;
	}

	void create_MFST(MFST& mfst, LT::Lexem_table table, GRB::Greibach grb)
	{
		mfst.lenta_size = table.size;
		mfst.lenta = new GRBALPHABET[table.size];

		for (int i = 0; i < mfst.lenta_size; i++) {
			mfst.lenta[i] = TS(table.table[i].lexema);
		}

		mfst.grb = grb;
		mfst.buffer.push(grb.end);
		mfst.buffer.push(grb.Start);
	}
	Results MFST::step(int& error_code, int& chain_size)
	{

		if (lenta_size == 0) return Results::FILE_EMPTY;

		if (lenta_position == lenta_size) {
			if (buffer.top() == grb.end) {
				return Results::LENTA_END_GOOD;
			}
			else {

				try { 
					grb.getChain_empty(buffer.top());
					buffer.pop();
					chain_size = -1;
					return Results::FIND_RULE;
				}
				catch (...) {
					return Results::FRONG_SYMBOL;
				}
			}
		}
		else if(buffer.top() == grb.end) {
			return Results::FRONG_SYMBOL;
		}


		if (GRB::Greibach::isN(this->buffer.top())) {

			RULE::GRB::Rule::Chain chain;
			try
			{
				 chain = grb.getChain(this->buffer.top(), this->lenta[lenta_position], chain_id);
				 

				
			}
			catch (...) {
				//Попробовать найти первый попавшийся нетерминальнй символ

				chain_id = 0;


				try {
					chain = grb.getChain_firstN(this->buffer.top(), chain_id);


					if (buffer.top()==NS("E") && buffer.size() > lenta_size - lenta_position+1) {
						std::cout << lenta_size << std::endl;
						std::cout << lenta_position << std::endl;
						return Results::NO_RULE;
					}
				}
				catch (...) {
					//Возможно допустимо пустой переход 

					try {

						chain = grb.getChain_empty(this->buffer.top());
					}
					catch(...) {

						return Results::NO_RULE;
					}
				}
			};

			chain_size = chain.elements.size();
			error_code = grb.getRule(this->buffer.top()).error_id;
			make_save(this->lenta_position, this->buffer, chain_id);

			buffer.pop();
			push_to_stack(this->buffer, chain);

			chain_id = 0;
			return Results::FIND_RULE;
		}
		else if (GRB::Greibach::isT(this->buffer.top())) {
			if (lenta[lenta_position] == buffer.top()) {

				buffer.pop();
				lenta_position++;

				return Results::OK_RESUULT;
				//достаем из стека
				//Двишаем ленту 
			}
			else {
				return Results::FRONG_SYMBOL;
				//возрашаем ошибку: символы не совпадают
			}
		}


		return Results::FATAL_ERRROR;
	}
	void MFST::make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer)
	{

		save::saves new_save;
		new_save.buffer = buffer;
		new_save.lenta_position = lenta;
		new_save.n_rule_cnain = 0;

		save::saves_stack.push(new_save);
	}
	void MFST::make_save(unsigned int lenta, std::stack<GRBALPHABET> buffer, unsigned int n_chain)
	{

		save::saves new_save;
		new_save.buffer = buffer;
		new_save.lenta_position = lenta;
		new_save.n_rule_cnain = n_chain;

		save::saves_stack.push(new_save);
	}
	void MFST::get_last_save()
	{

		if (save::saves_stack.empty()) throw;


		save::saves elem = save::saves_stack.top();
		save::saves_stack.pop();

		this->buffer = elem.buffer;
		this->chain_id = elem.n_rule_cnain;
		this->lenta_position = elem.lenta_position;
	}
}
