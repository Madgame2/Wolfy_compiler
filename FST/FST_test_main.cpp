#include<iostream>
#include"FST.h"

using namespace std;

int main() {
	fst::FST fst( 4,
		fst::NODE(3, fst::RELATION(L'a', 0), fst::RELATION(L'b', 0), fst::RELATION(L'a', 1)),
		fst::NODE(1, fst::RELATION(L'b', 2)),
		fst::NODE(1, fst::RELATION(L'a', 3)),
		fst::NODE()
	);

	cout << fst::execute(L"aaabbbabba", fst);
}
