#include"Graibach.h"




namespace GRB {
    bool Greibach::isT(GRBALPHABET ch)
    {
        return ch>0;
    }

    bool Greibach::isN(GRBALPHABET ch)
    {
        return ch<0;
    }

    Rule Greibach::getRule(GRBALPHABET N)
    {
        return this->rules[N];
    }

    Rule::Chain Greibach::getChain(GRBALPHABET N, int rulle_index)
    {
        if (rulle_index < 0 || rulle_index >= this->rules[N].chains.size()) throw Error::get_error(7);

        int index = 0;
        for (auto& elem : this->rules[N].chains) {
            if (index == rulle_index)
            {
                return elem;
            }
            index++;
        }
    }

    Rule::Chain Greibach::getChain(GRBALPHABET N, GRBALPHABET first_T, int offset)
    {
        if (offset < 0 || offset >= this->rules[N].chains.size()) throw Error::get_error(7);


        for (auto& elem : this->rules[N].chains) {
            if (elem.elements.front() == first_T) {
                if (offset == 0) {
                    return elem;
                }
                offset--;
            }

        }

        throw;
    }

    void get_GRB(Greibach& GRB, GRBALPHABET start, GRBALPHABET stack_end, std::list<Rule> rules)
    {
        GRB.Start = start;
        GRB.end = stack_end;
        for (auto& elem : rules) {
            GRB.rules[elem.start_symbol] = elem;
        }
    }


}