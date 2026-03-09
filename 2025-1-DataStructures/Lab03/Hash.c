#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h> // To calculate the time of insertions

#define X 1201
#define T 1000

typedef unsigned long long ull;

// First hash
int hash(char* s) {
    ull h = 0ULL;
    for (int i = 0; i < 7; i++) {
        if ('0' <= s[i] && s[i] <= '9') {
            h = (h * 37ULL + (ull)(s[i] - '0')) % (ull)X;
        }
        else if ('A' <= s[i] && s[i] <= 'Z') {
            h = (h * 37ULL + (ull)(s[i] - 'A')) % (ull)X;
        }
    }
    return (int)(h % (ull)X);
}


void insertPlate(char* plate, char table[][8], int* k) {
    int h = hash(plate); 
    int collision = 0;
    while (table[h][0] != '\0') {
        collision++;
        h += 37 + collision * (h % 1103); // Collision treatment with double hashing
        h = h % X;
    }
    (*k) += collision;
    strcpy(table[h], plate);
    return;
}


int findPlate(char* plate, char table[][8]) {
    int index = hash(plate);
    int collision = 0;
    while (strcmp(table[index], plate) != 0) {
        collision++;
        index += 37 + collision * (index % 1103); // Collision treatment with double hashing
        index = index % X;
    }
    return index;
}


void removePlate(char* plate, char table[][8]) {
    int index = hash(plate);
    int collision = 1;

    while (strcmp(table[index], plate) != 0) {
        index += collision * (index % 1103); // Collision treatment with double hashing
        index = index % X;
    }

    table[index][0] = '\0';
    return;
}


int main() {
    char hash_table[X][8]; // Hash table
    char tmp[8]; // Store string from file
    int k = 0; // Collision count
    int ti_idx = 0, tf_idx = 0; // Index for time insertion and finding
    ull i_times[T], f_times[T]; // Stores times from insertion and finding

    // Initializing the hash table
    for (int i = 0; i < X; i++) hash_table[i][0] = '\0';

    // Initializing the times' vector and the finding one
    for (int i = 0; i < T; i++) {
        i_times[i] = 0L;
        f_times[i] = 0L;
    }

    // Opening file
    FILE* fp = fopen("dados_placas.txt", "r");
    if (fp == NULL) {
        printf("File Opening Error!\n");
        exit (1);
    }

    LARGE_INTEGER freq; // Clock tick frequence
    if (!QueryPerformanceFrequency(&freq)) {
        fprintf(stderr, "High‐resolution performance counter not available.\n"); // Checks failing
        fclose(fp); // Close file
        exit(2);
    }

    LARGE_INTEGER base_cnt;
    QueryPerformanceCounter(&base_cnt); // Store the base clock ticks

    // Reading file for insertion
    while (fscanf(fp, "%7s", tmp) == 1) {
        insertPlate(tmp, hash_table, &k);

        LARGE_INTEGER now_cnt;
        QueryPerformanceCounter(&now_cnt); // Get the clock ticks before the insertions
        LONGLONG delta_ticks = now_cnt.QuadPart - base_cnt.QuadPart; // Difference in ticks

        // Convert ticks → microseconds: (delta_ticks / freq.QuadPart) = seconds elapsed
        ull us = (ull)((delta_ticks * 1000000ULL) / (ull)freq.QuadPart); // * 1e6 => Convert to microseconds
        i_times[ti_idx++] = us; // Send to the times vector
    }

    // Print the table
    for (int i = 0; i < X; i++) {
        if (hash_table[i][0] == '\0') printf("index: %4d\tPlaca: -1\n", i);
        else printf("index: %4d\tPlaca: %s\n", i, hash_table[i]);
    }
    printf("\nNumber of collisions: %d\n", k); // Print the number of collisions

    /*
    // Testing the find function
    int fplate = findPlate("WNO2103", hash_table);
    printf("\nIndex of WNO2103: %d\n", fplate);

    // Testing the remove function
    removePlate("WNO2103", hash_table);
    for (int i = 0; i < X; i++) {
        if (hash_table[i][0] == '\0') printf("index: %4d\tPlaca: -1\n", i);
        else printf("index: %4d\tPlaca: %s\n", i, hash_table[i]);
    }
    */

    // Print insertion times
    printf("\nInsertion time in micro seconds:\n");
    for (int i = 0; i < T; i++) printf("Up through %4d inserts: %8llu micro seconds\n", i, i_times[i]);

    // Reading file for finding
    printf("\nFinding time in micro seconds:\n");
    rewind(fp);
    while (fscanf(fp, "%7s", tmp) == 1) {
        // Record timestamp just before finding:
        LARGE_INTEGER start_cnt, end_cnt;
        QueryPerformanceCounter(&start_cnt);

        int idx = findPlate(tmp, hash_table);

        QueryPerformanceCounter(&end_cnt);
        LONGLONG delta_ticks = end_cnt.QuadPart - start_cnt.QuadPart;
        ull us = (ull)((delta_ticks * 1000000ULL) / (ull)freq.QuadPart);

        printf("Found plate %-7s at index %4d in %8llu micro seconds\n", tmp, idx, f_times[tf_idx]);
        tf_idx++;
    }

    fclose(fp); // Close file
    return 0;
}