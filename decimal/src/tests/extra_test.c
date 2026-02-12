#include "test_decimal.h"

START_TEST(test_copy_decimal_basic) {
  decimal src = {{12345, 678, 0, 0}};
  decimal dst;
  int res = copy_decimal(&src, &dst);
  ck_assert_int_eq(res, 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(src.bits[i], dst.bits[i]);
  }
}
END_TEST

// смена знака в функции
START_TEST(test_negate_basic) {
  decimal value = {{123, 0, 0, 0}};  // положительное число
  decimal result;

  int res = negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_sign(value), 0);   // 0
  ck_assert_int_eq(get_decimal_sign(result), 1);  // 1

  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(value.bits[i], result.bits[i]);
  }
}
END_TEST
// отрицательные числа
START_TEST(test_negate_negativ) {
  decimal value = {{123, 0, 0, 1 << 31}};
  decimal result;
  int res = negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_sign(value), 1);
  ck_assert_int_eq(get_decimal_sign(result), 0);

  for (int i = 0; i < 3; i++) {
    ck_assert_int_eq(value.bits[i], result.bits[i]);
  }
}
END_TEST

// положительное число floor
START_TEST(test_floor_basic) {
  decimal value = {{9999, 0, 0, 0}};
  decimal result = {{9999, 0, 0, 0}};

  int res = floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_sign(value), 0);
  ck_assert_int_eq(get_decimal_sign(result), 0);
  ck_assert_int_eq(get_decimal_scale(value), 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(value.bits[i], result.bits[i]);
  }
}
END_TEST

// floor с отрицательным числом (-3.7 -> -4)
START_TEST(test_floor_negative_fraction) {
  decimal value = {{37, 0, 0, 2147483648U}};  // знак установлен (bit 31)
  // scale = 1, чтобы значение было -3.7
  value.bits[3] = (1 << 16) | (1 << 31);            // scale=1, sign=1
  decimal expected = {{4, 0, 0, 2147483648U}};  // floor = -4
  expected.bits[3] = (0 << 16) | (1 << 31);         // scale=0, sign=1

  decimal result;
  int res = floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);
  ck_assert_uint_eq(result.bits[0], expected.bits[0]);
}
END_TEST

// floor положительное число округляется в меньшую сторону
START_TEST(test_floor_basic_min) {
  decimal value = {{57, 0, 0, 1 << 16}};  // 5.7
  decimal expected = {{5, 0, 0, 0}};      // 5

  decimal result;

  int res = floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 0);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_floor_min_in_min) {
  decimal value = {{00001, 0, 0, (4 << 16) | (1 << 31)}};  //-0.0001
  decimal expected = {{1, 0, 0, 1 << 31}};                 //-1

  decimal result;
  int res = floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_floor_min_big) {
  decimal value = {
      {705032705, 1, 0, (9 << 16) | (1 << 31)}};  //-5.000000001
  decimal expected = {{6, 0, 0, 1 << 31}};    //-6

  decimal result;
  int res = floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// округление база
START_TEST(test_round_basic) {
  decimal value = {{1234567890, 0, 0, 7 << 16}};  // 123.4567890, scale=7
  decimal result;
  int res = round(value, &result);
  ck_assert_int_eq(res, 0);

  int scale = get_decimal_scale(result);
  ck_assert_int_eq(scale, 0);

  // Округ значение должно быть 123
  decimal expected = {{123, 0, 0, 0}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_round_negate) {
  decimal value = {{123456, 0, 0, (3 << 16) | (1 << 31)}};  // -123.456
  decimal result;
  int res = round(value, &result);
  ck_assert_int_eq(res, 0);

  int scale = get_decimal_scale(result);
  ck_assert_int_eq(scale, 0);
  ck_assert_int_eq(get_decimal_sign(value), 1);   // 1
  ck_assert_int_eq(get_decimal_sign(result), 1);  // 1

  // Округ значение должно быть -123
  decimal expected = {{123, 0, 0, (1 << 31)}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Округление числа с первой дробной цифрой равной 5
START_TEST(test_round_half_up) {
  decimal value = {{125000, 0, 0, 4 << 16}};  // 12.5000
  decimal expected = {{13, 0, 0, 0}};         // 13
  decimal result;
  int res = round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 0);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Округление отрицательного числа
START_TEST(test_round_negative_down) {
  decimal value = {
      {15700, 0, 0, (4 << 16) | (1 << 31)}};    // -1.5700 scale=4
  decimal expected = {{2, 0, 0, 1 << 31}};  // -2
  decimal result;
  int res = round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Округление отрицательного числа с  >= 5
START_TEST(test_round_negative_up) {
  decimal value = {
      {12700, 0, 0, (4 << 16) | (1 << 31)}};    // -1.2700 scale=4
  decimal expected = {{1, 0, 0, 1 << 31}};  // -1
  decimal result;
  int res = round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(test_truncate_basic) {
  decimal value = {{4500, 0, 0, 2 << 16}};  // 45.00
  decimal result;
  int res = truncate(value, &result);
  ck_assert_int_eq(res, 0);

  int scale = get_decimal_scale(result);
  ck_assert_int_eq(scale, 0);

  decimal expected = {{45, 0, 0, 0}};  // 45
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Усечение числа без дробной части
START_TEST(test_truncate_no_fraction) {
  decimal value = {{42, 0, 0, 0}};     // 42
  decimal expected = {{42, 0, 0, 0}};  // 42
  decimal result;
  int res = truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Усечение очень маленького числа (0.0001234)
START_TEST(test_truncate_small_fraction) {
  decimal value = {{00001234, 0, 0, 7 << 16}};
  decimal expected = {{0, 0, 0, 0}};
  decimal result;
  int res = truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Усечение положительного числа с дробной частью
START_TEST(test_truncate_positive_fraction) {
  decimal value = {{12345, 0, 0, 3 << 16}};  // 12.345 scale=3
  decimal expected = {{12, 0, 0, 0}};        // 12 scale=0
  decimal result;
  int res = truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 0);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

// Усечение отрицательного числа с дробной частью
START_TEST(test_truncate_negative_fraction) {
  decimal value = {
      {98765, 0, 0, (4 << 16) | (1 << 31)}};    // -9.8765 scale=4, sign=1
  decimal expected = {{9, 0, 0, 1 << 31}};  // -9 scale=0, sign=1
  decimal result;
  int res = truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(get_decimal_scale(result), 0);
  ck_assert_int_eq(get_decimal_sign(result), 1);
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

Suite *suite_extra_functions(void) {
  Suite *s = suite_create("Extra Functions");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_copy_decimal_basic);
  tcase_add_test(tc_core, test_negate_basic);
  tcase_add_test(tc_core, test_negate_negativ);

  tcase_add_test(tc_core, test_floor_negative_fraction);
  tcase_add_test(tc_core, test_floor_basic);
  tcase_add_test(tc_core, test_floor_basic_min);
  tcase_add_test(tc_core, test_floor_min_in_min);
  tcase_add_test(tc_core, test_floor_min_big);

  tcase_add_test(tc_core, test_round_basic);
  tcase_add_test(tc_core, test_round_negate);
  tcase_add_test(tc_core, test_round_half_up);
  tcase_add_test(tc_core, test_round_negative_up);
  tcase_add_test(tc_core, test_round_negative_down);

  tcase_add_test(tc_core, test_truncate_basic);
  tcase_add_test(tc_core, test_truncate_no_fraction);
  tcase_add_test(tc_core, test_truncate_small_fraction);
  tcase_add_test(tc_core, test_truncate_positive_fraction);
  tcase_add_test(tc_core, test_truncate_negative_fraction);

  suite_add_tcase(s, tc_core);

  return s;
}
