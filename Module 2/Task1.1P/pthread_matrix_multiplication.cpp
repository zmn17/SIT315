#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std::chrono;

struct ThreadArgs {
  int row;
  std::vector<std::vector<int>> *matrix_a;
  std::vector<std::vector<int>> *matrix_b;
  std::vector<std::vector<int>> *result;
};

void *multiplyRow(void *args) {
  ThreadArgs *arg = (ThreadArgs *)args;
  int row = arg->row;
  auto &matrix_a = *(arg->matrix_a);
  auto &matrix_b = *(arg->matrix_b);
  auto &result = *(arg->result);

  int colsA = matrix_a[0].size();
  int colsB = matrix_b[0].size();

  // Matrix multiplication
  for (int col = 0; col < colsB; col++) {
    result[row][col] = 0;
    for (int k = 0; k < colsA; k++) {
      result[row][col] += matrix_a[row][k] * matrix_b[k][col];
    }
  }

  // exit the thread
  pthread_exit(nullptr);
}

int main() {
  std::vector<std::vector<int>> matrix_a = {{0, 1, 2, 3}, {4, 5, 6, 7}}; // 2x4
  std::vector<std::vector<int>> matrix_b = {
      {3, 6, 3}, {2, 1, 8}, {7, 4, 5}, {9, 6, 7}}; // 4x2

  std::vector<std::vector<int>> result(matrix_a.size(),
                                       std::vector<int>(matrix_b[0].size(), 0));

  int rowsA = matrix_a.size();

  // array to hold thread IDs
  pthread_t threads[3];
  // array to hold arguments (row indices)
  ThreadArgs thread_args[rowsA];

  try {
    auto start = high_resolution_clock::now();

    for (int i = 0; i < rowsA; i++) {
      thread_args[i] = {i, &matrix_a, &matrix_b, &result}; // set the row index
      pthread_create(&threads[i], nullptr, multiplyRow,
                     (void *)&thread_args[i]);
    }

    for (int i = 0; i < rowsA; i++) {
      pthread_join(threads[i], nullptr);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    // Open a file
    std::ofstream outputFile("pthread.txt", std::ios::app);
    if (!outputFile) {
      std::cout << "Error opening the file." << std::endl;
      return 1;
    }

    // Write the result to the file
    outputFile << std::endl;
    outputFile << "Resulting matrix (Parallel Multiplication): " << std::endl;
    for (const auto &row : result) {
      for (int val : row) {
        outputFile << val << " ";
      }
      outputFile << std::endl;
    }
    outputFile << "Execution Time (ms): " << duration.count() << std::endl;

    // Close the file
    outputFile.close();

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
