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

}