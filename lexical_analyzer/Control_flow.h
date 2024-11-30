#pragma once
#include<vector>
#include<stack>
#include<set>
#include"Error.h"

#define BRANCH_BEGIN_DEFOULT_ID 0 

namespace Constrol_flow {

	struct branch {
		bool is_returnable = false;
		std::vector<branch*>  parent;
		std::vector<branch*> children;

		int id = -1;

		void delete_me();
		int get_branch_arr_id(branch* obj);
	};

	struct Control_flow {

		//semantic::scope::node* alayzable = nullptr;

		bool is_active = false;

		branch* root = nullptr;
		branch* curent = nullptr;

		void beign();
		bool analyze();

		void create_new_branch();
		void merge_last();

		void set_returnable();
	};



}