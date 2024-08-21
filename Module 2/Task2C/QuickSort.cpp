#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono;

// Quick Sort implementation - Sequential

// Swap the elements
void swap(std::vector<int> &arr, int x, int y) {
  int tmp = arr[x];
  arr[x] = arr[y];
  arr[y] = tmp;
}

// 1. Partition
int Partition(std::vector<int> &arr, int l, int h) {
  int pivot = arr[l];
  int i = l, j = h;

  while (i < j) {
    do {
      i++;
    } while (i <= h && arr[i] <= pivot);

    do {
      j--;
    } while (arr[j] > pivot);

    if (i < j) {
      swap(arr, i, j);
    }
  };
  swap(arr, l, j);
  return j;
}

void QuickSort(std::vector<int> &arr, int l, int h) {
  if (l < h) {
    int j = Partition(arr, l, h);
    // sorting the left and right partitions
    QuickSort(arr, l, j);
    QuickSort(arr, j + 1, h);
  }
}

int main() {
  std::vector<int> arr = {99, 67, 43, 24, 54, 1,  2,  32, 21, 78, 54, 5,
                          8,  9,  4,  3,  66, 75, 45, 58, 97, 86, 88, 24};

  int l = 0;
  int h = arr.size();

  auto start = high_resolution_clock::now();
  QuickSort(arr, l, h);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start);

  for (auto &item : arr) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::cout << "Time taken to sort sequentially: " << duration.count() << "ms"
            << std::endl;

  return 0;
}
