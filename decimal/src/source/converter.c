#include "decimal.h"

int from_int_to_decimal(int src, decimal *dst) {
  if (dst == NULL) {
    return 1;
  }

  init_decimal(dst);

  if (src < 0) {
    *dst = set_decimal_sign(*dst, 1);
    src *= (-1);
  }
  dst->bits[0] = src;

  return 0;
}

uint32_t div96by10(uint32_t bits[3]);

void reduce_trailing_zeros_simple(decimal *dec) {
  int scale = get_decimal_scale(*dec);
  if (scale == 0) return;

  uint32_t bits[3] = {dec->bits[0], dec->bits[1], dec->bits[2]};
  int remainder, flag = 1;
  while (scale > 0 && flag) {
    remainder = div96by10(bits);

    if (remainder != 0) {
      flag = 0;
    }
    if (flag) {
      scale--;
    }
  }

  unsigned long long carry = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long tmp = (unsigned long long)bits[i] * 10 + carry;
    bits[i] = (unsigned int)(tmp & 0xFFFFFFFFu);
    carry = tmp >> 32;
  }

  dec->bits[0] = bits[0];
  dec->bits[1] = bits[1];
  dec->bits[2] = bits[2];

  *dec = set_decimal_scale(*dec, 0);
  int sign = get_decimal_sign(*dec);

  decimal rem;
  init_decimal(&rem);
  if (sign) remainder = -remainder;
  from_int_to_decimal(remainder, &rem);
  add(*dec, rem, dec);
  *dec = set_decimal_scale(*dec, scale);
}

int from_float_to_decimal(float src, decimal *dst) {
  if (dst == NULL) {
    return 1;
  }

  // число бесконечное или не число
  if (isinf(src) || isnan(src)) {
    return 1;
  }

  init_decimal(dst);
  // число меньше 10^-28 или больше 7.9*10^28 (примечание)
  float abs_src = fabsf(src);
  if ((abs_src > 0 && abs_src < 1e-28f) ||
      abs_src > 7.9228162514264337593543950335e28f) {
    return 1;
  }

  if (abs_src == 0.0) {
    return 0;
  }
  int is_negate = (src >= 0) ? 0 : 1;

  // вычисляем степень 10 на которую нужно домножить, чтобы получить
  // первые 7значимых чисел
  int exp10 = (int)floor(log10(abs_src));
  int scale = 0;
  if (exp10 < 7) {
    scale = 7 - exp10 - 1;
    if (scale > 28) scale = 28;
  }

  // val + 0.5f округление до ближайшего целого (+0.5 гарантия округления до
  // целого, а из-за присваивания дробная часть уйдет)
  double scaled = abs_src * pow(10, scale);
  unsigned long long intval = (unsigned long long)(scaled + 0.5);

  // записываем в bits
  dst->bits[0] = (int)(intval & 0xFFFFFFFF);
  dst->bits[1] = (int)((intval >> 32) & 0xFFFFFFFF);

  *dst = set_decimal_scale(*dst, scale);
  *dst = set_decimal_sign(*dst, is_negate);
  reduce_trailing_zeros_simple(dst);
  return 0;
}

int from_decimal_to_int(decimal src, int *dst) {
  int status = 0;

  if (dst == NULL) {
    status = 1;
  } else {
    *dst = 0;

    int sign = get_decimal_sign(src);
    int scale = get_decimal_scale(src);

    if (src.bits[1] != 0 || src.bits[2] != 0) {
      status = 1;
    } else {
      long long value = src.bits[0];

      while (scale > 0 && value != 0) {
        value /= 10;
        scale--;
      }

      if (value > INT_MAX) {
        status = 1;
      } else {
        *dst = (int)value;
        if (sign) *dst = -*dst;
      }
    }
  }

  return status;
}

int from_decimal_to_float(decimal src, float *dst) {
  int flag = 0;

  if (dst == NULL) {
    flag = 1;
  } else {
    *dst = 0.0f;

    int sign = get_decimal_sign(src);

    int scale = get_decimal_scale(src);

    long double value = 0.0;
    value += (unsigned int)src.bits[0];
    value += (unsigned long long)src.bits[1] * powl(2.0L, 32);
    value += (unsigned long long)src.bits[2] * powl(2.0L, 64);

    if (scale > 0) {
      value /= powl(10.0L, scale);
    }

    if (sign) value = -value;

    *dst = (float)value;
  }
  return flag;
}
