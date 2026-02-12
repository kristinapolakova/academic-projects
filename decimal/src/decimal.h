#ifndef DECIMAL_H
#define DECIMAL_H

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bits[4];
} decimal;

typedef struct {
  unsigned int bits[6];  // расширяем до 192 бит
  int scale;
  int sign;
} big_decimal;

#define MAX_BITS 128
#define MAX_PART_BITS 32
#define MAX_BASE_BITS 96
#define INT_MAX 2147483647

int is_set_bit(int num, int index);
int set_bit(int num, int index);
int reset_bit(int num, int index);
int decimal_is_set_bit(decimal dicimal, int index);
decimal decimal_set_bit(decimal decimal, int index);
decimal decimal_reset_bit(decimal decimal, int index);
decimal decimal_set_bit_val(decimal decimal, int index, int val);

int get_decimal_sign(decimal decimal);
decimal set_decimal_sign(decimal decimal, int sign);
decimal set_decimal_scale(decimal decimal, int scale);
int get_decimal_scale(decimal decimal);
int init_decimal(decimal *value);

int from_int_to_decimal(int src, decimal *dst);
int from_float_to_decimal(float src, decimal *dst);

int from_decimal_to_int(decimal src, int *dst);
int from_decimal_to_float(decimal src, float *dst);

int mul(decimal value_1, decimal value_2, decimal *result);
uint32_t div96by10(uint32_t bits[3]);
int add1to96(uint32_t bits[3]);

int is_less(decimal v1, decimal v2);
int is_less_or_equal(decimal v1, decimal v2);
int is_greater(decimal v1, decimal v2);
int is_greater_or_equal(decimal v1, decimal v2);
int is_equal(decimal v1, decimal v2);
int is_not_equal(decimal v1, decimal v2);

int add(decimal value_1, decimal value_2, decimal *result);
int sub(decimal value_1, decimal value_2, decimal *result);

void to_big(decimal src, big_decimal *dst);
decimal from_big(big_decimal src);
void normalize_scales(big_decimal *a, big_decimal *b);
void big_add(big_decimal a, big_decimal b, big_decimal *res);
void big_mul10(big_decimal *val);
int fit_in_decimal(big_decimal value);
void round_big(big_decimal *value);
int big_compare(big_decimal a, big_decimal b);
void big_sub(big_decimal a, big_decimal b, big_decimal *res);

int copy_decimal(const decimal *src, decimal *dst);
int floor(decimal value, decimal *result);
int round(decimal value, decimal *result);
int truncate(decimal value, decimal *result);
int negate(decimal value, decimal *result);

#endif