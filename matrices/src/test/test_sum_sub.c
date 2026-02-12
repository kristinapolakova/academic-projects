#include "matrix_test.h"

static matrix_t A;
static matrix_t B;
static matrix_t result;

void setup_arithmetic(void) {
  create_matrix(2, 2, &A);
  create_matrix(2, 2, &B);
  result.matrix = NULL;
  result.rows = 0;
  result.columns = 0;
}

void teardown_arithmetic(void) {
  remove_matrix(&A);
  remove_matrix(&B);
  remove_matrix(&result);
}

START_TEST(test_sum_basic) {
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 8.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 10.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 12.0, EPS);
}
END_TEST

START_TEST(test_sum_negative) {
  A.matrix[0][0] = 5.0;
  B.matrix[0][0] = -3.0;

  int status = sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 2.0, EPS);
}
END_TEST

START_TEST(test_sum_different_size) {
  remove_matrix(&B);
  create_matrix(3, 3, &B);

  int status = sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);
}
END_TEST

START_TEST(test_sum_invalid_a) {
  int status = sum_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_sum_invalid_b) {
  int status = sum_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_sub_basic) {
  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 6.0;
  A.matrix[1][0] = 7.0;
  A.matrix[1][1] = 8.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], 4.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 4.0, EPS);
}
END_TEST

START_TEST(test_sub_negative_result) {
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 5.0;

  int status = sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], -4.0, EPS);
}
END_TEST

START_TEST(test_sub_different_size) {
  remove_matrix(&B);
  create_matrix(1, 2, &B);

  int status = sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 2);
}
END_TEST

START_TEST(test_sub_invalid_a) {
  int status = sub_matrix(NULL, &B, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_sub_invalid_b) {
  int status = sub_matrix(&A, NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_sum_1x1) {
  remove_matrix(&A);
  remove_matrix(&B);
  create_matrix(1, 1, &A);
  create_matrix(1, 1, &B);

  A.matrix[0][0] = 2.5;
  B.matrix[0][0] = 3.5;

  int status = sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, EPS);
}
END_TEST

START_TEST(test_sub_1x1) {
  remove_matrix(&A);
  remove_matrix(&B);
  create_matrix(1, 1, &A);
  create_matrix(1, 1, &B);

  A.matrix[0][0] = 5.0;
  B.matrix[0][0] = 2.0;

  int status = sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 3.0, EPS);
}
END_TEST

Suite* suite_sum_sub(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Sum and Sub");
  tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup_arithmetic, teardown_arithmetic);

  tcase_add_test(tc_core, test_sum_basic);
  tcase_add_test(tc_core, test_sum_negative);
  tcase_add_test(tc_core, test_sum_different_size);
  tcase_add_test(tc_core, test_sum_invalid_a);
  tcase_add_test(tc_core, test_sum_invalid_b);
  tcase_add_test(tc_core, test_sub_basic);
  tcase_add_test(tc_core, test_sub_negative_result);
  tcase_add_test(tc_core, test_sub_different_size);
  tcase_add_test(tc_core, test_sub_invalid_a);
  tcase_add_test(tc_core, test_sub_invalid_b);
  tcase_add_test(tc_core, test_sum_1x1);
  tcase_add_test(tc_core, test_sub_1x1);

  suite_add_tcase(s, tc_core);
  return s;
}