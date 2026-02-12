#include "matrix_test.h"

static matrix_t A;
static matrix_t B;

void setup_eq_matrix(void) {
  create_matrix(2, 2, &A);
  create_matrix(2, 2, &B);
}

void teardown_eq_matrix(void) {
  remove_matrix(&A);
  remove_matrix(&B);
}

START_TEST(test_eq_identical) {
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);
}
END_TEST

START_TEST(test_eq_different) {
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.1;

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_different_size) {
  remove_matrix(&B);
  create_matrix(3, 3, &B);

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_precision_success) {
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 1.0 + 1e-8;

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);
}
END_TEST

START_TEST(test_eq_precision_failure) {
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 1.0 + 1e-6;

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_null_a) {
  int result = eq_matrix(NULL, &B);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_null_b) {
  int result = eq_matrix(&A, NULL);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_both_null) {
  int result = eq_matrix(NULL, NULL);
  ck_assert_int_eq(result, FAILURE);
}
END_TEST

START_TEST(test_eq_large_matrix) {
  remove_matrix(&A);
  remove_matrix(&B);
  create_matrix(10, 10, &A);
  create_matrix(10, 10, &B);

  int i = 0;
  while (i < 10) {
    int j = 0;
    while (j < 10) {
      A.matrix[i][j] = i * 10.0 + j;
      B.matrix[i][j] = i * 10.0 + j;
      j = j + 1;
    }
    i = i + 1;
  }

  int result = eq_matrix(&A, &B);
  ck_assert_int_eq(result, SUCCESS);
}
END_TEST

Suite* suite_eq_matrix(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Equal Matrix");
  tc_core = tcase_create("Core");

  tcase_add_checked_fixture(tc_core, setup_eq_matrix, teardown_eq_matrix);

  tcase_add_test(tc_core, test_eq_identical);
  tcase_add_test(tc_core, test_eq_different);
  tcase_add_test(tc_core, test_eq_different_size);
  tcase_add_test(tc_core, test_eq_precision_success);
  tcase_add_test(tc_core, test_eq_precision_failure);
  tcase_add_test(tc_core, test_eq_null_a);
  tcase_add_test(tc_core, test_eq_null_b);
  tcase_add_test(tc_core, test_eq_both_null);
  tcase_add_test(tc_core, test_eq_large_matrix);

  suite_add_tcase(s, tc_core);
  return s;
}