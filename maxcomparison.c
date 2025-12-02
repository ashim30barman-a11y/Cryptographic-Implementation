#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Global counters for comparisons and swaps
unsigned long long comparisons = 0;
unsigned long long swaps = 0;

// Swap function
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swaps++;
}

// QuickSort Implementation
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

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge Sort Implementation
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
            swaps++; // Counting merge as swap for consistency
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Bubble Sort Implementation
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Heap Sort Implementation
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n) {
        comparisons++;
        if (arr[left] > arr[largest]) largest = left;
    }
    if (right < n) {
        comparisons++;
        if (arr[right] > arr[largest]) largest = right;
    }
    
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    srand(time(0));
    
    // Open file to store results
    FILE* fp = fopen("sorting_max_data.csv", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    // Write CSV header
    fprintf(fp, "ArraySize,QuickSortComp,QuickSortSwaps,MergeSortComp,MergeSortSwaps,BubbleSortComp,BubbleSortSwaps,HeapSortComp,HeapSortSwaps\n");
    
    // Loop through array sizes from 100 to 1000
    for (int size = 100; size <= 1000; size += 100) {
        int num_runs = size; // Number of runs equals array size
        unsigned long long max_qs_comp = 0, max_qs_swaps = 0;
        unsigned long long max_ms_comp = 0, max_ms_swaps = 0;
        unsigned long long max_bs_comp = 0, max_bs_swaps = 0;
        unsigned long long max_hs_comp = 0, max_hs_swaps = 0;
        
        // Run each sorting algorithm
        for (int run = 0; run < num_runs; run++) {
            // Create array
            int* arr = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) arr[i] = rand() % 10000;
            
            // QuickSort
            comparisons = 0; swaps = 0;
            int* arr_qs = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) arr_qs[i] = arr[i];
            quickSort(arr_qs, 0, size - 1);
            if (comparisons > max_qs_comp) max_qs_comp = comparisons;
            if (swaps > max_qs_swaps) max_qs_swaps = swaps;
            free(arr_qs);
            
            // MergeSort
            comparisons = 0; swaps = 0;
            int* arr_ms = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) arr_ms[i] = arr[i];
            mergeSort(arr_ms, 0, size - 1);
            if (comparisons > max_ms_comp) max_ms_comp = comparisons;
            if (swaps > max_ms_swaps) max_ms_swaps = swaps;
            free(arr_ms);
            
            // BubbleSort
            comparisons = 0; swaps = 0;
            int* arr_bs = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) arr_bs[i] = arr[i];
            bubbleSort(arr_bs, size);
            if (comparisons > max_bs_comp) max_bs_comp = comparisons;
            if (swaps > max_bs_swaps) max_bs_swaps = swaps;
            free(arr_bs);
            
            // HeapSort
            comparisons = 0; swaps = 0;
            int* arr_hs = (int*)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) arr_hs[i] = arr[i];
            heapSort(arr_hs, size);
            if (comparisons > max_hs_comp) max_hs_comp = comparisons;
            if (swaps > max_hs_swaps) max_hs_swaps = swaps;
            free(arr_hs);
            
            free(arr);
        }
        
        // Write results to CSV
        fprintf(fp, "%d,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu\n",
                size, max_qs_comp, max_qs_swaps, max_ms_comp, max_ms_swaps,
                max_bs_comp, max_bs_swaps, max_hs_comp, max_hs_swaps);
    }
    
    fclose(fp);
    printf("Data written to sorting_max_data.csv\n");
    return 0;
}
