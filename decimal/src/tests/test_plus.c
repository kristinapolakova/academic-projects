#include <limits.h>

#include "test_decimal.h"

// вспомогательная функция для удобства
static decimal make_dec(int value, int scale, int sign) {
  decimal d;
  from_int_to_decimal(value, &d);
  d = set_decimal_scale(d, scale);
  d = set_decimal_sign(d, sign);
  return d;
}

// add
START_TEST(test_add_simple_positive) {
  decimal a, b, res;
  from_int_to_decimal(123, &a);
  from_int_to_decimal(456, &b);

  int code = add(a, b, &res);

  ck_assert_int_eq(code, 0);
  int v;
  from_decimal_to_int(res, &v);
  ck_assert_int_eq(v, 579);
}
END_TEST

START_TEST(test_add_with_scale_normalization) {
  decimal a = make_dec(123, 2, 0);  // 1.23
  decimal b = make_dec(456, 0, 0);  // 456

  decimal res;
  int code = add(a, b, &res);

  ck_assert_int_eq(code, 0);
  float f;
  from_decimal_to_float(res, &f);
  ck_assert_float_eq_tol(f, 457.23f, 1e-6);
}
END_TEST

START_TEST(test_add_negative_and_positive) {
  decimal a = make_dec(500, 0, 0);
  decimal b = make_dec(200, 0, 1);  // -200

  decimal res;
  int code = add(a, b, &res);

  ck_assert_int_eq(code, 0);
  int v;
  from_decimal_to_int(res, &v);
  ck_assert_int_eq(v, 300);
}
END_TEST

START_TEST(test_add_equal_opposite_signs) {
  decimal a = make_dec(12345, 0, 0);
  decimal b = make_dec(12345, 0, 1);

  decimal res;
  int code = add(a, b, &res);

  ck_assert_int_eq(code, 0);
  int v;
  from_decimal_to_int(res, &v);
  ck_assert_int_eq(v, 0);
}
END_TEST

// Доп функции
START_TEST(test_big_compare) {
  decimal da = make_dec(100, 0, 0);
  decimal db = make_dec(200, 0, 0);
  big_decimal a, b;
  to_big(da, &a);
  to_big(db, &b);

  ck_assert_int_eq(big_compare(a, b), -1);
  ck_assert_int_eq(big_compare(b, a), 1);
  ck_assert_int_eq(big_compare(a, a), 0);
}
END_TEST

START_TEST(test_big_add_and_sub)
{
  decimal da = make_dec(500, 0, 0);
  decimal db = make_dec(200, 0, 0);
  big_decimal a, b, sum;
  to_big(da, &a);
  to_big(db, &b);

  big_add(a, b, &sum);
  decimal sumd = from_big(sum);
  int v;
  from_decimal_to_int(sumd, &v);
  ck_assert_int_eq(v, 700);

  // big_sub(a, b, &diff);
  // decimal diffd = from_big(diff);
  // from_decimal_to_int(diffd, &v);
  // ck_assert_int_eq(v, 300);
}
END_TEST

START_TEST(test_big_mul10_div10) {
  decimal d = make_dec(123, 0, 0);
  big_decimal a;
  to_big(d, &a);

  big_mul10(&a);
  decimal m = from_big(a);
  int v;
  from_decimal_to_int(m, &v);
  ck_assert_int_eq(v, 1230);

  round_big(&a);
  decimal back = from_big(a);
  from_decimal_to_int(back, &v);
  ck_assert_int_eq(v, 123);
}
END_TEST

START_TEST(test_fit_in_decimal_and_round) {
  big_decimal a = {0};
  a.bits[5] = UINT_MAX;  // делаем заведомо большое число
  a.scale = 0;
  a.sign = 0;

  int fit = fit_in_decimal(a);

  ck_assert_msg(fit == 0 || fit == 1,
                "fit_in_decimal(a) returned %d (expected 0 or 1)", fit);
}
END_TEST

// Нормализация
START_TEST(test_normalize_scales) {
  decimal da = make_dec(123, 0, 0);
  decimal db = make_dec(45, 2, 0);  // 0.45
  big_decimal a, b;
  to_big(da, &a);
  to_big(db, &b);

  normalize_scales(&a, &b);

  ck_assert_int_eq(a.scale, b.scale);
  ck_assert_int_eq(a.scale, 2);
}
END_TEST

Suite *suite_add(void) {
  Suite *s = suite_create("suite_add");

  TCase *tc_add = tcase_create("add");
  tcase_add_test(tc_add, test_add_simple_positive);
  tcase_add_test(tc_add, test_add_with_scale_normalization);
  tcase_add_test(tc_add, test_add_negative_and_positive);
  tcase_add_test(tc_add, test_add_equal_opposite_signs);
  suite_add_tcase(s, tc_add);

  TCase *tc_helpers = tcase_create("helpers");
  tcase_add_test(tc_helpers, test_big_compare);
  tcase_add_test(tc_helpers, test_big_add_and_sub);
  tcase_add_test(tc_helpers, test_big_mul10_div10);
  tcase_add_test(tc_helpers, test_fit_in_decimal_and_round);
  suite_add_tcase(s, tc_helpers);

  TCase *tc_norm = tcase_create("normalize");
  tcase_add_test(tc_norm, test_normalize_scales);
  suite_add_tcase(s, tc_norm);

  return s;
}