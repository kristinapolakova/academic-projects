#include <stdint.h>

#include "decimal.h"

int is_less(decimal v1, decimal v2) {
  int result = -1;
  if (is_equal(v1, v2))
    result = 0;
  else {
    int sign1 = get_decimal_sign(v1);
    int sign2 = get_decimal_sign(v2);
    if (sign1 != sign2) {
      result = sign1;  // если v1 отрицательное, оно меньше
    }
    if (result < 0) {
      big_decimal v1_big, v2_big;
      to_big(v1, &v1_big);
      to_big(v2, &v2_big);
      normalize_scales(&v1_big, &v2_big);
      v1 = from_big(v1_big);
      v2 = from_big(v2_big);
      for (int i = 2; i >= 0 && result < 0; i--) {
        if (v1.bits[i] != v2.bits[i]) {
          if (!sign1) {
            result = v1.bits[i] < v2.bits[i];  // положительные
          } else {
            result = v1.bits[i] > v2.bits[i];  // отрицательные
          }
        }
      }
    }
  }
  return result;
}

int is_greater(decimal v1, decimal v2) {
  return !is_less(v1, v2) && !is_equal(v1, v2);
}

int is_zero(decimal d) {
  return d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0;
}

int is_equal(decimal v1, decimal v2) {
  int result = 1;
  if (is_zero(v1) && is_zero(v2)) {
    result = 1;
  } else if (get_decimal_sign(v1) != get_decimal_sign(v2)) {
    result = 0;
  } else {
    big_decimal v1_big, v2_big;
    to_big(v1, &v1_big);
    to_big(v2, &v2_big);
    normalize_scales(&v1_big, &v2_big);
    v1 = from_big(v1_big);
    v2 = from_big(v2_big);
    if (result && v1.bits[0] != v2.bits[0]) result = 0;
    if (result && v1.bits[1] != v2.bits[1]) result = 0;
    if (result && v1.bits[2] != v2.bits[2]) result = 0;
  }
  return result;
}

int is_not_equal(decimal v1, decimal v2) {
  return !is_equal(v1, v2);
}

int is_less_or_equal(decimal v1, decimal v2) {
  return is_less(v1, v2) || is_equal(v1, v2);
}

int is_greater_or_equal(decimal v1, decimal v2) {
  return is_greater(v1, v2) || is_equal(v1, v2);
}