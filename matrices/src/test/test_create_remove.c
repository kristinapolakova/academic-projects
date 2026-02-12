#include "matrix_test.h"

// Тесты для create/remove
START_TEST(test_create_matrix_comprehensive) {
  matrix_t mat;
  int result;

  // 1. Нормальное создание
  result = create_matrix(3, 3, &mat);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(mat.rows, 3);
  ck_assert_int_eq(mat.columns, 3);
  ck_assert_ptr_nonnull(mat.matrix);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(mat.matrix[i][j], 0.0, EPS);
    }
  }
  remove_matrix(&mat);

  // 2. Прямоугольная матрица
  result = create_matrix(2, 5, &mat);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(mat.rows, 2);
  ck_assert_int_eq(mat.columns, 5);
  remove_matrix(&mat);

  // 3. Матрица 1x1
  result = create_matrix(1, 1, &mat);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(mat.rows, 1);
  ck_assert_int_eq(mat.columns, 1);
  remove_matrix(&mat);

  // 4. Отрицательные строки
  result = create_matrix(-1, 3, &mat);
  ck_assert_int_eq(result, 1);

  // 5. Отрицательные столбцы
  result = create_matrix(3, -1, &mat);
  ck_assert_int_eq(result, 1);

  // 6. Оба отрицательные
  result = create_matrix(-2, -3, &mat);
  ck_assert_int_eq(result, 1);

  // 7. Нулевые строки
  result = create_matrix(0, 3, &mat);
  ck_assert_int_eq(result, 1);

  // 8. Нулевые столбцы
  result = create_matrix(3, 0, &mat);
  ck_assert_int_eq(result, 1);

  // 9. NULL указатель
  result = create_matrix(3, 3, NULL);
  ck_assert_int_eq(result, 1);

  // 10. Большая матрица
  result = create_matrix(100, 100, &mat);
  if (result == 0) {
    ck_assert_int_eq(mat.rows, 100);
    ck_assert_int_eq(mat.columns, 100);
    ck_assert_ptr_nonnull(mat.matrix);
    remove_matrix(&mat);
  }

  // 11. Перезапись существующей
  matrix_t mat2;
  mat2.rows = 999;
  mat2.columns = 999;
  mat2.matrix = (double**)0xDEADBEEF;

  result = create_matrix(2, 2, &mat2);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(mat2.rows, 2);
  ck_assert_int_eq(mat2.columns, 2);
  ck_assert_ptr_nonnull(mat2.matrix);
  remove_matrix(&mat2);

  // 12. Заполнение и удаление
  result = create_matrix(5, 5, &mat);
  ck_assert_int_eq(result, 0);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      mat.matrix[i][j] = i * 5.0 + j;
    }
  }

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ck_assert_double_eq_tol(mat.matrix[i][j], i * 5.0 + j, EPS);
    }
  }
  remove_matrix(&mat);

  ck_assert_ptr_null(mat.matrix);
  ck_assert_int_eq(mat.rows, 0);
  ck_assert_int_eq(mat.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_edge_cases) {
  remove_matrix(NULL);

  matrix_t mat1 = {0};
  remove_matrix(&mat1);

  matrix_t mat2;
  mat2.rows = 5;
  mat2.columns = 5;
  mat2.matrix = NULL;
  remove_matrix(&mat2);
  ck_assert_ptr_null(mat2.matrix);
  ck_assert_int_eq(mat2.rows, 5);
  ck_assert_int_eq(mat2.columns, 5);

  matrix_t mat3;
  create_matrix(3, 3, &mat3);
  remove_matrix(&mat3);
  remove_matrix(&mat3);
  ck_assert_ptr_null(mat3.matrix);
}
END_TEST

// Suite для create/remove
Suite* suite_create_remove(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Create and Remove");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_matrix_comprehensive);
  tcase_add_test(tc_core, test_remove_matrix_edge_cases);

  suite_add_tcase(s, tc_core);
  return s;
}