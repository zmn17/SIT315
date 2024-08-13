#include <fstream>
#include <iostream>
#include <vector>

using namespace std::chrono;

std::vector<std::vector<int>>
multiplyMatrices(const std::vector<std::vector<int>> &matrixA,
                 const std::vector<std::vector<int>> &matrixB) {

  // Get the dimensions of both matrices
  int rowsA = matrixA.size();
  int colsA = matrixA[0].size();
  int rowsB = matrixB.size();
  int colsB = matrixB[0].size();

  // Check if the matrices can be multiplied
  if (colsA != rowsB) {
    throw std::invalid_argument("Number of columns in matrixA must be equal to "
                                "the number of rows in matrixB");
  }

  // Result matrix
  std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

  // Matrix multiplication
  for (int row = 0; row < rowsA; row++) {
    for (int col = 0; col < colsB; col++) {
      for (int k = 0; k < colsA; k++) {
        result[row][col] += matrixA[row][k] * matrixB[k][col];
      }
    }
  }
  return result;
}

int main() {
  std::vector<std::vector<int>> matrixA = {{0, 1, 2, 3}, {4, 5, 6, 7}}; // 2x4
  std::vector<std::vector<int>> matrixB = {
      {3, 6, 3}, {2, 1, 8}, {7, 4, 5}, {9, 6, 7}}; // 4x2

  // A = 0 1 2 3          B = 3 6 3
  //     4 5 6 7              2 1 8
  //                          7 4 5
  //                          9 6 7

  try {
    auto start = high_resolution_clock::now();
    std::vector<std::vector<int>> result = multiplyMatrices(matrixA, matrixB);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    // Open a file
    std::ofstream outputFile("result_outupt.txt", std::ios::app);
    if (!outputFile) {
      std::cout << "Error opening the file." << std::endl;
      return 1;
    }

    // Write the result to the file
    outputFile << "Resulting matrix (Sequential Multiplication): " << std::endl;
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
