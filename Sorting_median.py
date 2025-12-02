import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv("sorting_median_data.csv")

# Extract columns
sizes = data['ArraySize']
qs_comp = data['QuickSortComp']
qs_swaps = data['QuickSortSwaps']
ms_comp = data['MergeSortComp']
ms_swaps = data['MergeSortSwaps']
bs_comp = data['BubbleSortComp']
bs_swaps = data['BubbleSortSwaps']
hs_comp = data['HeapSortComp']
hs_swaps = data['HeapSortSwaps']

# -------- Plot Comparisons --------
plt.figure(figsize=(10, 6))
plt.plot(sizes, qs_comp, label='QuickSort', marker='o')
plt.plot(sizes, ms_comp, label='MergeSort', marker='s')
plt.plot(sizes, bs_comp, label='BubbleSort', marker='^')
plt.plot(sizes, hs_comp, label='HeapSort', marker='d')

plt.xlabel('Array Size')
plt.ylabel('Median Comparisons')
plt.title('Median Comparisons vs Array Size')
plt.legend()
plt.grid(True)

# Save output file
comp_img = "sorting_median_comparisons.png"
plt.savefig(comp_img, dpi=300)
plt.show()

print(f"Saved comparison plot as: {comp_img}")

# -------- Plot Swaps --------
plt.figure(figsize=(10, 6))
plt.plot(sizes, qs_swaps, label='QuickSort', marker='o')
plt.plot(sizes, ms_swaps, label='MergeSort', marker='s')
plt.plot(sizes, bs_swaps, label='BubbleSort', marker='^')
plt.plot(sizes, hs_swaps, label='HeapSort', marker='d')

plt.xlabel('Array Size')
plt.ylabel('Median Swaps')
plt.title('Median Swaps vs Array Size')
plt.legend()
plt.grid(True)

# Save output file
swaps_img = "sorting_median_swaps.png"
plt.savefig(swaps_img, dpi=300)
plt.show()

print(f"Saved swaps plot as: {swaps_img}")

