#include <chrono>
#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std::chrono;

// arguments for thread function
struct QuickSortArgs {
  std::vector<int> *arr;
  int l;
  int h;
};

// Quick Sort implementation - Parallel

// Swap
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

void *QuickSortThread(void *args) {
  QuickSortArgs *q_args = (QuickSortArgs *)args;
  std::vector<int> *arr = q_args->arr;
  int l = q_args->l;
  int h = q_args->h;

  if (l < h) {
    int j = Partition(*arr, l, h);

    // argument structures for two threads
    QuickSortArgs leftArgs = {arr, l, j};
    QuickSortArgs rightArgs = {arr, j + 1, h};

    // create two threads for the left and right subarrays
    pthread_t leftThread, rightThread;
    pthread_create(&leftThread, NULL, QuickSortThread, &leftArgs);
    pthread_create(&rightThread, NULL, QuickSortThread, &rightArgs);

    // wait for both threads to finish
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);
  }
  pthread_exit(NULL);
}

int main() {
  std::vector<int> arr = {99, 67, 43, 24, 54, 1,  2,  32, 21, 78, 54, 5,
                          8,  9,  4,  3,  66, 75, 45, 58, 97, 86, 88, 24};

  int l = 0;
  int h = arr.size();

  // arguments for the first call to sort
  QuickSortArgs args = {&arr, l, h};
  auto start = high_resolution_clock::now();

  // start the sorting process
  pthread_t initialThread;
  pthread_create(&initialThread, NULL, QuickSortThread, &args);

  // wait for the initial thread to complete
  pthread_join(initialThread, NULL);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end - start);

  for (auto &item : arr) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::cout << "Time taken to sort parallelism: " << duration.count() << "ms"
            << std::endl;

  return 0;
}
