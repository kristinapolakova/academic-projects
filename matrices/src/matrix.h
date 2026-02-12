#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0
#define EPS 1e-7

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int create_matrix(int rows, int columns, matrix_t* result);
void remove_matrix(matrix_t* A);
int eq_matrix(matrix_t* A, matrix_t* B);
int sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int mult_number(matrix_t* A, double number, matrix_t* result);
int mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int transpose(matrix_t* A, matrix_t* result);
int calc_complements(matrix_t* A, matrix_t* result);
int determinant(matrix_t* A, double* result);
int inverse_matrix(matrix_t* A, matrix_t* result);

int is_valid(const matrix_t* A);
int is_same_size(const matrix_t* A, const matrix_t* B);
int is_square(const matrix_t* A);
double det_recursive(matrix_t* A);
int create_minor(matrix_t* A, int row, int col, matrix_t* result);
int inverse_1x1(double det, matrix_t* result);  // Убрали параметр A
int inverse_nxn(matrix_t* A, double det, matrix_t* result);

#endif  // MATRIX_H