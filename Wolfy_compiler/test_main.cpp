#include<iostream>
#include"Params.h"
#include"in.h"

using namespace std;

int wmain(int argc, wchar_t* argv[]) {

	Param::Params param = Param::getParams(argc, argv);
	Param::checking_in_params(param);

	for (int i = 0; i < param.in.size; i++) {
		wcout << param.in.data[i] << endl;
	}

	in::IN input_files = in::get_IN(param);



	Param::delete_all(param);
}