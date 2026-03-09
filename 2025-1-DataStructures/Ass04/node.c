#include "node.h"
#include <stdlib.h>

// Allocate and initialize a new node
Node* createNode(int key, int freq) {
	Node* new_node = (Node*)malloc(sizeof(Node));

	if (new_node != NULL) { // Check the allocation
		new_node->freq = freq;
		new_node->key = (unsigned char)key;
		new_node->left = NULL;
		new_node->right = NULL;
	}

	return new_node;
}