#include "matrix_test.h"

static matrix_t A;
static double det_result;

START_TEST(test_determinant_1x1) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, 5.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, -2.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
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

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, 0.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3_nonzero) {
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, -1.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_4x4) {
  create_matrix(4, 4, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 0.0;
  A.matrix[0][2] = 2.0;
  A.matrix[0][3] = -1.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 0.0;
  A.matrix[1][2] = 0.0;
  A.matrix[1][3] = 5.0;
  A.matrix[2][0] = 2.0;
  A.matrix[2][1] = 1.0;
  A.matrix[2][2] = 4.0;
  A.matrix[2][3] = -3.0;
  A.matrix[3][0] = 1.0;
  A.matrix[3][1] = 0.0;
  A.matrix[3][2] = 5.0;
  A.matrix[3][3] = 0.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, 30.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_not_square) {
  create_matrix(2, 3, &A);

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 2);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_invalid) {
  int status = determinant(NULL, &det_result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_determinant_zero) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 4.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, 0.0, EPS);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_upper_triangular) {
  create_matrix(3, 3, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 5.0;
  A.matrix[2][0] = 0.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 6.0;

  int status = determinant(&A, &det_result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(det_result, 24.0, EPS);

  remove_matrix(&A);
}
END_TEST

Suite* suite_determinant(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Determinant");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_determinant_1x1);
  tcase_add_test(tc_core, test_determinant_2x2);
  tcase_add_test(tc_core, test_determinant_3x3);
  tcase_add_test(tc_core, test_determinant_3x3_nonzero);
  tcase_add_test(tc_core, test_determinant_4x4);
  tcase_add_test(tc_core, test_determinant_not_square);
  tcase_add_test(tc_core, test_determinant_invalid);
  tcase_add_test(tc_core, test_determinant_zero);
  tcase_add_test(tc_core, test_determinant_upper_triangular);

  suite_add_tcase(s, tc_core);
  return s;
}