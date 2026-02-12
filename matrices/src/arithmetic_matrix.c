#include "matrix.h"

int eq_matrix(matrix_t* A, matrix_t* B) {
  int result = SUCCESS;

  if (!is_valid(A) || !is_valid(B)) {
    result = FAILURE;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    result = FAILURE;
  } else {
    int i = 0;
    int continue_check = 1;

    while (i < A->rows && continue_check) {
      int j = 0;
      while (j < A->columns && continue_check) {
        double diff = fabs(A->matrix[i][j] - B->matrix[i][j]);
        if (diff > EPS) {
          result = FAILURE;
          continue_check = 0;
        }
        j = j + 1;
      }
      i = i + 1;
    }
  }

  return result;
}

int sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = 0;

  if (!is_valid(A) || !is_valid(B)) {
    status = 1;
  } else if (!is_same_size(A, B)) {
    status = 2;
  } else {
    status = create_matrix(A->rows, A->columns, result);

    if (status == 0) {
      int i = 0;
      while (i < A->rows) {
        int j = 0;
        while (j < A->columns) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
          j = j + 1;
        }
        i = i + 1;
      }
    }
  }

  return status;
}

int sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = 0;

  if (!is_valid(A) || !is_valid(B)) {
    status = 1;
  } else if (!is_same_size(A, B)) {
    status = 2;
  } else {
    status = create_matrix(A->rows, A->columns, result);

    if (status == 0) {
      int i = 0;
      while (i < A->rows) {
        int j = 0;
        while (j < A->columns) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          j = j + 1;
        }
        i = i + 1;
      }
    }
  }

  return status;
}

int mult_number(matrix_t* A, double number, matrix_t* result) {
  int status = 0;

  if (!is_valid(A)) {
    status = 1;
  } else {
    status = create_matrix(A->rows, A->columns, result);

    if (status == 0) {
      int i = 0;
      while (i < A->rows) {
        int j = 0;
        while (j < A->columns) {
          result->matrix[i][j] = A->matrix[i][j] * number;
          j = j + 1;
        }
        i = i + 1;
      }
    }
  }

  return status;
}

int mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  int status = 0;

  if (!is_valid(A) || !is_valid(B)) {
    status = 1;
  } else if (A->columns != B->rows) {
    status = 2;
  } else {
    status = create_matrix(A->rows, B->columns, result);

    if (status == 0) {
      int i = 0;
      while (i < A->rows) {
        int j = 0;
        while (j < B->columns) {
          double sum = 0.0;
          int k = 0;
          while (k < A->columns) {
            sum = sum + A->matrix[i][k] * B->matrix[k][j];
            k = k + 1;
          }
          result->matrix[i][j] = sum;
          j = j + 1;
        }
        i = i + 1;
      }
    }
  }

  return status;
}