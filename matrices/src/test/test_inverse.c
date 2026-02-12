#include "matrix_test.h"

static matrix_t A;
static matrix_t result;
static matrix_t identity;
static matrix_t product;

START_TEST(test_inverse_1x1) {
  create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 0.2, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_2x2) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);

  // Проверяем что A * A^(-1) = I
  create_matrix(2, 2, &identity);
  identity.matrix[0][0] = 1.0;
  identity.matrix[0][1] = 0.0;
  identity.matrix[1][0] = 0.0;
  identity.matrix[1][1] = 1.0;

  mult_matrix(&A, &result, &product);
  ck_assert_int_eq(eq_matrix(&product, &identity), SUCCESS);

  remove_matrix(&A);
  remove_matrix(&result);
  remove_matrix(&identity);
  remove_matrix(&product);
}
END_TEST

START_TEST(test_inverse_3x3) {
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

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);

  // из примера в ТЗ
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][1], -1.0, EPS);
  ck_assert_double_eq_tol(result.matrix[0][2], 1.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][0], -38.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][1], 41.0, EPS);
  ck_assert_double_eq_tol(result.matrix[1][2], -34.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][0], 27.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][1], -29.0, EPS);
  ck_assert_double_eq_tol(result.matrix[2][2], 24.0, EPS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_zero_det) {
  create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 4.0;

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 2);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_not_square) {
  create_matrix(2, 3, &A);

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 2);

  remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_invalid) {
  int status = inverse_matrix(NULL, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_inverse_identity) {
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

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(eq_matrix(&A, &result), SUCCESS);

  remove_matrix(&A);
  remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_property) {
  create_matrix(3, 3, &A);

  // Случайная обратимая матрица
  A.matrix[0][0] = 4.0;
  A.matrix[0][1] = 7.0;
  A.matrix[0][2] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 6.0;
  A.matrix[1][2] = 1.0;
  A.matrix[2][0] = 2.0;
  A.matrix[2][1] = 5.0;
  A.matrix[2][2] = 3.0;

  int status = inverse_matrix(&A, &result);
  ck_assert_int_eq(status, 0);

  // A * A^(-1) = I
  create_matrix(3, 3, &identity);
  identity.matrix[0][0] = 1.0;
  identity.matrix[0][1] = 0.0;
  identity.matrix[0][2] = 0.0;
  identity.matrix[1][0] = 0.0;
  identity.matrix[1][1] = 1.0;
  identity.matrix[1][2] = 0.0;
  identity.matrix[2][0] = 0.0;
  identity.matrix[2][1] = 0.0;
  identity.matrix[2][2] = 1.0;

  mult_matrix(&A, &result, &product);
  ck_assert_int_eq(eq_matrix(&product, &identity), SUCCESS);

  remove_matrix(&A);
  remove_matrix(&result);
  remove_matrix(&identity);
  remove_matrix(&product);
}
END_TEST

Suite* suite_inverse(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Inverse Matrix");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_inverse_1x1);
  tcase_add_test(tc_core, test_inverse_2x2);
  tcase_add_test(tc_core, test_inverse_3x3);
  tcase_add_test(tc_core, test_inverse_zero_det);
  tcase_add_test(tc_core, test_inverse_not_square);
  tcase_add_test(tc_core, test_inverse_invalid);
  tcase_add_test(tc_core, test_inverse_identity);
  tcase_add_test(tc_core, test_inverse_property);

  suite_add_tcase(s, tc_core);
  return s;
}