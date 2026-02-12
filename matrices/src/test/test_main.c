#include "matrix_test.h"

void run_tests(void) {
  SRunner* sr;
  int number_failed;

  sr = srunner_create(suite_create_remove());
  srunner_add_suite(sr, suite_eq_matrix());
  srunner_add_suite(sr, suite_sum_sub());
  srunner_add_suite(sr, suite_mult());
  srunner_add_suite(sr, suite_transpose());
  srunner_add_suite(sr, suite_determinant());
  srunner_add_suite(sr, suite_calc_complements());
  srunner_add_suite(sr, suite_inverse());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  if (number_failed == 0) {
    printf("\n====================\n");
    printf("ALL TESTS PASSED!\n");
    printf("====================\n");
  } else {
    printf("\n====================\n");
    printf("%d TESTS FAILED\n", number_failed);
    printf("====================\n");
  }
}

int main(void) {
  run_tests();
  return 0;
}