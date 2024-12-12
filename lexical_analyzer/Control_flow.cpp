#include"Control_flow.h"

namespace Constrol_flow {

	branch* DFS_get_leaf_arr(branch* root, int& out_size) {
		if (!root) { // Проверка на nullptr
			out_size = 0;
			return nullptr;
		}

		std::stack<branch*> buffer;
		std::vector<branch*> out;
		std::set<branch*> visited;

		buffer.push(root); // Начинаем с корня

		while (!buffer.empty()) {
			branch* current = buffer.top();
			buffer.pop();

			// Пропускаем уже посещённые узлы
			if (visited.find(current) != visited.end()) {
				continue;
			}

			visited.insert(current);

			// Если узел — лист, добавляем в результат
			if (current->children.empty()) {
				out.push_back(current);
			}
			else {
				// Иначе добавляем детей в стек
				for (branch* child : current->children) {
					if (visited.find(child) == visited.end()) {
						buffer.push(child);
					}
				}
			}
		}

		// Возвращаем результат в виде массива
		out_size = out.size();
		branch* result = new branch[out_size];
		for (int i = 0; i < out_size; ++i) {
			result[i] = *out[i];
		}

		return result;
	}

	void delete_all(branch* curent) {
		if (curent == nullptr) {
			return;
		}

		for (int i = 0; i < curent->children.size(); i++) {
			delete_all(curent->children[i]);

			curent->delete_me();
		}

	}


	void Control_flow::beign()
	{
		//this->alayzable = alayzable;
		is_active = true;

		branch* new_branch = new branch();

		root = new_branch;
		curent = new_branch;
		new_branch->id = 0;
	}

	bool Control_flow::analyze()
	{
		is_active = false;

		int size;
		branch* leaf = DFS_get_leaf_arr(root, size);

		bool result = true;
		for (int i = 0; i < size; i++) {
			if (!leaf[i].is_returnable) {
				result = false;
			}
		}

		delete_all(root);

		root = nullptr;
		curent = nullptr;

		return result;
	}
	void Control_flow::create_new_branch()
	{

		
		if (curent&&!curent->is_returnable) {


			branch* new_branch = new  branch();

			curent->children.push_back(new_branch);
			new_branch->parent.push_back(curent);
			new_branch->id = curent->id + 1;

			curent = new_branch;
		}
		else {
			//Тут можно выводить предупреждения 
		}
	}
	void Control_flow::merge_last()
	{
		branch* new_node = new branch();
		branch* parent_branch = curent->parent.back();

		parent_branch->children.push_back(new_node);
		curent->children.push_back(new_node);

		new_node->parent.push_back(curent);
		new_node->parent.push_back(parent_branch);

		new_node->id = curent->id - 1;

		curent = new_node;
	}
	void Control_flow::set_returnable()
	{
		if(curent!=nullptr)
		curent->is_returnable = true;

	}
	void branch::delete_me()
	{
		for (int i = 0; i < parent.size(); i++) {
			branch* parent_branch = parent[i];
			int id = parent_branch->get_branch_arr_id(this);

			if (id != -1) {
				parent_branch->children.erase(parent_branch->children.begin() + id);
			}
		}

		delete this;
	}

	int branch::get_branch_arr_id(branch* obj)
	{
		for (int i = 0; i < children.size(); i++) {
			if (children[i] == obj) {
				return  i;
			}
		}

		return -1;
	}

}