import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv("sorting_max_data.csv")

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

# -------- Plot Maximum Comparisons --------
plt.figure(figsize=(10, 6))
plt.plot(sizes, qs_comp, label='QuickSort', marker='o', color='#1f77b4')
plt.plot(sizes, ms_comp, label='MergeSort', marker='s', color='#ff7f0e')
plt.plot(sizes, bs_comp, label='BubbleSort', marker='^', color='#2ca02c')
plt.plot(sizes, hs_comp, label='HeapSort', marker='d', color='#d62728')

plt.xlabel('Array Size')
plt.ylabel('Maximum Comparisons')
plt.title('Maximum Comparisons vs Array Size')
plt.legend()
plt.grid(True)

comp_img = "sorting_max_comparisons.png"
plt.savefig(comp_img, dpi=300)
plt.show()
print(f"Saved comparison plot as: {comp_img}")


# -------- Plot Maximum Swaps --------
plt.figure(figsize=(10, 6))
plt.plot(sizes, qs_swaps, label='QuickSort', marker='o', color='#1f77b4')
plt.plot(sizes, ms_swaps, label='MergeSort', marker='s', color='#ff7f0e')
plt.plot(sizes, bs_swaps, label='BubbleSort', marker='^', color='#2ca02c')
plt.plot(sizes, hs_swaps, label='HeapSort', marker='d', color='#d62728')

plt.xlabel('Array Size')
plt.ylabel('Maximum Swaps')
plt.title('Maximum Swaps vs Array Size')
plt.legend()
plt.grid(True)

swaps_img = "sorting_max_swaps.png"
plt.savefig(swaps_img, dpi=300)
plt.show()
print(f"Saved swaps plot as: {swaps_img}")

