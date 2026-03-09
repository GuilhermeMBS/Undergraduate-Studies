#include "graph.h"

void dfs(Node** adj, int* visited, int index) {
	visited[index] = 1; // Mark as visited
	printf("%d ", index); // Print it
	for (Node* curr = adj[index]; curr != NULL; curr = curr->next) { // Walks the linked list
		if (!visited[curr->idx]) dfs(adj, visited, curr->idx); // If not visited, enter its linked list
	}
	return;
}


void bfs(Node** adj, int first_node) {
	int visited[N];
	int queue[N];
	int p1 = 0, p2 = 0; // Two pointers: start (p1) and end (p2) of the queue

	// Initialize the visited vector
	for (int i = 0; i < N; i++) visited[i] = 0;

	queue[p2++] = first_node;
	visited[first_node] = 1;

	while (p1 < p2) {
		int index = queue[p1++]; // Current node (always the start of the queue)
		printf("%d ", index); // Print it

		for (Node* curr = adj[index]; curr != NULL; curr = curr->next) { // Walk index node's linked list
			int node = curr->idx; // Node's value
			if (!visited[node]) {
				queue[p2++] = node; // Add node to the end of the queue
				visited[node] = 1; // Mark it as visited
			}
		}
	}

	return;
}


int find(int* parent, int node) {
	if (parent[node] != node)
		parent[node] = find(parent, parent[node]); // Path compression
	return parent[node];
}


void unionSets(int* parent, int* rank, int n1, int n2) {
	int p1 = find(parent, n1); // Parent of n1
	int p2 = find(parent, n2); // Parent of n2

	if (p1 == p2) return; // They are in the same set

	// Unite the shorter to the taller tree
	if (rank[p1] < rank[p2]) {
		parent[p1] = p2;
	}
	else if (rank[p1] > rank[p2]) {
		parent[p2] = p1;
	}

	// Both have the same rank
	else {
		parent[p2] = p1; // Chose arbitrarily one of them to be the parent
		rank[p1]++; // Increase the rank
	}

	return;
}


void kruskal(Node** adj, Edge* edges) {
	int parent[N];
	int rank[N];
	int mstWeight = 0, count = 0; // Total weight and count of edges in the spanning tree

	// Initialize parents and ranks
	for (int i = 0; i < N; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	for (int i = 0; i < E && count < N - 1; i++) { // While not finishes all the N-1 vertices or ends the edges
		// Stores to not have to access them multiple times
		Edge e = edges[i];
		int v1 = e.n1;
		int v2 = e.n2;

		if (find(parent, v1) != find(parent, v2)) { // They are in different sets
			unionSets(parent, rank, v1, v2); // Unite sets
			printf("(%d, %d) weight: %d\n", v1, v2, e.weight); // Print edge
			mstWeight += e.weight; // Add the weight
			count++; // Increase number of edges connected
		}
	}

	printf("Total MST Weight: %d\n", mstWeight); // Prints total cost

	return;
}


int main() {
	Node* adj[N]; // Adjacent list
	Edge edges[E]; // Edges vector
	int visited[N]; // Visited vector for the DFS

	// Initialize the adjacency matrix and the visited vector
	for (int i = 0; i < N; i++) {
		visited[i] = 0;
		adj[i] = NULL;
	}

	// Build graph and do the searchs
	buildGraph(adj, edges);

	printf("DFS Order:\n");
	dfs(adj, visited, 0); // DFS
	printf("\n\nBFS Order:\n");
	bfs(adj, 0); // BFS
	printf("\n\nEdges in Minimum Spanning Tree (MST):\n");
	kruskal(adj, edges); // Kruskal
	
	freeNodes(adj);

	return 0;
}