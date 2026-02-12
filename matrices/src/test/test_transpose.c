#include "matrix_test.h"

static matrix_t A;
static matrix_t result;

void setup_matrix_ops(void) {
  A.matrix = NULL;
  A.rows = 0;
  A.columns = 0;
  result.matrix = NULL;
  result.rows = 0;
  result.columns = 0;
}

void teardown_matrix_ops(void) {
  remove_matrix(&A);
  remove_matrix(&result);
}

START_TEST(test_transpose_square) {
  create_matrix(3, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 7.0;
  A.matrix[2][1] = 8.0;
  A.matrix[2][2] = 9.0;

  int status = transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][2], 7.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, EPS);
}
END_TEST

START_TEST(test_transpose_rectangular) {
  create_matrix(2, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  int status = transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 2.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 5.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][0], 3.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][1], 6.0, EPS);
}
END_TEST

START_TEST(test_transpose_1x1) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int status = transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.rows, 1);
  ck_assert_int_eq(result.columns, 1);
  ck_assert_double_eq_tol(result.matrix[0][0], 5.0, EPS);
}
END_TEST

START_TEST(test_transpose_invalid) {
  int status = transpose(NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_transpose_identity) {
  create_matrix(3, 3, &A);

  // Единичная матрица
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[0][2] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 1.0;
  A.matrix[1][2] = 0.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 1.0;

  int status = transpose(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(eq_matrix(&A, &result), SUCCESS);
}
END_TEST

Suite* suite_transpose(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Transpose");
  tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup_matrix_ops, teardown_matrix_ops);

  tcase_add_test(tc_core, test_transpose_square);
  tcase_add_test(tc_core, test_transpose_rectangular);
  tcase_add_test(tc_core, test_transpose_1x1);
  tcase_add_test(tc_core, test_transpose_invalid);
  tcase_add_test(tc_core, test_transpose_identity);

  suite_add_tcase(s, tc_core);
  return s;
}