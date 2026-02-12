#include "matrix.h"

int is_valid(const matrix_t* A) {
  int result = 1;

  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    result = 0;
  }

  return result;
}

int is_same_size(const matrix_t* A, const matrix_t* B) {
  int result = 0;

  if (is_valid(A) && is_valid(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      result = 1;
    }
  }

  return result;
}

int is_square(const matrix_t* A) {
  int result = 0;

  if (is_valid(A) && A->rows == A->columns) {
    result = 1;
  }

  return result;
}

static void fill_minor_column(matrix_t* A, int column, int row_offset,
                              matrix_t* minor) {
  int minor_row = 0;
  int i = row_offset;

  while (i < A->rows) {
    int minor_col = 0;
    int j = 0;

    while (j < A->columns) {
      if (j != column) {
        minor->matrix[minor_row][minor_col] = A->matrix[i][j];
        minor_col = minor_col + 1;
      }
      j = j + 1;
    }
    minor_row = minor_row + 1;
    i = i + 1;
  }
}

static double calculate_large_det(matrix_t* A) {
  double result = 0.0;
  matrix_t minor;

  int create_status = create_matrix(A->rows - 1, A->columns - 1, &minor);

  if (create_status == 0) {
    int column = 0;

    while (column < A->columns) {
      fill_minor_column(A, column, 1, &minor);
      double minor_det = det_recursive(&minor);
      double sign = (column % 2 == 0) ? 1.0 : -1.0;
      result = result + sign * A->matrix[0][column] * minor_det;
      column = column + 1;
    }

    remove_matrix(&minor);
  }

  return result;
}

double det_recursive(matrix_t* A) {
  double result = 0.0;

  if (A->rows == 1) {
    result = A->matrix[0][0];
  } else if (A->rows == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    result = calculate_large_det(A);
  }

  return result;
}

static void fill_minor_row(matrix_t* A, int row, int col, int i,
                           matrix_t* minor, int* minor_row) {
  if (i != row) {
    int minor_col = 0;
    int j = 0;

    while (j < A->columns) {
      if (j != col) {
        minor->matrix[*minor_row][minor_col] = A->matrix[i][j];
        minor_col = minor_col + 1;
      }
      j = j + 1;
    }
    *minor_row = *minor_row + 1;
  }
}

int create_minor(matrix_t* A, int row, int col, matrix_t* result) {
  int status = 0;

  if (!is_square(A)) {
    status = 2;
  } else {
    status = create_matrix(A->rows - 1, A->columns - 1, result);

    if (status == 0) {
      int minor_row = 0;
      int i = 0;

      while (i < A->rows) {
        fill_minor_row(A, row, col, i, result, &minor_row);
        i = i + 1;
      }
    }
  }

  return status;
}

int inverse_1x1(double det, matrix_t* result) {
  int status = create_matrix(1, 1, result);

  if (status == 0) {
    result->matrix[0][0] = 1.0 / det;
  }

  return status;
}

int inverse_nxn(matrix_t* A, double det, matrix_t* result) {
  int status = 0;
  matrix_t complements = {0};
  matrix_t transposed = {0};

  status = calc_complements(A, &complements);

  if (status == 0) {
    status = transpose(&complements, &transposed);
  }

  if (status == 0) {
    status = mult_number(&transposed, 1.0 / det, result);
  }

  if (complements.matrix != NULL) {
    remove_matrix(&complements);
  }

  if (transposed.matrix != NULL) {
    remove_matrix(&transposed);
  }

  return status;
}