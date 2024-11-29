#pragma once



namespace Constrol_flow {


	struct branch {



	};

	struct Control_flow {

		bool is_active = false;

		branch* root = nullptr;

		void beign();
		bool analyze();
	};



}