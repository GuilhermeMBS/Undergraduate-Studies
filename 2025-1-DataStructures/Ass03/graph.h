#pragma once
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define E 14

struct node {
	int idx; // Node index
	int weight; // Weight from vector index to this node
	struct node* next; // Pointer to next node within this index
};
typedef struct node Node;


struct edge { // Pair (n1,n2) with weight
	int n1; // Node one of (n1,n2)
	int n2; // Node two of (n1,n2)
	int weight; // Node two of (n1,n2)
};
typedef struct edge Edge;

Node* createNode(int n_idx, int n_weight);
Node* insertNode(Node* head, Node* new_node);
void createLink(int n1, int n2, int weight, Node** adj, Edge* edges, int* pt);
int cmpEdges(const void* a, const void* b);
void buildGraph(Node** adj, Edge* edges);
void freeNodes(Node** adj);