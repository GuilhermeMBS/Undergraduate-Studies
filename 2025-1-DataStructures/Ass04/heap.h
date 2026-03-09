#pragma once
#include "node.h"

extern int heap_size; // Sends the size to the other file
void minHeap(int* freq); // Build a min heap from a frequence array
Node* extractMinHeap(void); // Extract and return the node with smallest frequence
void insertMinHeap(Node* n); // Insert a node into the min heap
void printHeap(void); // Prints the heap