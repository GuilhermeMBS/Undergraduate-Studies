#include "graph.h"

// Creates a new node with weight for a specific index of the linked list
Node* createNode(int n_idx, int n_weight) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node != NULL) {
		node->idx = n_idx;
		node->weight = n_weight;
		node->next = NULL;
	}
	return node;
}

// Adds a node to the end of the linked list of curr_node
Node* insertNode(Node* head, Node* new_node) {
	if (head == NULL) {
		head = new_node;
		return new_node;
	}

	Node* pt = head;
	while (pt->next != NULL) pt = pt->next;
	pt->next = new_node;

	return head;
}

// Creates the nodes and inserts it in the linked list. Do the same for the edges
void createLink(int n1, int n2, int weight, Node** adj, Edge* edges, int* pt) {
	Node* v2 = createNode(n2, weight);
	if (v2 == NULL) exit(1); // Check the memory allocation
	adj[n1] = insertNode(adj[n1], v2);

	// Undirected graph (goes both ways)
	Node* v1 = createNode(n1, weight);
	if (v1 == NULL) exit(1); // Check the memory allocation
	adj[n2] = insertNode(adj[n2], v1);

	// Insert edge in the edges vector
	edges[(*pt)++] = (Edge){ n1, n2, weight };

	return;
}

// Compare function to sort the edges by weight
int cmpEdges(const void* a, const void* b) {
	return ((Edge*)a)->weight - ((Edge*)b)->weight; // Edge* to cast the void*
}

// Creates the graph of the exercise list
void buildGraph(Node** adj, Edge* edges) {
	int pt = 0; // Pointer for adding the edges
	
	// Creating graph connections
	createLink(0, 1, 4, adj, edges, &pt); // (0, 1) -> 4
	createLink(0, 8, 8, adj, edges, &pt); // (0, 8) -> 8
	createLink(1, 3, 8, adj, edges, &pt); // (1, 3) -> 8
	createLink(1, 8, 11, adj, edges, &pt); // (1, 8) -> 11
	createLink(3, 4, 7, adj, edges, &pt); // (3, 4) -> 7
	createLink(3, 6, 4, adj, edges, &pt); // (3, 6) -> 4
	createLink(3, 9, 2, adj, edges, &pt); // (3, 9) -> 2
	createLink(4, 5, 9, adj, edges, &pt); // (4, 5) -> 9
	createLink(4, 6, 14, adj, edges, &pt); // (4, 6) -> 14
	createLink(5, 6, 10, adj, edges, &pt); // (5, 6) -> 10
	createLink(6, 7, 2, adj, edges, &pt); // (6, 7) -> 2
	createLink(7, 8, 1, adj, edges, &pt); // (7, 8) -> 1
	createLink(7, 9, 6, adj, edges, &pt); // (7, 9) -> 6
	createLink(8, 9, 7, adj, edges, &pt); // (8, 9) -> 7
	
	qsort(edges, E, sizeof(Edge), cmpEdges); // Sort the edges by weight to use in Kruskal

	return;
}

// Free Memory of the Nodes
void freeNodes(Node** adj) {
	for (int i = 0; i < N; i++) {
		Node* pt = adj[i];
		while (pt != NULL) {
			Node* tmp = pt;
			pt = pt->next;
			free(tmp);
		}
	}
}