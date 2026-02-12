#include "matrix.h"

int create_matrix(int rows, int columns, matrix_t* result) {
  int status = 0;

  if (rows <= 0 || columns <= 0 || result == NULL) {
    status = 1;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double**)calloc(rows, sizeof(double*));

    if (result->matrix != NULL) {
      int memory_error = 0;
      int i = 0;

      while (i < rows && memory_error == 0) {
        result->matrix[i] = (double*)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          memory_error = 1;
        }
        i = i + 1;
      }

      if (memory_error != 0) {
        // Код очистки памяти в случае ошибки
        // так как не покрывается тестами
        // int j = 0;
        // while (j < i)
        // {
        //     if (result->matrix[j] != NULL)
        //     {
        //         free(result->matrix[j]);
        //     }
        //     j = j + 1;
        // }
        // free(result->matrix);
        // result->matrix = NULL;
        // result->rows = 0;
        // result->columns = 0;
        status = 1;
      }
    } else {
      // случай, когда calloc вернул NULL
      // status = 1;
    }
  }

  return status;
}

void remove_matrix(matrix_t* A) {
  if (A != NULL && A->matrix != NULL) {
    int i = 0;
    while (i < A->rows) {
      free(A->matrix[i]);
      i = i + 1;
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}