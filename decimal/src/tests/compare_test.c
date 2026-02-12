#include "test_decimal.h"

decimal make_decimal(uint32_t low, uint32_t mid, uint32_t high, int scale,
                         int sign) {
  decimal d = {{low, mid, high, 0}};
  d = set_decimal_scale(d, scale);
  d = set_decimal_sign(d, sign);
  return d;
}

START_TEST(test_equal_simple) {
  decimal a = make_decimal(123, 0, 0, 0, 0);
  decimal b = make_decimal(123, 0, 0, 0, 0);
  ck_assert_int_eq(is_equal(a, b), 1);
  ck_assert_int_eq(is_not_equal(a, b), 0);
  ck_assert_int_eq(is_less(a, b), 0);
  ck_assert_int_eq(is_greater(a, b), 0);
}
END_TEST

START_TEST(test_equal_different_scales) {
  decimal a = make_decimal(123, 0, 0, 0, 0);  // 123
  decimal b = make_decimal(123, 0, 0, 2, 0);  // 1.23
  ck_assert_int_eq(is_equal(a, b), 0);
  ck_assert_int_eq(is_not_equal(a, b), 1);
  ck_assert_int_eq(is_less_or_equal(a, b), 0);
  ck_assert_int_eq(is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_equal_normalized) {
  decimal a = make_decimal(12300, 0, 0, 2, 0);  // 123.00
  decimal b = make_decimal(123, 0, 0, 0, 0);    // 123
  ck_assert_int_eq(is_equal(a, b), 1);
  ck_assert_int_eq(is_not_equal(a, b), 0);
  ck_assert_int_eq(is_less_or_equal(a, b), 1);
  ck_assert_int_eq(is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_zero_and_minus_zero) {
  decimal a = make_decimal(0, 0, 0, 5, 0);   // 0.00000
  decimal b = make_decimal(0, 0, 0, 10, 1);  // -0.0000000000
  ck_assert_int_eq(is_equal(a, b), 1);
  ck_assert_int_eq(is_less(a, b), 0);
  ck_assert_int_eq(is_greater(a, b), 0);
}
END_TEST

START_TEST(test_not_equal_sign) {
  decimal a = make_decimal(123, 0, 0, 0, 0);  // +123
  decimal b = make_decimal(123, 0, 0, 0, 1);  // -123
  ck_assert_int_eq(is_equal(a, b), 0);
  ck_assert_int_eq(is_less(a, b), 0);
  ck_assert_int_eq(is_greater(a, b), 1);
}
END_TEST

START_TEST(test_equal_large_numbers) {
  decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  decimal b = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  ck_assert_int_eq(is_not_equal(a, b), 0);
  ck_assert_int_eq(is_less_or_equal(a, b), 1);
  ck_assert_int_eq(is_greater(a, b), 0);
}
END_TEST

START_TEST(test_not_equal_large_numbers) {
  decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  decimal b = make_decimal(0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  ck_assert_int_eq(is_not_equal(a, b), 1);
  ck_assert_int_eq(is_less(a, b), 0);
  ck_assert_int_eq(is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_scale_overflow_not_equal) {
  // 1e28 vs 1e27
  decimal a = make_decimal(1, 0, 0, 28, 0);  // 1 * 10^-28
  decimal b = make_decimal(1, 0, 0, 27, 0);  // 1 * 10^-27
  ck_assert_int_eq(is_not_equal(a, b), 1);
  ck_assert_int_eq(is_less(a, b), 1);
  ck_assert_int_eq(is_greater(a, b), 0);
}
END_TEST

Suite *compare_suite(void) {
  Suite *s = suite_create("compare");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_equal_simple);
  tcase_add_test(tc, test_equal_different_scales);
  tcase_add_test(tc, test_equal_normalized);
  tcase_add_test(tc, test_zero_and_minus_zero);
  tcase_add_test(tc, test_not_equal_sign);
  tcase_add_test(tc, test_equal_large_numbers);
  tcase_add_test(tc, test_not_equal_large_numbers);
  tcase_add_test(tc, test_scale_overflow_not_equal);

  suite_add_tcase(s, tc);
  return s;
}
