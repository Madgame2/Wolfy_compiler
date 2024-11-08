#include"Parser.h"

namespace parser {
	void Parse(LT::Lexem_table table)
	{
		GRB::Greibach grb;
		GRB::get_GRB(grb, NS("S"), TS("$"), GENERAl);

		MFST::MFST mfst;
		MFST::create_MFST(mfst, table, grb);

		while (true)
		{
			break;
		}
	}

}