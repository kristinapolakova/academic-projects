#include "matrix_test.h"

static matrix_t A;
static matrix_t B;
static matrix_t result;

START_TEST(test_mult_number_basic) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = mult_number(&A, 2.0, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 2.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 6.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 8.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_negative) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 3.0;

  int status = mult_number(&A, -2.0, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], -6.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_zero) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = mult_number(&A, 0.0, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 0.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_fraction) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 4.0;

  int status = mult_number(&A, 0.5, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 2.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_number_invalid) {
  int status = mult_number(NULL, 2.0, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_mult_matrix_2x2) {
  create_matrix(2, 2, &A);
  create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 2.0;
  B.matrix[0][1] = 0.0;
  B.matrix[1][0] = 1.0;
  B.matrix[1][1] = 2.0;

  int status = mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 10.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 8.0, EPS);

  remove_matrix(&A);
  remove_matrix(&B);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_2x3_3x2) {
  create_matrix(2, 3, &A);
  create_matrix(3, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  B.matrix[0][0] = 7.0;
  B.matrix[0][1] = 8.0;
  B.matrix[1][0] = 9.0;
  B.matrix[1][1] = 10.0;
  B.matrix[2][0] = 11.0;
  B.matrix[2][1] = 12.0;

  int status = mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 58.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 64.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 139.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 154.0, EPS);

  remove_matrix(&A);
  remove_matrix(&B);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
  create_matrix(2, 3, &A);
  create_matrix(2, 3, &B);

  int status = mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);

  remove_matrix(&A);
  remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_1x1) {
  create_matrix(1, 1, &A);
  create_matrix(1, 1, &B);

  A.matrix[0][0] = 3.0;
  B.matrix[0][0] = 4.0;

  int status = mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 12.0, EPS);

  remove_matrix(&A);
  remove_matrix(&B);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_null_a) {
  create_matrix(2, 2, &B);
  int status = mult_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, 1);
  remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_matrix_null_b) {
  create_matrix(2, 2, &A);
  int status = mult_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, 1);
  remove_matrix(&A);
}
END_TEST

Suite* suite_mult(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Multiplication");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mult_number_basic);
  tcase_add_test(tc_core, test_mult_number_negative);
  tcase_add_test(tc_core, test_mult_number_zero);
  tcase_add_test(tc_core, test_mult_number_fraction);
  tcase_add_test(tc_core, test_mult_number_invalid);
  tcase_add_test(tc_core, test_mult_matrix_2x2);
  tcase_add_test(tc_core, test_mult_matrix_2x3_3x2);
  tcase_add_test(tc_core, test_mult_matrix_incompatible);
  tcase_add_test(tc_core, test_mult_matrix_1x1);
  tcase_add_test(tc_core, test_mult_matrix_null_a);
  tcase_add_test(tc_core, test_mult_matrix_null_b);

  suite_add_tcase(s, tc_core);
  return s;
}