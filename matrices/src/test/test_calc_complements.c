#include "matrix_test.h"

static matrix_t A;
static matrix_t result;

START_TEST(test_calc_complements_1x1) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int status = calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_2x2) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], -3.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], -2.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 1.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_3x3) {
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  int status = calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 10.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][2], -20.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_calc_complements_not_square) {
  create_matrix(2, 3, &A);

  int status = calc_complements(&A, &result);
  ck_assert_int_eq(status, 2);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_invalid) {
  int status = calc_complements(NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_calc_complements_diagonal) {
  create_matrix(3, 3, &A);

  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 0.0;
  A.matrix[0][2] = 0.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 0.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 4.0;

  int status = calc_complements(&A, &result);
  ck_assert_int_eq(status, 0);

  ck_assert_double_eq_tol(result.matrix[0][0], 12.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][2], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 8.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][2], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][0], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][1], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][2], 6.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

Suite* suite_calc_complements(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Calc Complements");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_calc_complements_1x1);
  tcase_add_test(tc_core, test_calc_complements_2x2);
  tcase_add_test(tc_core, test_calc_complements_3x3);
  tcase_add_test(tc_core, test_calc_complements_not_square);
  tcase_add_test(tc_core, test_calc_complements_invalid);
  tcase_add_test(tc_core, test_calc_complements_diagonal);

  suite_add_tcase(s, tc_core);
  return s;
}