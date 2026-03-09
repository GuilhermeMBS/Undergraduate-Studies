#pragma once
#include <stdlib.h>

// Node for a Huffman tree
struct node {
	int freq;
	unsigned char key; // To store from 0 to 255 (ASCII values)
	struct node* left;
	struct node* right;
};
typedef struct node Node;

Node* createNode(int key, int freq); // Create a tree node