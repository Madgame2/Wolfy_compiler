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
	Results MFST::step()
	{
		if (GRB::Greibach::isN(this->buffer.top())) {

			try
			{
				RULE::GRB::Rule::Chain chain = grb.getChain(this->buffer.top(), this->lenta[0], chain_id);

				push_to_stack(this->buffer, chain);
				chain_id = 0;
			}
			catch (...) {
				//Вернуть что нету парвила 

				return Results::NO_RULE;
			};

			
		}
		else if (GRB::Greibach::isT(this->buffer.top())) {
			if (lenta[lenta_position] == buffer.top()) {

				buffer.pop();
				lenta_position++;

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
}
