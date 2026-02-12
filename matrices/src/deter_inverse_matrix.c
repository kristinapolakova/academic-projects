#include "matrix.h"

int determinant(matrix_t* A, double* result) {
  int status = 0;

  if (!is_valid(A)) {
    status = 1;
  } else if (!is_square(A)) {
    status = 2;
  } else {
    *result = det_recursive(A);
  }

  return status;
}

int inverse_matrix(matrix_t* A, matrix_t* result) {
  int status = 0;
  double det = 0.0;

  if (!is_valid(A)) {
    status = 1;
  } else if (!is_square(A)) {
    status = 2;
  } else {
    status = determinant(A, &det);

    if (status == 0) {
      if (fabs(det) < EPS) {
        status = 2;
      } else {
        if (A->rows == 1) {
          status = inverse_1x1(det, result);
        } else {
          status = inverse_nxn(A, det, result);
        }
      }
    }
  }

  return status;
}