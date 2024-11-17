#include"AST.h"


namespace AST {
	void create_ast(program_struct& tree, RULE::GRB::GRBALPHABET start)
	{
		tree.root = new node;

		tree.root->symbol[0] = (char)-start;
		tree.root->symbol[1] = '\0';
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

}