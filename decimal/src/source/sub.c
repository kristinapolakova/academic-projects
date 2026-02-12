#include "decimal.h"

int get_num_sign_of_decimal(decimal val) {
  int num_signs = 1, flag = 0;
  for (int i = MAX_BASE_BITS - 1; i >= 0 && flag == 0; i--) {
    if (decimal_is_set_bit(val, i)) {
      num_signs = i + 1;
      flag = 1;
    }
  }
  return num_signs;
}

decimal inversia_decimal(decimal val, int num_sign) {
  for (int i = 0; i < num_sign; i++) {
    if (decimal_is_set_bit(val, i)) {
      val = decimal_reset_bit(val, i);
    } else {
      val = decimal_set_bit(val, i);
    }
  }
  return val;
}

int big_or_small(decimal value_1, decimal value_2,
                     big_decimal *a, big_decimal *b, int *flag_perm) {
  int num_signs_big = 0;

  // нужно для сравнения, пока не добавят к нему нормалицацию
  to_big(value_1, a);
  to_big(value_2, b);
  normalize_scales(a, b);
  value_1 = from_big(*a);
  value_2 = from_big(*b);

  if (!is_greater_or_equal(value_1, value_2)) {
    decimal temp = value_1;
    value_1 = value_2;
    value_2 = temp;

    *flag_perm = 1;
  }
  num_signs_big = get_num_sign_of_decimal(value_1);
  value_2 = inversia_decimal(value_2, num_signs_big);

  to_big(value_1, a);
  to_big(value_2, b);
  return num_signs_big;
}

int sub_big_decimal(decimal value_1, decimal value_2,
                        decimal *result, int *flag_perm) {
  int exit_code = 0;

  big_decimal a, b, sum;
  memset(&a, 0, sizeof(a));
  memset(&b, 0, sizeof(b));
  memset(&sum, 0, sizeof(sum));

  int num_signs_big = big_or_small(value_1, value_2, &a, &b, flag_perm);

  normalize_scales(&a, &b);

  big_decimal one, dop;
  memset(&one, 0, sizeof(one));
  memset(&dop, 0, sizeof(dop));
  one.bits[0] = 1;

  big_add(b, one, &dop);
  big_add(a, dop, &sum);

  int flag_m = 0;
  if (sum.bits[3] == 1) {
    sum.bits[3] = 0;
    flag_m = 1;
  }

  while (!fit_in_decimal(sum) && sum.scale > 0) {
    round_big(&sum);
  }
  if (!fit_in_decimal(sum)) {
    exit_code = 1;  // overflow
  } else {
    *result = from_big(sum);
  }

  int num_signs_res = get_num_sign_of_decimal(*result);
  if (num_signs_res > num_signs_big && !flag_m) {
    *result = decimal_reset_bit(*result, num_signs_res - 1);
  }

  int scale1 = get_decimal_scale(value_1);
  int scale2 = get_decimal_scale(value_2);

  int bigger_scale = (scale1 > scale2) ? scale1 : scale2;
  *result = set_decimal_scale(*result, bigger_scale);

  return exit_code;
}

int sub(decimal value_1, decimal value_2, decimal *result) {
  int exit_code = 0;
  if (result != NULL) {
    init_decimal(result);

    int flag_perm = 0;
    int sign1 = get_decimal_sign(value_1);
    int sign2 = get_decimal_sign(value_2);

    if (!sign1 && sign2) {  // + -
      value_2 = set_decimal_sign(value_2, 0);
      exit_code = add(value_1, value_2, result);
    } else if (sign1 && !sign2) {  // - +
      value_1 = set_decimal_sign(value_1, 0);
      exit_code = add(value_1, value_2, result);
      flag_perm = 1;
    } else {
      if (sign1 && sign2) {  // - -

        value_1 = set_decimal_sign(value_1, 0);
        value_2 = set_decimal_sign(value_2, 0);

        if (is_greater(value_1, value_2)) {
          flag_perm = 1;
        } else {
          decimal c;
          c = value_1;
          value_1 = value_2;
          value_2 = c;
        }
      }

      exit_code = sub_big_decimal(value_1, value_2, result, &flag_perm);
    }

    int final_sign = flag_perm ? 1 : 0;
    *result = set_decimal_sign(*result, final_sign);
  }
  return exit_code;
}