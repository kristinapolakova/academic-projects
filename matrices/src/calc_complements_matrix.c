#include "matrix.h"

int calc_complements(matrix_t* A, matrix_t* result) {
  int status = 0;

  if (!is_valid(A)) {
    status = 1;
    return status;
  }

  if (!is_square(A)) {
    status = 2;
    return status;
  }

  status = create_matrix(A->rows, A->columns, result);
  if (status != 0) {
    return status;
  }

  if (A->rows == 1) {
    result->matrix[0][0] = 1.0;
    return status;
  }

  int operation_ok = 1;

  for (int i = 0; i < A->rows && operation_ok; i++) {
    for (int j = 0; j < A->columns && operation_ok; j++) {
      matrix_t minor;
      int minor_status = create_minor(A, i, j, &minor);

      if (minor_status != 0) {
        status = minor_status;
        operation_ok = 0;
        continue;
      }

      double minor_det = det_recursive(&minor);
      double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
      result->matrix[i][j] = sign * minor_det;
      remove_matrix(&minor);
    }
  }

  if (operation_ok == 0) {
    remove_matrix(result);
  }

  return status;
}