#include "test_decimal.h"

START_TEST(test_mul_zero) {
  decimal a = {{123456, 0, 0, 0}};
  decimal b = {{0, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);         // проверяем код ошибки
  ck_assert_int_eq(res.bits[0], 0);  // и каждый разряд
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_mul_one) {
  decimal a = {{987654321, 0, 0, 0}};
  decimal b = {{1, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(res.bits[0], 987654321);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_mul_neg_neg) {
  decimal a = {{2, 0, 0, 0}};
  decimal b = {{3, 0, 0, 0}};
  a = set_decimal_sign(a, 1);
  b = set_decimal_sign(b, 1);

  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_decimal_sign(res), 0);
  ck_assert_int_eq(res.bits[0], 6);
}
END_TEST

START_TEST(test_mul_neg_pos) {
  decimal a = {{5, 0, 0, 0}};
  decimal b = {{7, 0, 0, 0}};
  a = set_decimal_sign(a, 1);

  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_decimal_sign(res), 1);
  ck_assert_int_eq(res.bits[0], 35);
}
END_TEST

START_TEST(test_mul_big_no_overflow) {
  decimal a = {{100000, 0, 0, 0}};
  decimal b = {{100000, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(res.bits[0], 10000000000u & 0xFFFFFFFFu);
  ck_assert_uint_eq(res.bits[1], (10000000000u >> 32) & 0xFFFFFFFFu);
}
END_TEST

START_TEST(test_mul_overflow) {
  decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  decimal b = {{2, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 1);  // overflow
}
END_TEST

START_TEST(test_mul_with_scale) {
  decimal a = {{123, 0, 0, 0}};
  a = set_decimal_scale(a, 2);  // 1.23
  decimal b = {{100, 0, 0, 0}};
  decimal res;

  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_decimal_scale(res), 2);
  ck_assert_int_eq(res.bits[0], 12300);  // 123.00
}
END_TEST

START_TEST(test_mul_scale_overflow_rounding) {
  decimal a = {{1, 0, 0, 0}};
  a = set_decimal_scale(a, 28);
  decimal b = {{1, 0, 0, 0}};
  b = set_decimal_scale(b, 1);  // scale=29

  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 2);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(get_decimal_scale(res), 28);  // scale урезан
}
END_TEST

START_TEST(test_mul_carry_to_middle) {
  // (2^32 - 1) * 2 = 0x1FFFFFFFE
  decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  decimal b = {{2, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq((unsigned)res.bits[0], 0xFFFFFFFE);
  ck_assert_uint_eq((unsigned)res.bits[1], 0x1);
  ck_assert_uint_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_mul_carry_to_high) {
  // число в среднем блоке × 2 → перенос в старший блок
  decimal a = {{0, 0xFFFFFFFF, 0, 0}};
  decimal b = {{2, 0, 0, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(res.bits[0], 0);
  ck_assert_uint_eq((unsigned)res.bits[1], 0xFFFFFFFE);
  ck_assert_uint_eq((unsigned)res.bits[2], 0x1);
}
END_TEST

START_TEST(test_mul_mixed_blocks) {
  // (bits[0]=1, bits[1]=1) × (bits[0]=1, bits[2]=1)
  // результат = (1 + 2^32) * (1 + 2^64) = 1 + 2^32 + 2^64 + 2^96 (но обрезается
  // в 96 бит)
  decimal a = {{1, 1, 0, 0}};
  decimal b = {{1, 0, 1, 0}};
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 1);  // т.к. есть перенос в 2^96
}
END_TEST

START_TEST(test_mul_with_scale_large) {
  // 123456789 (scale=3) * 1000 (scale=0) = 123456789000 (scale=3)
  decimal a = {{123456789, 0, 0, 0}};
  a = set_decimal_scale(a, 3);
  decimal b = {{1000, 0, 0, 0}};
  decimal res;

  int code = mul(a, b, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_decimal_scale(res), 3);

  // Проверяем само число: 123456789000 = 0x1CBE991A8
  ck_assert_uint_eq((unsigned)res.bits[0], 0x1CBE991A08 & 0xFFFFFFFFu);
  ck_assert_uint_eq((unsigned)res.bits[1], (0x1CBE991A08 >> 32) & 0xFFFFFFFFu);
  ck_assert_uint_eq(res.bits[2], 0);
}
END_TEST

START_TEST(test_mul_large_no_overflow) {
  // (2^32) * (2^32) = 2^64
  decimal a = {{0, 1, 0, 0}};  // 2^32
  decimal b = {{0, 1, 0, 0}};  // 2^32
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_uint_eq(res.bits[0], 0);
  ck_assert_uint_eq(res.bits[1], 0);
  ck_assert_uint_eq(res.bits[2], 1);
}
END_TEST

START_TEST(test_with_bank_rounding) {
  decimal a = {{0xFFFFFFFF, 0, 0, 0}};
  decimal b = {{1, 0, 0, 0}};
  a = set_decimal_scale(a, 20);
  b = set_decimal_scale(b, 9);
  decimal res;
  int code = mul(a, b, &res);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(get_decimal_scale(res), 28);
  ck_assert_uint_eq((unsigned)res.bits[0], 0x1999999A);
}
END_TEST

Suite *suite_mul(void) {
  Suite *s = suite_create("mul");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mul_zero);
  tcase_add_test(tc_core, test_mul_one);
  tcase_add_test(tc_core, test_mul_neg_neg);
  tcase_add_test(tc_core, test_mul_neg_pos);
  tcase_add_test(tc_core, test_mul_big_no_overflow);
  tcase_add_test(tc_core, test_mul_overflow);
  tcase_add_test(tc_core, test_mul_with_scale);
  tcase_add_test(tc_core, test_mul_scale_overflow_rounding);
  tcase_add_test(tc_core, test_mul_carry_to_middle);
  tcase_add_test(tc_core, test_mul_carry_to_high);
  tcase_add_test(tc_core, test_mul_mixed_blocks);
  tcase_add_test(tc_core, test_mul_with_scale_large);
  tcase_add_test(tc_core, test_mul_large_no_overflow);
  tcase_add_test(tc_core, test_with_bank_rounding);

  suite_add_tcase(s, tc_core);
  return s;
}