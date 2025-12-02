#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global counters for comparisons and swaps
unsigned long long comparisons = 0;
unsigned long long swaps = 0;

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swaps++;
}

// Partition function for QuickSort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        comparisons++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// QuickSort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to compare unsigned long long for qsort
int compare_ull(const void* a, const void* b) {
    unsigned long long val1 = *(unsigned long long*)a;
    unsigned long long val2 = *(unsigned long long*)b;
    return (val1 > val2) - (val1 < val2);
}

// Function to compute median of an array
unsigned long long compute_median(unsigned long long arr[], int n) {
    qsort(arr, n, sizeof(unsigned long long), compare_ull);
    if (n % 2 == 0) {
        return (arr[n/2 - 1] + arr[n/2]) / 2;
    } else {
        return arr[n/2];
    }
}

int main() {
    srand(time(0));
    
    // Open file to store results
    FILE* fp = fopen("quicksort_median_data.csv", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    // Write CSV header
    fprintf(fp, "ArraySize,MedianComparisons,MedianSwaps\n");
    
    // Loop through array sizes from 100 to 1000
    for (int size = 100; size <= 1000; size += 100) {
        int num_runs = size; // Number of runs equals array size (100, 200, ..., 1000)
        unsigned long long* comp_array = (unsigned long long*)malloc(num_runs * sizeof(unsigned long long));
        unsigned long long* swap_array = (unsigned long long*)malloc(num_runs * sizeof(unsigned long long));
        
        // Run QuickSort num_runs times
        for (int run = 0; run < num_runs; run++) {
            // Reset counters
            comparisons = 0;
            swaps = 0;
            
            // Create and fill array with random numbers
            int* arr = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 10000; // Random numbers between 0 and 9999
            }
            
            // Run QuickSort
            quickSort(arr, 0, size - 1);
            
            // Store results
            comp_array[run] = comparisons;
            swap_array[run] = swaps;
            
            // Free array memory
            free(arr);
        }
        
        // Compute medians
        unsigned long long median_comparisons = compute_median(comp_array, num_runs);
        unsigned long long median_swaps = compute_median(swap_array, num_runs);
        
        // Write median results to CSV
        fprintf(fp, "%d,%llu,%llu\n", size, median_comparisons, median_swaps);
        
        // Free memory
        free(comp_array);
        free(swap_array);
    }
    
    fclose(fp);
    printf("Data written to quicksort_median_data.csv\n");
    return 0;
}
