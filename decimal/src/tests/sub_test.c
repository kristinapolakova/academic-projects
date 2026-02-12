#include "test_decimal.h"

// static void check_decimal_from_int(int src, decimal *dec, int
// expected_sign,
//                                    int expected_scale) {
//   int sign = get_decimal_sign(*dec);
//   int scale = get_decimal_scale(*dec);

//   ck_assert_int_eq(sign, expected_sign);
//   ck_assert_int_eq(scale, expected_scale);

//   unsigned int abs_src = (src < 0) ? -src : src;
//   ck_assert_uint_eq(dec->bits[0], abs_src);
//   ck_assert_uint_eq(dec->bits[1], 0);
//   ck_assert_uint_eq(dec->bits[2], 0);
// }

START_TEST(test_sub_int_pos_pos_1) {
  // printf("primer 7201-37\n");
  decimal a, b, res;
  from_int_to_decimal(7201, &a);
  from_int_to_decimal(37, &b);

  int code = sub(a, b, &res);
  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);
  ck_assert_int_eq(code, 0);

  check_decimal_from_int(7201 - 37, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_pos_pos_2) {
  // printf("primer 37- 7201\n");
  decimal a, b, res;
  from_int_to_decimal(37, &a);
  from_int_to_decimal(7201, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(37 - 7201, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_neg_pos_1) {
  // printf("primer -72013-3734\n");
  decimal a, b, res;
  from_int_to_decimal(-72013, &a);
  from_int_to_decimal(3734, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-72013 - 3734, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_neg_pos_2) {
  // printf("primer -3734- 72013\n");
  decimal a, b, res;
  from_int_to_decimal(-3734, &a);
  from_int_to_decimal(72013, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-3734 - 72013, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_pos_neg_1) {
  // printf("primer 72013--3734\n");
  decimal a, b, res;
  from_int_to_decimal(72013, &a);
  from_int_to_decimal(-3734, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(72013 + 3734, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_pos_neg_2) {
  // printf("primer 3734- -72013\n");
  decimal a, b, res;
  from_int_to_decimal(3734, &a);
  from_int_to_decimal(-72013, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(3734 + 72013, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_neg_neg_1) {
  // printf("primer -72013--3734\n");
  decimal a, b, res;
  from_int_to_decimal(-72013, &a);
  from_int_to_decimal(-3734, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-72013 + 3734, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_neg_neg_2) {
  // printf("primer -3734--72013\n");
  decimal a, b, res;
  from_int_to_decimal(-3734, &a);
  from_int_to_decimal(-72013, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-3734 + 72013, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_equal_pos_1) {
  // printf("primer 5-5\n");
  decimal a, b, res;
  from_int_to_decimal(5, &a);
  from_int_to_decimal(5, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(0, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_equal_neg_1) {
  // printf("primer -5--5\n");
  decimal a, b, res;
  from_int_to_decimal(-5, &a);
  from_int_to_decimal(-5, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(0, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_pos_zero_1) {
  // printf("primer 5-0\n");
  decimal a, b, res;
  from_int_to_decimal(5, &a);
  from_int_to_decimal(0, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(5, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_neg_zero_1) {
  // printf("primer -5-0\n");
  decimal a, b, res;
  from_int_to_decimal(-5, &a);
  from_int_to_decimal(0, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-5, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_zero_pos_1) {
  // printf("primer 0-5\n");
  decimal a, b, res;
  from_int_to_decimal(0, &a);
  from_int_to_decimal(5, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(-5, &res, 1, 0);
}
END_TEST

START_TEST(test_sub_int_zero_neg_1) {
  // printf("primer 0--5\n");
  decimal a, b, res;
  from_int_to_decimal(0, &a);
  from_int_to_decimal(-5, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(5, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_int_zero_zero_1) {
  // printf("primer 0-0\n");
  decimal a, b, res;
  from_int_to_decimal(0, &a);
  from_int_to_decimal(0, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // int res_int;
  // from_decimal_to_int(res, &res_int);
  // printf("answer %d\n\n", res_int);

  check_decimal_from_int(0, &res, 0, 0);
}
END_TEST

START_TEST(test_sub_float_equal_dot_1) {
  // printf("primer 135.2333 - 13.1567\n");
  decimal a, b, res;
  from_float_to_decimal(135.2333, &a);
  from_float_to_decimal(13.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(135.2333 - 13.1567, &res, 0, 4);
}
END_TEST

START_TEST(test_sub_float_equal_dot_2) {
  // printf("primer -135.2333 - 13.1567\n");
  decimal a, b, res;
  from_float_to_decimal(-135.2333, &a);
  from_float_to_decimal(13.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(-135.2333 - 13.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_equal_dot_3) {
  // printf("primer 13.2333 - 135.1567\n");
  decimal a, b, res;
  from_float_to_decimal(13.2333, &a);
  from_float_to_decimal(135.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(13.2333 - 135.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_equal_dot_4) {
  // printf("primer -13.2333 - 135.1567\n");
  decimal a, b, res;
  from_float_to_decimal(-13.2333, &a);
  from_float_to_decimal(135.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(-13.2333 - 135.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_dif_dot_1) {
  // printf("primer 135.2333 - 13.15\n");
  decimal a, b, res;
  from_float_to_decimal(135.2333, &a);
  from_float_to_decimal(13.15, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(135.2333 - 13.15, &res, 0, 4);
}
END_TEST

START_TEST(test_sub_float_dif_dot_2) {
  // printf("primer -135.2333 - 13.15\n");
  decimal a, b, res;
  from_float_to_decimal(-135.2333, &a);
  from_float_to_decimal(13.15, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(-135.2333 - 13.15, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_dif_dot_3) {
  // printf("primer 13.23 - 135.1567\n");
  decimal a, b, res;
  from_float_to_decimal(13.23, &a);
  from_float_to_decimal(135.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(13.23 - 135.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_dif_dot_4) {
  // printf("primer -13.23 - 135.1567\n");
  decimal a, b, res;
  from_float_to_decimal(-13.23, &a);
  from_float_to_decimal(135.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(-13.23 - 135.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_zero_1) {
  // printf("primer 135.2333 - 0\n");
  decimal a, b, res;
  from_float_to_decimal(135.2333, &a);
  from_float_to_decimal(0, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(135.2333 - 0, &res, 0, 4);
}
END_TEST

START_TEST(test_sub_float_zero_2) {
  // printf("primer 0 - 13.15\n");
  decimal a, b, res;
  from_float_to_decimal(0, &a);
  from_float_to_decimal(13.15, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(0 - 13.15, &res, 1, 2);
}
END_TEST

START_TEST(test_sub_float_not_dec_part_1) {
  // printf("primer 0.2333 - 135.1567\n");
  decimal a, b, res;
  from_float_to_decimal(0.2333, &a);
  from_float_to_decimal(135.1567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(0.2333 - 135.1567, &res, 1, 4);
}
END_TEST

START_TEST(test_sub_float_not_dec_part_2) {
  // printf("primer 0.00004 - 0.15\n");
  decimal a, b, res;
  from_float_to_decimal(0.00004, &a);
  from_float_to_decimal(0.15, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(0.00004 - 0.15, &res, 1, 5);
}
END_TEST

START_TEST(test_sub_int_float_1) {
  // printf("primer 1233457 - 0.1234567\n");
  decimal a, b, res;
  from_float_to_decimal(1233457, &a);
  from_float_to_decimal(0.1234567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(1233457 - 0.1234567, &res, 0, 7);
}
END_TEST

START_TEST(test_sub_int_float_2) {
  // printf("primer -0.1233457 - 1234567\n");
  decimal a, b, res;
  from_float_to_decimal(-0.1233457, &a);
  from_float_to_decimal(1234567, &b);

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);

  check_decimal_float(-0.1233457 - 1234567, &res, 1, 7);
}
END_TEST

START_TEST(test_sub_bigger_than_int_1) {
  // printf("primer bigger_than_int\n");
  decimal a, b, res;

  a.bits[0] = 0xF0000006;
  a.bits[1] = 0;
  a.bits[2] = 0;
  a.bits[3] = 0;

  b.bits[0] = 7;
  b.bits[1] = 0;
  b.bits[2] = 0;
  b.bits[3] = 0;

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  decimal expected;
  expected.bits[0] = 0xEFFFFFFF;
  expected.bits[1] = 0;
  expected.bits[2] = 0;
  expected.bits[3] = 0;

  // float res_fl;
  // from_decimal_to_float(res, &res_fl);
  // printf("answer %lf\n\n", res_fl);
  ck_assert(is_equal(res, expected));
}
END_TEST

START_TEST(test_sub_bigger_than_int_2) {
  // printf("primer bigger_than_int\n");
  decimal a, b, res;
  //  4157546670138709   E C544 4469 2555
  a.bits[0] = 0x44692555;
  a.bits[1] = 0xEC544;
  a.bits[2] = 0;
  a.bits[3] = 0;
  // 517 918 167 774   78 9654 12DE
  b.bits[0] = 0x965412DE;
  b.bits[1] = 0x78;
  b.bits[2] = 0;
  b.bits[3] = 0;

  int code = sub(a, b, &res);

  ck_assert_int_eq(code, 0);

  // 4157028751970935  E C4CB AE15 1277
  decimal expected;
  expected.bits[0] = 0xAE151277;
  expected.bits[1] = 0xEC4CB;
  expected.bits[2] = 0;
  expected.bits[3] = 0;
  ck_assert(is_equal(res, expected));
}
END_TEST

START_TEST(test_sub_bigger_than_int_3) {
  decimal a, b, res;

  // dec 4 294 967 ,419 hex 10000007B
  a.bits[0] = 123;  // 7b
  a.bits[1] = 1;
  a.bits[2] = 0;
  a.bits[3] = 3 << 16;

  // b = 7.001
  b.bits[0] = 7001;
  b.bits[1] = 0;
  b.bits[2] = 0;
  b.bits[3] = 3 << 16;

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  decimal expected;
  expected.bits[0] = 4294960418u;
  expected.bits[1] = 0;
  expected.bits[2] = 0;
  expected.bits[3] = 3 << 16;

  ck_assert(is_equal(res, expected));
}
END_TEST

START_TEST(test_sub_bigger_than_int_4) {
  decimal a, b, res;

  // dec 858993459,7  hex 2 0000 0005
  a.bits[0] = 5;
  a.bits[1] = 2;
  a.bits[2] = 0;
  a.bits[3] = (1 << 31) | (1 << 16);  // sign=1, scale=1

  // b = 7.0
  b.bits[0] = 7;
  b.bits[1] = 0;
  b.bits[2] = 0;
  b.bits[3] = 0;  // scale=0, sign=0

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  // 20000004,B
  decimal expected;
  expected.bits[0] = 0x4B;
  expected.bits[1] = 2;
  expected.bits[2] = 0;
  expected.bits[3] = (1 << 31) | (1 << 16);  // sign=1, scale=1

  ck_assert(is_equal(res, expected));
}
END_TEST

START_TEST(test_sub_bigger_than_int_5) {
  decimal a, b, res;

  // 8 589 935 092      2 0000 01F4
  a.bits[0] = 500;
  a.bits[1] = 2;
  a.bits[2] = 0;
  a.bits[3] = 0;

  // 1000
  b.bits[0] = 1000;
  b.bits[1] = 0;
  b.bits[2] = 0;
  b.bits[3] = 0;

  int code = sub(a, b, &res);
  ck_assert_int_eq(code, 0);

  decimal expected;
  expected.bits[0] = 0xFFFFFE0C;
  expected.bits[1] = 1;
  expected.bits[2] = 0;
  expected.bits[3] = 0;

  ck_assert(is_equal(res, expected));
}
END_TEST

void add_test_sub_int(TCase *tc_core) {
  tcase_add_test(tc_core, test_sub_int_pos_pos_1);
  tcase_add_test(tc_core, test_sub_int_pos_pos_2);

  tcase_add_test(tc_core, test_sub_int_neg_pos_1);
  tcase_add_test(tc_core, test_sub_int_neg_pos_2);

  tcase_add_test(tc_core, test_sub_int_pos_neg_1);
  tcase_add_test(tc_core, test_sub_int_pos_neg_2);

  tcase_add_test(tc_core, test_sub_int_neg_neg_1);
  tcase_add_test(tc_core, test_sub_int_neg_neg_2);

  tcase_add_test(tc_core, test_sub_int_equal_pos_1);
  tcase_add_test(tc_core, test_sub_int_equal_neg_1);

  tcase_add_test(tc_core, test_sub_int_pos_zero_1);
  tcase_add_test(tc_core, test_sub_int_neg_zero_1);
  tcase_add_test(tc_core, test_sub_int_zero_pos_1);
  tcase_add_test(tc_core, test_sub_int_zero_neg_1);
  tcase_add_test(tc_core, test_sub_int_zero_zero_1);
}

void add_test_sub_float(TCase *tc_core) {
  tcase_add_test(tc_core, test_sub_float_equal_dot_1);
  tcase_add_test(tc_core, test_sub_float_equal_dot_2);
  tcase_add_test(tc_core, test_sub_float_equal_dot_3);
  tcase_add_test(tc_core, test_sub_float_equal_dot_4);

  tcase_add_test(tc_core, test_sub_float_dif_dot_1);
  tcase_add_test(tc_core, test_sub_float_dif_dot_2);
  tcase_add_test(tc_core, test_sub_float_dif_dot_3);
  tcase_add_test(tc_core, test_sub_float_dif_dot_4);

  tcase_add_test(tc_core, test_sub_float_zero_1);
  tcase_add_test(tc_core, test_sub_float_zero_2);

  tcase_add_test(tc_core, test_sub_float_not_dec_part_1);
  tcase_add_test(tc_core, test_sub_float_not_dec_part_2);
}

void add_test_sub_diferent(TCase *tc_core) {
  tcase_add_test(tc_core, test_sub_int_float_1);
  tcase_add_test(tc_core, test_sub_int_float_2);

  // tcase_add_test(tc_core, test_sub_overflow_large_numbers);
  // tcase_add_test(tc_core, test_sub_overflow_expect_error);
  // tcase_add_test(tc_core, test_sub_with_scale_overflow_rounding);

  tcase_add_test(tc_core, test_sub_bigger_than_int_1);
  tcase_add_test(tc_core, test_sub_bigger_than_int_2);
  tcase_add_test(tc_core, test_sub_bigger_than_int_3);
  tcase_add_test(tc_core, test_sub_bigger_than_int_4);
  tcase_add_test(tc_core, test_sub_bigger_than_int_5);
}

Suite *suite_sub(void) {
  Suite *s = suite_create("sub");
  TCase *tc_core = tcase_create("Core");

  add_test_sub_int(tc_core);
  add_test_sub_float(tc_core);
  add_test_sub_diferent(tc_core);

  suite_add_tcase(s, tc_core);
  return s;
}
