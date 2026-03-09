#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "heap.h"

int freq[256] = { 0 }; // Frequency array for ASCII values
char* codes[256]; // Huffman codes (ASCII index → binary string)
char  codeBuf[512]; // Temporary buffer for code generation
int bitCount = 0; // Count bits
unsigned char bitBuffer = 0; // Temporary bit


void writeBit(FILE* out, int bit) {
	bitBuffer = (bitBuffer << 1) | (bit); // Shift buffer left and add the new bit in the LSB
	bitCount++; // Update count

	if (bitCount == 8) { // Reach one byte
		fputc(bitBuffer, out); // Write it down
		bitBuffer = 0; // Reset buffer
		bitCount = 0; // Reset count
	}

	return;
}


void flushBits(FILE* out) {
	if (bitCount > 0) { // Not filled one byt
		bitBuffer <<= (8 - bitCount); // Fill with zeroes
		fputc(bitBuffer, out); // Add it
		bitBuffer = 0; // Reset buffer
		bitCount = 0; // Reset count
	}

	return;
}


void printFreq(void) {
	printf("\nFREQUENCES ARRAY:\n\n");
	printf("INDEX \t CHAR \t FREQ\n");
	printf("----- \t ---- \t ----\n");

	for (int i = 0; i < 256; i++) {
		if (freq[i] == 0) continue;
		else if (i == '\n') printf("%3d \t \\n \t %d\n", i, freq[i]);
		else if (i == '\t') printf("%3d \t \\t \t%d\n", i, freq[i]);
		else if (i == '\r') printf("%3d \t \\r \t %d\n", i, freq[i]);
		else printf("%3d \t '%c' \t %d\n", i, i, freq[i]);
	}

	return;
}


Node* huffmanAlg(void) {
	while (heap_size > 1) { // Until only the root is left
		// printHeap();

		Node* left = extractMinHeap(); // Least frequency
		Node* right = extractMinHeap(); // Second least frequency
		Node* parent = createNode(0, left->freq + right->freq); // Create a node of the sum
		if (parent == NULL) return NULL; // Memory allocation check

		// Define the left and right values (0 and 1) than add the node to the heap
		parent->left = left;
		parent->right = right;
		insertMinHeap(parent);
	}

	return extractMinHeap(); // Root node
}


void printHuffman(Node* root, char* code, int depth) {
	if (root == NULL) return;

	// If it's a leaf node (char exists)
	if (root->left == NULL && root->right == NULL) {
		code[depth] = '\0'; // Make the path a string (finishes it)

		// Print the path (string)
		if (root->key == '\n') printf("\\n  : %s\n", code);
		else if (root->key == '\t') printf("\\t  : %s\n", code);
		else if (root->key == '\r') printf("\\r  : %s\n", code);
		else printf("'%c' : %s\n", root->key, code);

		return;
	}

	// Do a DFS in pre order
	code[depth] = '0'; // Left = 0
	printHuffman(root->left, code, depth + 1);
	code[depth] = '1'; // Right = 1
	printHuffman(root->right, code, depth + 1);

	return;
}


void buildCodeTable(Node* root, int depth) {
	if (root == NULL) return;

	if (root->left == NULL && root->right == NULL) { // If its a leaf
		codeBuf[depth] = '\0'; // Adds the end of the string to the temporary string
		codes[root->key] = strdup(codeBuf); // Copies dinamically the temporary string to the coding array
		return;
	}

	codeBuf[depth] = '0'; // If its left
	buildCodeTable(root->left, depth + 1);
	codeBuf[depth] = '1'; // If its right
	buildCodeTable(root->right, depth + 1);

	return;
}


void encodeFile(FILE* in, const char* outputFile) {
	FILE* out = fopen(outputFile, "wb");  // Creates the output file
	if (out == NULL) exit(1); // Memory allocation check

	bitBuffer = 0; // Reset bit buffer
	bitCount = 0; // Reset bit count

	int ch;
	while ((ch = fgetc(in)) != EOF) { // Gets the charactere in ASCII form
		char* code = codes[(unsigned char)ch];
		// Read until \0 and write in binary
		for (int i = 0; code[i] != '\0'; i++) writeBit(out, code[i] == '1' ? 1 : 0);
	}

	flushBits(out); // Pad the rest with zero‐bits on the right
	fclose(out); // Close the output file

	return;
}


void decodeFile(Node* root, const char* encodedFile, const char* decodedOutputFile) {
	FILE* in = fopen(encodedFile, "rb"); // Open the encoded file
	FILE* out = fopen(decodedOutputFile, "wb"); // Create a decoded file
	if (in == NULL || out == NULL) exit(1); // Memory allocation check

	Node* curr = root;
	int byte;

	while ((byte = fgetc(in)) != EOF) { // Read one byte at a time
		for (int i = 7; i >= 0; i--) {
			int bit = (byte >> i) & 1;
			curr = bit ? curr->right : curr->left; // 1 goes to the right, 0 goes to the left
			if (curr->left == NULL && curr->right == NULL) { // If its a leaf
				fputc(curr->key, out); // Writes the decoded char in the file
				curr = root; // Goes back to the root for the next letter
			}
		}
	}

	fclose(in); // Close input file
	fclose(out); // Close output file

	return;
}


void freeTree(Node* root) {
	if (root == NULL) return;

	// Free in post-order
	freeTree(root->left);
	freeTree(root->right);
	free(root);

	return;
}


int main(void) {
	FILE* fp = fopen("EDA Textos Huffman.txt", "rb"); // Open file in binary
	if (fp == NULL) exit(1); // Error to open file

	int ch; // Store characters from txt file
	while ((ch = fgetc(fp)) != EOF) freq[ch]++; // Read txt file to get frequences
	rewind(fp); // Rewind to be used in the encoding
	printFreq();

	minHeap(&freq); // Create a min heap for the heap
	printHeap();

	Node* huff_tree = huffmanAlg(); // Create the Huffman Tree
	if (huff_tree == NULL) exit(2); // Memory allocation error

	char h_code[256]; // Store binary sequence (path) of a character
	printf("\nHUFFMAN TREE\n\n");
	printHuffman(huff_tree, h_code, 0); // Print in pre order

	buildCodeTable(huff_tree, 0); // Build the codes array for each letter
	encodeFile(fp, "encoded.bin"); // Creates the compressed file (as text)
	decodeFile(huff_tree, "encoded.bin", "decoded.txt"); // Decodes the compressed file

	fclose(fp); // Close file
	freeTree(huff_tree); // Free memory

	return 0;
}