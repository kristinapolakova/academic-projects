#include "matrix.h"

int transpose(matrix_t* A, matrix_t* result) {
  int status = 0;

  if (!is_valid(A)) {
    status = 1;
  } else {
    status = create_matrix(A->columns, A->rows, result);

    if (status == 0) {
      int i = 0;
      while (i < A->rows) {
        int j = 0;
        while (j < A->columns) {
          result->matrix[j][i] = A->matrix[i][j];
          j = j + 1;
        }
        i = i + 1;
      }
    }
  }

  return status;
}