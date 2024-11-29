#pragma once
#include<vector>


namespace Constrol_flow {


	struct branch {
		bool is_returnable = false;
		branch* parent = nullptr;
		std::vector<branch*> children;


	};

	struct Control_flow {

		bool is_active = false;

		branch* root = nullptr;
		branch* curent = nullptr;

		void beign();
		bool analyze();

		void create_new_branch();
		void merge_last();
	};



}