#include "decimal.h"

int is_set_bit(int num, int index) { return !!(num & (1 << index)); }

int set_bit(int num, int index) { return num | (1 << index); }

int reset_bit(int num, int index) { return num & ~(1 << index); }

int decimal_is_set_bit(decimal decimal, int index) {
  return is_set_bit(decimal.bits[index / MAX_PART_BITS],
                        index % MAX_PART_BITS);
}

decimal decimal_set_bit(decimal decimal, int index) {
  decimal.bits[index / MAX_PART_BITS] =
      set_bit(decimal.bits[index / MAX_PART_BITS], index % MAX_PART_BITS);
  return decimal;
}

decimal decimal_reset_bit(decimal decimal, int index) {
  decimal.bits[index / MAX_PART_BITS] =
      reset_bit(decimal.bits[index / MAX_PART_BITS], index % MAX_PART_BITS);
  return decimal;
}

int get_decimal_sign(decimal decimal) {
  return is_set_bit(decimal.bits[3], 31);
}

decimal set_decimal_sign(decimal decimal, int sign) {
  if (sign)
    decimal.bits[3] = set_bit(decimal.bits[3], 31);
  else
    decimal.bits[3] = reset_bit(decimal.bits[3], 31);
  return decimal;
}

decimal set_decimal_scale(decimal decimal, int scale) {
  decimal.bits[3] &= ~(0xFFFFFF);
  decimal.bits[3] |= (scale & 0xFF) << 16;
  return decimal;
}

int get_decimal_scale(decimal decimal) {
  return (decimal.bits[3] >> 16) & 0xFF;
}

int init_decimal(decimal *value) {
  int flag = 0;

  if (value == NULL) {
    flag = 1;
  } else {
    (*value).bits[0] = 0;
    (*value).bits[1] = 0;
    (*value).bits[2] = 0;
    (*value).bits[3] = 0;

    int i = 0;
    while (i < 4 && (*value).bits[i] == 0) {
      i++;
    }
    if (i < 4) {
      flag = 2;
    }
  }

  return flag;
}