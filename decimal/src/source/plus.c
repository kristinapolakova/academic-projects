#include "decimal.h"

int add(decimal value_1, decimal value_2, decimal *result) {
  int status = 0;

  if (result) {
    big_decimal a, b, sum;
    to_big(value_1, &a);
    to_big(value_2, &b);

    normalize_scales(&a, &b);

    if (a.sign == b.sign) {
      big_add(a, b, &sum);
      sum.sign = a.sign;

      while (!fit_in_decimal(sum) && sum.scale > 0) {
        round_big(&sum);
      }
      if (!fit_in_decimal(sum)) {
        status = 1;  // overflow
      } else {
        *result = from_big(sum);
      }
    } else {
      // разные знаки → превращаем в вычитание
      decimal tmp2 = set_decimal_sign(value_2, 0);
      status = sub(value_1, tmp2, result);
    }
  } else {
    status = 1;
  }

  return status;
}

int big_compare(big_decimal a, big_decimal b) {
  int flag = 0;
  for (int i = 5; i >= 0; i--) {
    if (a.bits[i] > b.bits[i] && flag == 0) flag = 1;
    if (a.bits[i] < b.bits[i] && flag == 0) flag = -1;
  }
  return flag;
}

void to_big(decimal src, big_decimal *dst) {
  memset(dst, 0, sizeof(*dst));
  dst->bits[0] = src.bits[0];
  dst->bits[1] = src.bits[1];
  dst->bits[2] = src.bits[2];
  dst->scale = get_decimal_scale(src);
  dst->sign = get_decimal_sign(src);
}

decimal from_big(big_decimal src) {
  decimal dst = {{0}};
  dst.bits[0] = src.bits[0];
  dst.bits[1] = src.bits[1];
  dst.bits[2] = src.bits[2];
  dst = set_decimal_scale(dst, src.scale);
  dst = set_decimal_sign(dst, src.sign);
  return dst;
}

void normalize_scales(big_decimal *a, big_decimal *b) {
  while (a->scale != b->scale) {
    if (a->scale < b->scale) {
      if (a->scale < 28) {
        big_mul10(a);
        a->scale++;
      } else {
        round_big(b);
        b->scale--;
      }
    } else {
      if (b->scale < 28) {
        big_mul10(b);
        b->scale++;
      } else {
        round_big(a);
        a->scale--;
      }
    }
  }
}

void big_add(big_decimal a, big_decimal b, big_decimal *res) {
  memset(res, 0, sizeof(*res));
  unsigned long long carry = 0;

  for (int i = 0; i < 6; i++) {
    unsigned long long tmp =
        (unsigned long long)a.bits[i] + (unsigned long long)b.bits[i] + carry;
    res->bits[i] = (unsigned int)(tmp & 0xFFFFFFFFu);
    carry = tmp >> 32;
  }

  res->scale = a.scale;
}

void big_mul10(big_decimal *val) {
  unsigned long long carry = 0;

  for (int i = 0; i < 6; i++) {
    unsigned long long tmp = (unsigned long long)val->bits[i] * 10 + carry;
    val->bits[i] = (unsigned int)(tmp & 0xFFFFFFFFu);
    carry = tmp >> 32;
  }
}

int fit_in_decimal(big_decimal val) {
  int status = 1;

  if (val.scale < 0 || val.scale > 28) {
    status = 0;
  } else {
    if (val.bits[3] != 0 || val.bits[4] != 0 || val.bits[5] != 0) {
      status = 0;
    }
  }
  return status;
}

void round_big(big_decimal *val) {
  unsigned long long rem = 0;

  for (int i = 5; i >= 0; i--) {
    unsigned long long cur = (rem << 32) | val->bits[i];
    val->bits[i] = (unsigned int)(cur / 10);
    rem = cur % 10;
  }

  // банковское округления
  if (rem > 5 || (rem == 5 && (val->bits[0] & 1))) {
    unsigned long long carry = 1;
    for (int i = 0; i < 6 && carry; i++) {
      unsigned long long tmp = (unsigned long long)val->bits[i] + carry;
      val->bits[i] = (unsigned int)(tmp & 0xFFFFFFFFu);
      carry = tmp >> 32;
    }
  }

  if (val->scale > 0) {
    val->scale--;
  }
}

// void big_sub(big_decimal a, big_decimal b, big_decimal *res)
// {
//   memset(res, 0, sizeof(*res));
//   long long borrow = 0;

//   for (int i = 0; i < 6; i++) {
//     long long tmp = (long long)a.bits[i] - (long long)b.bits[i] - borrow;
//     if (tmp < 0) {
//       tmp += ((long long)1 << 32);
//       borrow = 1;
//     } else {
//       borrow = 0;
//     }
//     res->bits[i] = (unsigned int)(tmp & 0xFFFFFFFFu);
//   }
//   res->scale = a.scale;
// }