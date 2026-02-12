#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "../matrix.h"

// Функции для тестов
Suite *suite_create_remove(void);
Suite *suite_eq_matrix(void);
Suite *suite_sum_sub(void);
Suite *suite_mult(void);
Suite *suite_transpose(void);
Suite *suite_determinant(void);
Suite *suite_calc_complements(void);
Suite *suite_inverse(void);

void run_tests(void);
int main(void);

#endif  // MATRIX_TEST_H