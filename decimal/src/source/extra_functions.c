#include "decimal.h"

int copy_decimal(const decimal *src, decimal *dst) {
  if (dst == NULL) return 1;
  for (int i = 0; i < 4; i++) {
    (*dst).bits[i] = (*src).bits[i];
  }
  return 0;
}

int floor(decimal value, decimal *result) {
  if (result == NULL) {
    return 1;
  }

  int err = truncate(value, result);  // целая часть числа

  decimal diff = {{0, 0, 0, 0}};  // для дробной части
  decimal one = {{1, 0, 0, 0}};

  int sign = get_decimal_sign(value);
  int scale = get_decimal_scale(value);

  if (!err && sign == 1 && scale > 0) {
    // diff = value - целая часть (result)
    err = sub(value, *result, &diff);

    int has_frac = 0;  // ненулевая часть
    for (int i = 0; i < 3 && !has_frac; i++) {
      if (diff.bits[i] != 0) {
        has_frac = 1;  // есть
      }
    }

    // округляем вниз если есть дробная часть (целая - 1)
    if (has_frac) {
      err = sub(*result, one, result);
    }
  }

  *result = set_decimal_scale(*result, 0);
  *result = set_decimal_sign(*result, sign);

  return err;
}

int round(decimal value, decimal *result) {
  int err = 0;

  if (result == NULL) {
    return 1;
  }

  int sign = get_decimal_sign(value);
  int scale = get_decimal_scale(value);

  if (scale == 0) {
    // Если число уже целое
    for (int i = 0; i < 4; i++) {
      (*result).bits[i] = value.bits[i];
    }
  } else {
    uint32_t bits[3] = {value.bits[0], value.bits[1],
                        value.bits[2]};  // копируем мантиссу
    int local_scale = scale;

    uint32_t remainder = 0;  // для остатка

    for (int i = 0; i < local_scale; i++) {
      remainder = div96by10(bits);
    }
    // Округляем вверх, если остаток >= 5
    int need_add = (remainder >= 5) ? 1 : 0;

    if (need_add) {
      err = add1to96(bits);
    }

    if (!err) {
      (*result).bits[0] = bits[0];
      (*result).bits[1] = bits[1];
      (*result).bits[2] = bits[2];
      (*result).bits[3] = 0;
    }

    if (!err) {
      *result = set_decimal_scale(*result, 0);
      *result = set_decimal_sign(*result, sign);
    }
  }

  return err;
}

int truncate(decimal value, decimal *result) {
  if (copy_decimal(&value, result) != 0) return 1;

  int scale = get_decimal_scale(value);

  if (scale > 0 && scale < 29) {
    // Пока scale > 0 делим мантиссу на 10, уменьшая scale
    while (scale > 0) {
      div96by10((uint32_t *)&(result->bits[0]));
      scale--;
    }
    *result = set_decimal_scale(*result, 0);
  }

  int sign = get_decimal_sign(value);
  *result = set_decimal_sign(*result, sign);

  return 0;
}

int negate(decimal value, decimal *result) {
  if (copy_decimal(&value, result) != 0) return 1;

  int current_sign = get_decimal_sign(value);
  *result = set_decimal_sign(*result, !current_sign);

  return 0;
}
