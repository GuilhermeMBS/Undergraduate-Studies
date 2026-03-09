#include <stdio.h>

typedef unsigned int Set;


Set set_remove(Set s, int i) {
	return s ^ (1 << i-1); // Remove the i-th element (as it starts at 0)
}


Set set_diferenca(Set r, Set s) {
	return r & ~s; // Elements in r that are not in se
}


void print_binary(Set s) {
	for (int i = 31; i >= 0; i--) {
		printf("%d", (s >> i) & 1);
		if (i % 4 == 0) printf(" "); // Separate in 4 digits
	}
	printf("\n");
}


int main() {
	Set s = ~0u; // Initialize set with all it's elements

	// Test the functions:
	print_binary(s);
	s = set_remove(s, 5);
	print_binary(s);
	Set n = set_diferenca(s, 31);
	print_binary(n);

	return 0;
}