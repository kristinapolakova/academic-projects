#include "test_decimal.h"

// сравнение decimal с int
void check_decimal_from_int(int src, decimal *dec, int expected_sign,
                            int expected_scale) {
  // проверка знака и степени
  int sign = get_decimal_sign(*dec);
  int scale = get_decimal_scale(*dec);

  ck_assert_int_eq(scale, expected_scale);
  ck_assert_int_eq(sign, expected_sign);

  // проверка мантисы
  unsigned int abs_src = (src < 0) ? -src : src;
  ck_assert_uint_eq(dec->bits[0], abs_src);

  ck_assert_uint_eq(dec->bits[1], 0);
  ck_assert_uint_eq(dec->bits[2], 0);
}

START_TEST(test_from_int_to_decimal_positive) {
  int src = 123456;
  decimal dst;
  int res = from_int_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_from_int(src, &dst, 0, 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative) {
  int src = -7890;
  decimal dst;
  int res = from_int_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_from_int(src, &dst, 1, 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_zero) {
  int src = 0;
  decimal dst;
  int res = from_int_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_from_int(src, &dst, 0, 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_nullptr) {
  int src = 123;
  int res = from_int_to_decimal(src, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

Suite *suite_from_int_to_decimal(void) {
  Suite *s = suite_create("from_int_to_decimal");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_from_int_to_decimal_positive);
  tcase_add_test(tc_core, test_from_int_to_decimal_negative);
  tcase_add_test(tc_core, test_from_int_to_decimal_zero);
  tcase_add_test(tc_core, test_from_int_to_decimal_nullptr);

  suite_add_tcase(s, tc_core);
  return s;
}

// сравнение decimal с float
void check_decimal_float(float src, decimal *dec, int expected_sign,
                         int expected_scale) {
  int sign = get_decimal_sign(*dec);
  int scale = get_decimal_scale(*dec);

  ck_assert_int_eq(sign, expected_sign);
  ck_assert_int_eq(scale, expected_scale);

  // 1 часть числа младшие и средние биты (64 бита)
  // приводим к unsigned long long для безопасного сдвига
  unsigned long long low64 =
      ((unsigned long long)(unsigned int)dec->bits[1] << 32) |
      (unsigned int)dec->bits[0];
  // 2 часть старшие биты (32 бита)
  unsigned int high32 = (unsigned int)dec->bits[2];

  // собираем число в double, (double)high32 * pow(2.0, 64) по факту сдвиг
  // на 64 бита только в double
  double int_value = (double)high32 * pow(2.0, 64) + (double)low64;

  // получаем значение с запятой
  double dec_value = int_value / pow(10.0, scale);

  if (sign) dec_value = -dec_value;

  // для больших чисел позволяем относительную ошибку пропорционально величине,
  // а для маленьких — минимальную абсолютную погрешность
  double epsilon = fmax(1e-7, 1e-7 * fabs(src));

  // сравниваем значения с учетом эпсилон
  ck_assert_msg(fabs(dec_value - src) <= epsilon,
                "Decimal value %.8f differs from float %.8f by more than %.8f",
                dec_value, src, epsilon);
  // printf("dec: %.8f, float: %.8f\n", dec_value, src);
}

START_TEST(test_from_float_to_decimal_positive) {
  float src = 123.456f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_float(src, &dst, 0, 3);
}
END_TEST

START_TEST(test_from_float_to_decimal_negative) {
  float src = -789.0123f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_float(src, &dst, 1, 4);
}
END_TEST

START_TEST(test_from_float_to_decimal_zero) {
  float src = 0.0f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_float(src, &dst, 0, 0);
}
END_TEST

START_TEST(test_from_float_to_decimal_small_value) {
  float src = 1e-30f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_large_frac_part) {
  float src = 0.1234567f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_float(src, &dst, 0, 7);
}
END_TEST

START_TEST(test_from_float_to_decimal_large_int_part) {
  float src = 1234567.f;
  decimal dst;
  int res = from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  check_decimal_float(src, &dst, 0, 0);
}
END_TEST

START_TEST(test_from_float_to_decimal_inf_nan) {
  decimal dst;

  int res_inf = from_float_to_decimal(INFINITY, &dst);
  ck_assert_int_eq(res_inf, 1);

  int res_nan = from_float_to_decimal(NAN, &dst);
  ck_assert_int_eq(res_nan, 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_nullptr) {
  float src = 42.0f;
  int res = from_float_to_decimal(src, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

Suite *suite_from_float_to_decimal(void) {
  Suite *s = suite_create("from_float_to_decimal");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_from_float_to_decimal_positive);
  tcase_add_test(tc_core, test_from_float_to_decimal_negative);
  tcase_add_test(tc_core, test_from_float_to_decimal_zero);
  tcase_add_test(tc_core, test_from_float_to_decimal_small_value);
  tcase_add_test(tc_core, test_from_float_to_decimal_large_frac_part);
  tcase_add_test(tc_core, test_from_float_to_decimal_large_int_part);
  tcase_add_test(tc_core, test_from_float_to_decimal_inf_nan);
  tcase_add_test(tc_core, test_from_float_to_decimal_nullptr);

  suite_add_tcase(s, tc_core);
  return s;
}

void check_decimal_to_int(decimal *dec, int expected_value,
                          int expected_result) {
  int actual_value = 0;
  int res = from_decimal_to_int(*dec, &actual_value);

  ck_assert_int_eq(res, expected_result);
  if (res == 0) {
    ck_assert_int_eq(actual_value, expected_value);
  }
}

START_TEST(test_from_decimal_to_int_positive) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 123456;

  check_decimal_to_int(&dec, 123456, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 7890;
  dec = set_decimal_sign(dec, 1);

  check_decimal_to_int(&dec, -7890, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_zero) {
  decimal dec;
  init_decimal(&dec);

  check_decimal_to_int(&dec, 0, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_with_scale) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 1234500;
  dec = set_decimal_scale(dec, 2);  // 12345.00

  check_decimal_to_int(&dec, 12345, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_with_fraction_cut) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 9876;  // 987.6
  dec = set_decimal_scale(dec, 1);

  check_decimal_to_int(&dec, 987, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 0xFFFFFFFF;
  dec.bits[1] = 1;  // больше 32 бит

  check_decimal_to_int(&dec, 0, 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_nullptr) {
  decimal dec;
  init_decimal(&dec);

  int res = from_decimal_to_int(dec, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

Suite *suite_from_decimal_to_int(void) {
  Suite *s = suite_create("from_decimal_to_int");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_from_decimal_to_int_positive);
  tcase_add_test(tc_core, test_from_decimal_to_int_negative);
  tcase_add_test(tc_core, test_from_decimal_to_int_zero);
  tcase_add_test(tc_core, test_from_decimal_to_int_with_scale);
  tcase_add_test(tc_core, test_from_decimal_to_int_with_fraction_cut);
  tcase_add_test(tc_core, test_from_decimal_to_int_overflow);
  tcase_add_test(tc_core, test_from_decimal_to_int_nullptr);

  suite_add_tcase(s, tc_core);
  return s;
}

void check_decimal_to_float(decimal *dec, float expected) {
  float actual = 0.0f;
  int res = from_decimal_to_float(*dec, &actual);
  ck_assert_int_eq(res, 0);

  float epsilon = fmaxf(1e-7f, fabsf(expected) * 1e-7f);
  ck_assert_msg(fabsf(actual - expected) <= epsilon,
                "Expected %.7f, got %.7f, diff %.7f", expected, actual,
                fabsf(actual - expected));
}

START_TEST(test_from_decimal_to_float_positive) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 31415;
  dec = set_decimal_scale(dec, 2);  // 314.15

  check_decimal_to_float(&dec, 314.15f);
}
END_TEST

START_TEST(test_from_decimal_to_float_negative) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 500;
  dec = set_decimal_scale(dec, 1);  // 50.0
  dec = set_decimal_sign(dec, 1);   // -50.0

  check_decimal_to_float(&dec, -50.0f);
}
END_TEST

START_TEST(test_from_decimal_to_float_zero) {
  decimal dec;
  init_decimal(&dec);

  check_decimal_to_float(&dec, 0.0f);
}
END_TEST

START_TEST(test_from_decimal_to_float_big_value) {
  decimal dec;
  init_decimal(&dec);
  dec.bits[0] = 0xFFFFFFFF;
  dec.bits[1] = 0xFFFFFFFF;
  dec.bits[2] = 0x1;  // большое число, но не переполнение

  float actual = 0.0f;
  int res = from_decimal_to_float(dec, &actual);
  ck_assert_int_eq(res, 0);
  ck_assert_float_lt(actual, 1e30f);
}
END_TEST

START_TEST(test_from_decimal_to_float_nullptr) {
  decimal dec;
  init_decimal(&dec);
  int res = from_decimal_to_float(dec, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

Suite *suite_from_decimal_to_float(void) {
  Suite *s = suite_create("from_decimal_to_float");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_from_decimal_to_float_positive);
  tcase_add_test(tc_core, test_from_decimal_to_float_negative);
  tcase_add_test(tc_core, test_from_decimal_to_float_zero);
  tcase_add_test(tc_core, test_from_decimal_to_float_big_value);
  tcase_add_test(tc_core, test_from_decimal_to_float_nullptr);

  suite_add_tcase(s, tc_core);
  return s;
}
