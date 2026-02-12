#include "test_decimal.h"

void sign_scale_decimal() {
  decimal decimal = {{49219, 6, 0, 2147483648U}};
  // int num = decimal_is_set_bit(decimal, 34);
  decimal = decimal_set_bit(decimal, 35);
  int sign = get_decimal_sign(decimal);
  printf("%d\n", sign);
  decimal = set_decimal_sign(decimal, 0);
  sign = get_decimal_sign(decimal);
  printf("%d\n", sign);
  decimal = set_decimal_sign(decimal, 1);
  sign = get_decimal_sign(decimal);
  printf("%d\n", sign);
  decimal = set_decimal_scale(decimal, 10);
  int scale = get_decimal_scale(decimal);
  printf("%d\n", scale);
  sign = get_decimal_sign(decimal);
  printf("%d", sign);

  // int value = 0x12345678;
  // int byte0 = value & 0xFF;  // 0x78
  // // Байт 1
  // int byte1 = (value >> 8) & 0xFF;  // 0x56

  // // Байт 2
  // int byte2 = (value >> 16) & 0xFF;  // 0x34

  // // Старший байт (байт 3)
  // int byte3 = (value >> 24) & 0xFF;  // 0x12
  //                                    // printf("%d", byte1);
}

void set_reset_bit() {
  decimal decimal = {{49219, 6, 0, 0}};
  // int num = decimal_is_set_bit(decimal, 34);
  decimal = decimal_set_bit(decimal, 35);
  printf("%d ", decimal.bits[1]);
  decimal = decimal_reset_bit(decimal, 35);
  printf("%d", decimal.bits[1]);
}

int main() {
  // set_reset_bit();
  // sign_scale_decimal();

  SRunner *runner = srunner_create(suite_from_int_to_decimal());

  srunner_add_suite(runner, suite_from_float_to_decimal());
  srunner_add_suite(runner, suite_from_decimal_to_int());
  srunner_add_suite(runner, suite_from_decimal_to_float());
  srunner_add_suite(runner, suite_add());

  srunner_add_suite(runner, suite_sub());
  srunner_add_suite(runner, suite_mul());

  srunner_add_suite(runner, suite_extra_functions());

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}