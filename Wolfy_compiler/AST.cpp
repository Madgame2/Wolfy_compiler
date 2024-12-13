#include"AST.h"


namespace AST {
	void create_ast(program_struct& tree, RULE::GRB::GRBALPHABET start)
	{
		tree.root = new node;
	}

    void delete_node(node* current_node) {
        if (current_node == nullptr) {
            return; 
        }

        
        for (node* child : current_node->links) {
            if (child != nullptr) {
                delete_node(child); 
            }
        }

        
        current_node->links.clear();

        
        delete current_node;
    }


    void delete_node(node* sorce, node* node) {
        delete_node(node);

        int i = get_node_id(sorce, node);

        sorce->links.erase(sorce->links.begin() + i);
    }

    int get_node_id(node* sorce, node* node)
    {
        for (int i = 0; i < sorce->links.size(); i++) {
            if (sorce->links[i] == node) {
                return i;
            }
        }

        return -1;
    }

    node** get_node_ref(node* from, node* to)
    {
        for (auto& elem : from->links) {
            if (elem == to) {
                return &elem;
            }
        }

        return nullptr;
    }

    void program_struct::Reset()
    {
        node* curent = root;

        if (curent == nullptr) {

        }

        dfs::entry new_entry;
        new_entry.node = curent;
        new_entry.offset = 0;

        DFS.stack.push(new_entry);
        //DFS.queue.push(new_entry);
    }
    

    node* program_struct::dfs::Step()
    {
        // Если стек пуст, обход завершен
        if (stack.empty()) return nullptr;

        entry& current = stack.top();

        // Обработка текущего узла (первое посещение)
        if (current.offset == 0) {
            current.offset++; // Отмечаем, что текущий узел был обработан
            return current.node; // Возвращаем узел
        }

        // Переход к дочерним узлам
        if (!current.node->links.empty() && current.offset - 1 < current.node->links.size()) {
            entry new_entry;
            new_entry.node = current.node->links[current.offset - 1]; // Переход к следующему дочернему узлу
            new_entry.offset = 0;

            stack.push(new_entry); // Добавляем дочерний узел в стек
            return Step(); // Рекурсивно обрабатываем следующий узел
        }

        // Возврат к родителю, если все дочерние узлы обработаны
        stack.pop(); // Удаляем текущий узел из стека
        if (!stack.empty()) {
            stack.top().offset++; // Увеличиваем смещение у родительского узла
        }
        return Step(); // Переходим к следующему узлу
    }


    //node* program_struct::dfs::Step()
    //{
    //    entry curent = queue.front();

    //    if (curent.node->symbol_type==AST::symbol_type::NonTerminal) {
    //        this->queue.pop();

    //        for (auto& elem : curent.node->links) {
    //            entry new_entry;
    //            new_entry.node = elem;
    //            new_entry.offset = 0;
    //            
    //            this->queue.push(new_entry);
    //        }
    //    }
    //    else {
    //        this->queue.pop();
    //    }

    //    return curent.node;
    //}


}