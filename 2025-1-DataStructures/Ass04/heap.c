#include "heap.h"
#include <stdlib.h>

#define MAX_HEAP 256
Node* heap[MAX_HEAP]; // Min heap for the Huffman (sort the frequences)
int heap_size = 0; // Size of the heap

// Inserts in the min heap
void insertMinHeap(Node* node) {
    int idx = heap_size++; // Updates heap size

    // While its not the root and is smaller than the parent (must swap)
    while (idx > 0 && heap[(idx - 1) / 2]->freq > node->freq) {
        heap[idx] = heap[(idx - 1) / 2]; // Bubble up
        idx = (idx - 1) / 2; // Update pointer
    }

    heap[idx] = node;

    return;
}

// Creates a min heap
void minHeap(int* freq) {
    for (int i = 1; i < MAX_HEAP; i++) {
        int val = freq[i]; // Frequence of character with ASCII = i
        if (val == 0) continue; // There is no such character in the file

        Node* new_node = createNode(i, val); // Create the node
        if (new_node == NULL) exit(2); // Memory allocation error

        insertMinHeap(new_node); // Insert in min heap array
    }

    return;
}

// Extracts the min value from min heap
Node* extractMinHeap(void) {
    Node* first = heap[0]; // Pointer to the last node
    Node* last = heap[--heap_size]; // Pointer to the first node (updating the size for removal)
    int child, idx = 0; // Index of the last node's position

    // While left child is in the heap, try to bubble down the last node
    while ((child = 2 * idx + 1) < heap_size) {
        // If right child exists, compare both of them and pick the smaller (just update child variable)
        if (child + 1 < heap_size && heap[child + 1]->freq < heap[child]->freq) child++;

        if (last->freq <= heap[child]->freq) break; // If is in the right order, stop

        // Else, bubble up the child
        heap[idx] = heap[child];
        idx = child;
    }

    heap[idx] = last; // Insert the last node to its correct index

    return first; // Return the minimum value of the heap
}

// Prints the heap
void printHeap(void) {
    printf("\nHEAP ARRAY:\n\n");
    printf("INDEX \t CHAR \t FREQ\n");
    printf("----- \t ---- \t ----\n");

    for (int i = 0; i < heap_size; i++) {
        Node* node = heap[i];
        if (node->key == '\n') printf("%3d \t \\n \t %d\n", i, node->freq);
        else if (node->key == '\t') printf("%3d \t \\t \t %d\n", i, node->freq);
        else if (node->key == '\r') printf("%3d \t \\r \t %d\n", i, node->freq);
        else printf("%3d \t '%c' \t %d\n", i, node->key, node->freq);
    }

    return;
}