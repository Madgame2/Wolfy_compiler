#include"Control_flow.h"

namespace Constrol_flow {

	void delete_all(branch* curent) {
		if (curent == nullptr) {
			return;
		}

		for (int i = 0; i < curent->children.size(); i++) {
			delete_all(curent->children[i]);

			delete curent->children[i];
		}

	}


	void Control_flow::beign()
	{
		is_active = true;

		branch* new_branch = new branch();

		root = new_branch;
		curent = new_branch;

	}

	bool Control_flow::analyze()
	{
		is_active = false;


		delete_all(root);

		root = nullptr;
		curent = nullptr;

		return false;
	}
	void Control_flow::create_new_branch()
	{
		branch* new_branch = new  branch();

		curent->children.push_back(new_branch);
		new_branch->parent = curent;


		curent = new_branch;
	}
	void Control_flow::merge_last()
	{

		branch* parent_branch = curent->parent;

		curent->children.push_back(parent_branch);
		
		curent = parent_branch;
	}
}